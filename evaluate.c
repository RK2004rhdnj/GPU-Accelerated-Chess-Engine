// evaluate.c

#include "stdio.h"
#include "defs.h"

const int PawnIsolated = -10;
const int PawnPassed[8] = { 0, 5, 10, 20, 35, 60, 100, 200 }; 
const int RookOpenFile = 10;
const int RookSemiOpenFile = 5;
const int QueenOpenFile = 5;
const int QueenSemiOpenFile = 3;
const int BishopPair = 30;

const int PawnTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int KnightTable[64] = {
0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0		
};

const int BishopTable[64] = {
0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int RookTable[64] = {
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0		
};

const int KingE[64] = {	
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
	0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
	0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
	0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	
};

const int KingO[64] = {	
	0	,	5	,	5	,	-10	,	-10	,	0	,	10	,	5	,
	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,
	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70		
};
// sjeng 11.2
//8/6R1/2k5/6P1/8/8/4nP2/6K1 w - - 1 41 
int MaterialDraw(const S_BOARD *pos) {

	ASSERT(CheckBoard(pos));
	
    if (!pos->pceNum[wR] && !pos->pceNum[bR] && !pos->pceNum[wQ] && !pos->pceNum[bQ]) {
	  if (!pos->pceNum[bB] && !pos->pceNum[wB]) {
	      if (pos->pceNum[wN] < 3 && pos->pceNum[bN] < 3) {  return TRUE; }
	  } else if (!pos->pceNum[wN] && !pos->pceNum[bN]) {
	     if (abs(pos->pceNum[wB] - pos->pceNum[bB]) < 2) { return TRUE; }
	  } else if ((pos->pceNum[wN] < 3 && !pos->pceNum[wB]) || (pos->pceNum[wB] == 1 && !pos->pceNum[wN])) {
	    if ((pos->pceNum[bN] < 3 && !pos->pceNum[bB]) || (pos->pceNum[bB] == 1 && !pos->pceNum[bN]))  { return TRUE; }
	  }
	} else if (!pos->pceNum[wQ] && !pos->pceNum[bQ]) {
        if (pos->pceNum[wR] == 1 && pos->pceNum[bR] == 1) {
            if ((pos->pceNum[wN] + pos->pceNum[wB]) < 2 && (pos->pceNum[bN] + pos->pceNum[bB]) < 2)	{ return TRUE; }
        } else if (pos->pceNum[wR] == 1 && !pos->pceNum[bR]) {
            if ((pos->pceNum[wN] + pos->pceNum[wB] == 0) && (((pos->pceNum[bN] + pos->pceNum[bB]) == 1) || ((pos->pceNum[bN] + pos->pceNum[bB]) == 2))) { return TRUE; }
        } else if (pos->pceNum[bR] == 1 && !pos->pceNum[wR]) {
            if ((pos->pceNum[bN] + pos->pceNum[bB] == 0) && (((pos->pceNum[wN] + pos->pceNum[wB]) == 1) || ((pos->pceNum[wN] + pos->pceNum[wB]) == 2))) { return TRUE; }
        }
    }
  return FALSE;
}

#define ENDGAME_MAT (1 * PieceVal[wR] + 2 * PieceVal[wN] + 2 * PieceVal[wP] + PieceVal[wK])

int EvalPosition(const S_BOARD *pos) {
    //parallelize - parallel evaluation of different piece types and board aspects
    int pce;
    int pceNum;
    int sq;
    int score = pos->material[WHITE] - pos->material[BLACK];
	
	if(!pos->pceNum[wP] && !pos->pceNum[bP] && MaterialDraw(pos) == TRUE) {
		return 0;
	}
	#pragma acc parallel loop private(pceNum, sq) shared(IsolatedMask, WhitePassedMask, pos)
	for(pceNum = 0; pceNum < pos->pceNum[wP]; ++pceNum) {
		sq = pos->pList[wP][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		score += PawnTable[SQ64(sq)];
	}
	#pragma acc parallel loop private(pceNum, sq) shared(IsolatedMask, WhitePassedMask, pos)
	for(pceNum = 0; pceNum < pos->pceNum[bP]; ++pceNum) {
		sq = pos->pList[bP][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		score -= PawnTable[Mirror64[SQ64(sq)]];
	}
	
	pce = wN;	
	#pragma acc parallel loop private(pceNum, sq) shared(pos)
	for(pceNum = 0; pceNum < pos->pceNum[wN]; ++pceNum) {
		sq = pos->pList[wN][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		score += KnightTable[SQ64(sq)];
	}	

	pce = bN;	
	#pragma acc parallel loop private(pceNum, sq) shared(pos)
	for(pceNum = 0; pceNum < pos->pceNum[bN]; ++pceNum) {
		sq = pos->pList[bN][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		score -= KnightTable[Mirror64[SQ64(sq)]];
	}			
	
	pce = wB;	
	#pragma acc parallel loop private(pceNum, sq) shared(pos)
	for(pceNum = 0; pceNum < pos->pceNum[wB]; ++pceNum) {
		sq = pos->pList[wB][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		score += BishopTable[SQ64(sq)];
	}	

	pce = bB;	
	#pragma acc parallel loop private(pceNum, sq) shared(pos)
	for(pceNum = 0; pceNum < pos->pceNum[bB]; ++pceNum) {
		sq = pos->pList[bB][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		score -= BishopTable[Mirror64[SQ64(sq)]];
	}	

	pce = wR;	
	#pragma acc parallel loop private(pceNum, sq) shared(pos)
	for(pceNum = 0; pceNum < pos->pceNum[wR]; ++pceNum) {
		sq = pos->pList[wR][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		score += RookTable[SQ64(sq)];
		
		ASSERT(FileRankValid(FilesBrd[sq]));
		
		if(!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score += RookOpenFile;
		} else if(!(pos->pawns[WHITE] & FileBBMask[FilesBrd[sq]])) {
			score += RookSemiOpenFile;
		}
	}	

	pce = bR;	
	#pragma acc parallel loop private(pceNum, sq) shared(pos)
	for(pceNum = 0; pceNum < pos->pceNum[bR]; ++pceNum) {
		sq = pos->pList[bR][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		score -= RookTable[Mirror64[SQ64(sq)]];
		ASSERT(FileRankValid(FilesBrd[sq]));
		if(!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score -= RookOpenFile;
		} else if(!(pos->pawns[BLACK] & FileBBMask[FilesBrd[sq]])) {
			score -= RookSemiOpenFile;
		}
	}	
	
	pce = wQ;	
	#pragma acc parallel loop private(pceNum, sq) shared(pos)
	for(pceNum = 0; pceNum < pos->pceNum[wQ]; ++pceNum) {
		sq = pos->pList[wQ][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		ASSERT(FileRankValid(FilesBrd[sq]));
		if(!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score += QueenOpenFile;
		} else if(!(pos->pawns[WHITE] & FileBBMask[FilesBrd[sq]])) {
			score += QueenSemiOpenFile;
		}
	}	

	pce = bQ;	
	#pragma acc parallel loop private(pceNum, sq) shared(pos)
	for(pceNum = 0; pceNum < pos->pceNum[bQ]; ++pceNum) {
		sq = pos->pList[bQ][pceNum];
		ASSERT(sq>=A1 && sq<=H8);
		ASSERT(FileRankValid(FilesBrd[sq]));
		if(!(pos->pawns[BOTH] & FileBBMask[FilesBrd[sq]])) {
			score -= QueenOpenFile;
		} else if(!(pos->pawns[BLACK] & FileBBMask[FilesBrd[sq]])) {
			score -= QueenSemiOpenFile;
		}
	}	
	//8/p6k/6p1/5p2/P4K2/8/5pB1/8 b - - 2 62 
	pce = wK;
	int piece;
	#pragma acc parallel loop private(sq, piece) shared(pos)
	for(sq = 0; sq < BRD_SQ_NUM; ++sq) {
		piece = pos->pieces[sq];
		if(piece==wK) {
			score += KingE[SQ64(sq)];
		} else if(piece==bK) {
			score += KingO[SQ64(sq)];
		}
	}
	
	if(pos->pceNum[wB] >= 2) score += BishopPair;
	if(pos->pceNum[bB] >= 2) score -= BishopPair;
	
	if(pos->side == WHITE) {
		return score;
	} else {
		return -score;
	}	
}