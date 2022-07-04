#pragma once
#include "stdfix.h"

typedef struct WD {
	char16_t vBuf[ROWS][COLS]{ 0 };
	int grid[ROWS][COLS]{ 0 };
} wd;

enum Colors {
	White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	Red = FOREGROUND_RED,
	Green = FOREGROUND_GREEN,
	Blue = FOREGROUND_BLUE,
	Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	Purple = FOREGROUND_RED | FOREGROUND_BLUE,
	Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
	BrRed = FOREGROUND_RED | FOREGROUND_INTENSITY,
	BrGreen = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	BrBlue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	BrCyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	BrPurple = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	BrYellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
};