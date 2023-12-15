#include "pch.h"

#include "CMain.h"

int main() {
    cursor();
    system("mode con: cols=100 lines=30 | title 미니게임천국");
    PlaySound(TEXT("main.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    // 마우스 클릭 및 드래그로 인한 오류 방지
    DWORD consoleModePrev;
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(handle, &consoleModePrev);
    SetConsoleMode(handle, consoleModePrev & ~ENABLE_QUICK_EDIT_MODE);

    CMain::GetInst()->Init();
    while (true) {
        CMain::GetInst()->Render();
        CMain::GetInst()->Update();
    }
}