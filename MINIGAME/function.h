#pragma once
#include "pch.h"
#include "img.h"

int WORD_NUM = 64; // text - 1파일 단어 갯수
int GAMEOVER_COUNT = 5; // 게임오버 횟수
int CLEAR_NUM = 10; // 클리어 조건 횟수
const int timeLimitSeconds = 7; // 제한시간

//랜덤 단어 받아오기
string get_word() {
    string str;
    ifstream inword("word_list.txt");

    if (!inword) {
        cout << "파일 열기 불가";
        exit(-1);
    }

    srand(time(NULL));

    int rannum = rand() % WORD_NUM;
    int curnum = 0;


    while (getline(inword, str)) {
        if (rannum == curnum) {
            break;
        }
        curnum++;
    }
    return str;
}

//단어 출력
void word_prn(string str, int miss_count, int clear_count) {
    GAMEMAIN_IMG();
    gotoxy(34, 11); line();
    gotoxy(34, 13); cout << "     HP     : ";

    setColor(RED);
    for (int a = GAMEOVER_COUNT - miss_count; a > 0; a--) cout << heart; //목숨 GAMEOVER_COUNT로 설정 후 틀리면 목숨에 -1하고 현재 가지고 있는 목숨 출력
    for (int b = miss_count; b > 0; b--) cout << loseheart;
    setColor(WHITE);
    cout << endl;

    gotoxy(34, 14); line();
    gotoxy(34, 16); cout << " 맞춘 갯수  : ";
    setColor(YELLOW);
    for (int b = clear_count; b > 0; b--) cout << box;
    for (int a = CLEAR_NUM - clear_count; a > 0; a--) cout << empty_box;
    setColor(WHITE);
    cout << endl;
    gotoxy(34, 17); line();



    gotoxy(46, 19); cout << str << endl;
    gotoxy(34, 20); line();

    gotoxy(46, 23);
    setColor(GREEN);
    for (int i = 0; i < str.length(); i++) cout << "_";
    for (int i = 0; i < str.length(); i++) cout << "\b";
    setColor(WHITE);
}

// 사용자 단어 입력
string input_asw(clock_t st, string str,  int miss_count) { //사용자 단어입력
    string gameover = "gameover";
    int key = 0;
    string curstr, asw;
    while (true)
    {   
        clock_t check_time = clock();
        
        int time = (check_time - st) / CLOCKS_PER_SEC; // 진행된 시간
        
        
        if ((time) < timeLimitSeconds) {
            gotoxy(35, 10);
            cout << " 남은시간   : ";
            setColor(BLUE);
            for (int a = timeLimitSeconds - time; a > 0; a--) cout << box;
            for (int b = time; b > 0; b--) cout << empty_box;
            setColor(WHITE);
        }
        if (time >= timeLimitSeconds)
        {
            return gameover;
        }
        if (_kbhit() != 0)
        {
            key = _getch();    //키 입력 받는 함수
            if (key == ENTER)
            {
                asw = curstr;
                return curstr;
            }
            else if (key == BACKSPACE)
            {
                if (curstr.size() != 0)
                {
                    setColor(GREEN);
                    gotoxy(46 + curstr.length(), 23);
                    cout << "\b";
                    cout << "_";
                    setColor(WHITE);
                    curstr.pop_back();
                   
                    gotoxy(46, 23);
                    
                    cout << curstr;
                }
            }
            else
            {
                if (curstr.length() < str.length())
                {
                    curstr += key;
                    gotoxy(46, 23);
                    cout << curstr;
                }
            }
        }
    }
}

class Sound
{
public:
    Sound(string input)
    {
        if (input == "GameBGM")      //인게임 BGM
            PlaySound(TEXT("GameBGM.wav"), NULL, SND_ASYNC || SND_LOOP);

        if (input == "Clear")       //게임 클리어 시 나오는 효과음
            PlaySound(TEXT("Gameclear.wav"), NULL, SND_ASYNC);

        if (input == "Gameover")    //게임 오버 시 나오는  효과음
            PlaySound(TEXT("Gameover.wav"), NULL, SND_ASYNC);

        if (input == "Correct")
        {
            Beep(260, 100);
            Beep(330, 100);
            Beep(380, 100);
        }
        if (input == "Wrong")
        {
            Beep(330, 100);
            Beep(330, 100);
        }
        if (input == "Stop")        //BGM을 멈추는 명령어
            PlaySound(NULL, 0, 0);
    }
};