#include "pch.h"

#include "CMain.h"

int main() {
    cursor();
    system("mode con: cols=100 lines=30 | title 미니게임천국");
    PlaySound(TEXT("main.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    CMain::GetInst()->Init();
    while (true) {
        CMain::GetInst()->Render();
        CMain::GetInst()->Update();
    }
}