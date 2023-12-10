#include "pch.h"
#include "CGameMgr.h"

void setColor(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void cursor() { //커서 안보이게
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCusor;
	ConsoleCusor.bVisible = false;
	ConsoleCusor.dwSize = 1;
	SetConsoleCursorInfo(consoleHandle, &ConsoleCusor);
}

void MineSweeper() {
	CGameMgr::GetInst()->Init();
	while (true)
	{
		CGameMgr::GetInst()->Render();
		CGameMgr::GetInst()->Update();
		if (CGameMgr::GetInst()->Quit()) break;
	}
}