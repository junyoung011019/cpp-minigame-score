#pragma once
#include "pch.h"

// === #define ===

#define RESOLUTION SPos(100, 30)

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define TAB 9
#define ENTER 13
#define ESC 27
#define BACKSPACE 8

// MineSweeper
#define MS_ROW 16
#define MS_COL 16
#define MINE_NUM 40


// === Function ===

void gotoxy(int x, int y);


// === ETC ===

enum class MENU_TYPE {
	PLAY,
	LOGIN,
	SIGNIN,
	SCORE,
	DEV,
	QUIT,
	MINESWEEPER,
	TYPINGGAME,
	BACK,
	END
};

enum COLOR {
	BLACK = 0,
	D_BLUE = 1,
	D_GREEN = 2,
	D_RED = 4,
	BLUE = 9,
	GREEN = 10,
	MINT = 11,
	RED = 12,
	PINK = 13,
	YELLOW = 14,
	WHITE = 15
};

