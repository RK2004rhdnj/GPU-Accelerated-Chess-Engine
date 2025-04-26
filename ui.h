#ifndef UI_H
#define UI_H

#include "defs.h"
#include <ncurses.h>

// Initialize the UI with ncurses
void InitUI(void);

// Clean up the UI and restore terminal
void CleanupUI(void);

// Display the chess board and game state
void DisplayBoard(const S_BOARD *pos);

// Get a move from the user
char* GetUserMove(void);

// Display engine thinking information
void DisplayThinking(const char* thinking);

// Display move history
void DisplayHistory(const S_BOARD *pos);

// Main UI loop for interactive play
void UI_Loop(S_BOARD *pos, S_SEARCHINFO *info);

#endif // UI_H 