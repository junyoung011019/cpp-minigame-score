#include "pch.h"

#include "img.h"
#include "function.h"

int TypingGame() {
    Sound("GameBGM");
    string str, aswstr; //랜덤단어, 입력단어
    int miss_count = 0; //틀린 갯수
    int match_count = 0; // 맞춘 갯수
    clock_t startTime = clock();
    clock_t S_score_t = clock();
    clock_t E_score_t;
    int end_score_time;
    int start_score_time = (int)(S_score_t / CLOCKS_PER_SEC);
    int st;
    while (true) {

        str = get_word(); //랜덤 단어 받아오기
        word_prn(str, miss_count, match_count); // 답 입력 화면

        aswstr = input_asw(startTime, str, miss_count);

        if (aswstr == str)
        {

            startTime = clock();
            gotoxy(15, 23);
            MATCH_IMG();
            Sound("Correct");
            match_count++;
            if (match_count > CLEAR_NUM - 1)
            {
                E_score_t = clock();
                end_score_time = (int)(E_score_t / CLOCKS_PER_SEC);
                Sound("Clear");
                system("cls");
                gotoxy(33, 3);
                GAMECLER_IMG();
                setColor(PINK);
                gotoxy(42, 18);
                int typing_score = end_score_time - start_score_time;
                Record(typing_score);
                gotoxy(42, 18);
                cout << "Score : " << typing_score << " 초"; //기록 출력
                setColor(WHITE);
                char c;
                while (true) {
                    if (_kbhit()) {
                        c = _getch();
                        if (c == ESC) {
                            return 0;
                        }
                    }
                }
            }
        }
        if (aswstr == "gameover")
        {
            Sound("Gameover");
            system("cls");
            gotoxy(33, 3);
            GAMEOVER_IMG();
            char c;
            while (true) {
                if (_kbhit()) {
                    c = _getch();
                    if (c == ESC) {
                        return 0;
                    }
                }
            }
        }
        if (aswstr != str)
        {
            gotoxy(15, 23);
            MISS_IMG();
            Sound("Wrong");
            miss_count++;
            if (miss_count > GAMEOVER_COUNT - 1)
            {
                Sound("Gameover");
                system("cls");
                gotoxy(33, 3);
                GAMEOVER_IMG();
                
                char c;
                while (true) {
                    if (_kbhit()) {
                        c = _getch();
                        if (c == ESC) {
                            return 0;
                        }
                    }
                }
            }
        }

        system("cls");
    }
}