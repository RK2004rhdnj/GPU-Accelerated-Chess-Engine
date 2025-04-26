#include "stdio.h"
#include "defs.h"
#include "ui.h"
#include <string.h>

static char userMove[6];

// Color pairs
#define WHITE_SQUARE_PAIR 1
#define BLACK_SQUARE_PAIR 2
#define HIGHLIGHT_PAIR 3

void InitUI(void) {
    initscr();            // Initialize ncurses
    start_color();        // Enable colors
    cbreak();             // Disable line buffering
    noecho();            // Don't echo user input
    keypad(stdscr, TRUE); // Enable keypad
    curs_set(1);         // Show cursor
    
    // Initialize color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLACK);   // White pieces
    init_pair(2, COLOR_RED, COLOR_BLACK);     // Black pieces
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   // Board coordinates
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);  // Status messages
}

void CleanupUI(void) {
    endwin();  // Clean up ncurses
}

char* GetUserMove(void) {
    mvprintw(15, 0, "Enter move (e.g. e2e4): ");
    refresh();
    
    memset(userMove, 0, sizeof(userMove));
    echo();  // Show user input
    getnstr(userMove, 5);
    noecho();  // Hide input again
    
    mvprintw(15, 0, "                                ");  // Clear prompt
    refresh();
    
    return userMove;
}

void DisplayBoard(const S_BOARD *pos) {
    clear();
    
    // Print board frame
    mvprintw(0, 0, "   +---+---+---+---+---+---+---+---+");
    
    int sq, file, rank, piece;
    
    #pragma acc parallel loop private(sq, file, rank, piece) shared(pos) schedule(dynamic)
    for(sq = 0; sq < BRD_SQ_NUM; ++sq) {
        file = FilesBrd[sq];
        rank = RanksBrd[sq];
        piece = pos->pieces[sq];
        if(piece != EMPTY && piece != OFFBOARD) {
            mvaddch(rank, file*2, PceChar[piece]);
        }
    }
    
    // Print files
    mvprintw(9, 4, "a   b   c   d   e   f   g   h");
    
    // Print side to move
    attron(COLOR_PAIR(4));
    mvprintw(11, 0, "Side to move: %s", pos->side == WHITE ? "White" : "Black");
    
    // Print castling permissions
    mvprintw(12, 0, "Castling: %s%s%s%s", 
        (pos->castlePerm & WKCA) ? "K" : "-",
        (pos->castlePerm & WQCA) ? "Q" : "-",
        (pos->castlePerm & BKCA) ? "k" : "-",
        (pos->castlePerm & BQCA) ? "q" : "-");
    
    // Print en passant square
    mvprintw(13, 0, "En Passant: %s", 
        pos->enPas != NO_SQ ? PrSq(pos->enPas) : "none");
    attroff(COLOR_PAIR(4));
    
    refresh();
}

void DisplayThinking(const char* thinking) {
    mvprintw(16, 0, "%s", thinking);
    clrtoeol();
    refresh();
}

void DisplayHistory(const S_BOARD *pos) {
    mvprintw(20, 0, "Move History:");
    clrtoeol();
    
    // Clear all history lines first
    for(int i = 0; i < 5; i++) {
        mvprintw(21 + i, 0, "");
        clrtoeol();
    }
    
    // Only show last 10 moves
    int startMove = pos->hisPly > 10 ? pos->hisPly - 10 : 0;
    
    #pragma acc parallel loop private(i) shared(pos) schedule(dynamic)
    for(i = 0; i < pos->hisPly; ++i) {
        if(i % 2 == 0) {
            mvprintw(10+i/2, 0, "%d. ", i/2+1);
        }
        mvprintw(10+i/2, 4+i%2*6, "%s ", PrMove(pos->history[i].move));
    }
    refresh();
}

void UI_Loop(S_BOARD *pos, S_SEARCHINFO *info) {
    InitUI();
    
    // Initialize a fresh game
    ResetBoard(pos);
    ParseFen(START_FEN, pos);
    pos->hisPly = 0;
    
    while(TRUE) {
        DisplayBoard(pos);
        DisplayHistory(pos);
        
        if(pos->side == WHITE) {
            // Human's turn
            char* moveStr = GetUserMove();
            if(strcmp(moveStr, "quit") == 0) break;
            if(strcmp(moveStr, "reset") == 0) {
                ResetBoard(pos);
                ParseFen(START_FEN, pos);
                pos->hisPly = 0;
                continue;
            }
            
            int move = ParseMove(moveStr, pos);
            if(move != NOMOVE) {
                MakeMove(pos, move);
                pos->ply = 0;
            } else {
                attron(COLOR_PAIR(2));  // Red for error
                mvprintw(17, 0, "Invalid move! Press any key to continue...");
                refresh();
                getch();
                mvprintw(17, 0, "                                          ");
                attroff(COLOR_PAIR(2));
            }
        } else {
            // Engine's turn
            attron(COLOR_PAIR(4));  // Yellow for thinking
            mvprintw(16, 0, "Engine is thinking...");
            refresh();
            attroff(COLOR_PAIR(4));
            
            info->depth = 6;
            info->timeset = FALSE;
            info->starttime = GetTimeMs();
            info->stoptime = 0;
            info->stopped = FALSE;
            info->GAME_MODE = CONSOLEMODE;
            
            SearchPosition(pos, info);
            
            mvprintw(16, 0, "                      ");
            refresh();
            pos->ply = 0;
        }
        
        // Check for game end conditions
        S_MOVELIST list[1];
        GenerateAllMoves(pos, list);
        int MoveNum = 0;
        int found = 0;
        #pragma acc parallel loop private(MoveNum, found) shared(list)
        for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
            if(list->moves[MoveNum].move == move) {
                found = TRUE;
                break;
            }
        }
        
        if(found == 0) {
            attron(COLOR_PAIR(4));  // Yellow for game over
            mvprintw(17, 0, "Game Over! Press any key to exit...");
            refresh();
            attroff(COLOR_PAIR(4));
            getch();
            break;
        }
    }
    
    CleanupUI();
} 