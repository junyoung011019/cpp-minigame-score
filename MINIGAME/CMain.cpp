#include "pch.h"
#include "CMain.h"
#include "CMainBtn.h"

CMain::CMain()
    : iSelect(0)
    , tCurMenu(MENU_TYPE::END)
    , sCurId(" ")
    , bMsg(false)
{
}

CMain::~CMain()
{
}

void CMain::Init()
{
    system("cls");
    for (int i = 0; i < vMainBtnArr.size(); i++) {
        delete vMainBtnArr[i];
    }
    vMainBtnArr.clear();

    switch (tCurMenu)
    {
    case MENU_TYPE::PLAY:
        for (int i = (int)MENU_TYPE::MINESWEEPER; i <= (int)MENU_TYPE::BACK; i++) {
            CMainBtn* pMainBtnObj = new CMainBtn;
            pMainBtnObj->SetType(static_cast<MENU_TYPE>(i));
            vMainBtnArr.push_back(pMainBtnObj);
        }
        break;
    case MENU_TYPE::LOGIN: //완료
        LOGIN();
        tCurMenu = MENU_TYPE::END;
        Init();
        break;
    case MENU_TYPE::SIGNIN: //완료
        SIGN_UP();
        tCurMenu = MENU_TYPE::END;
        Init();
        break;
    case MENU_TYPE::SCORE:
        Check_Score();
        tCurMenu = MENU_TYPE::END;
        Init();
        break;
    case MENU_TYPE::DEV:
        devsINFO();
        tCurMenu = MENU_TYPE::END;
        Init();
        break;
    case MENU_TYPE::QUIT: //완료
        END();
        exit(0);
        break;
    case MENU_TYPE::MINESWEEPER:
        MineSweeper();
        tCurMenu = MENU_TYPE::PLAY;
        Init();
        PlaySound(TEXT("main.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        break;
    case MENU_TYPE::TYPINGGAME:
        TypingGame();
        tCurMenu = MENU_TYPE::PLAY;
        Init();
        PlaySound(TEXT("main.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        break;
    default:
        for (int i = 0; i <= (int)MENU_TYPE::QUIT; i++) {
            CMainBtn* pMainBtnObj = new CMainBtn;
            pMainBtnObj->SetType(static_cast<MENU_TYPE>(i));
            vMainBtnArr.push_back(pMainBtnObj);
        }
        break;
    }

    // 메뉴 작업 미완료로 인해 발생하는 예외 처리입니다. 추후 개발이 완료되면 수정할 것을 권장합니다.
    try {
        if (vMainBtnArr.size() == 0) throw 0;
        vMainBtnArr[iSelect]->Select();
    }
    catch(int _) {
        tCurMenu = MENU_TYPE::END;
        Init();
    }
}

void CMain::Update()
{
    char cInput;
    while (true) {
        static time_t tMsgTimer = 0;
        if ((clock() - tMsgTimer) / CLOCKS_PER_SEC > 0.5 && bMsg) {
            tMsgTimer = 0;
            bMsg = false;
            break;
        }
        if (_kbhit()) {
            cInput = _getch();
            if (cInput == -32) {
                cInput = _getch();
                switch (cInput) {
                case RIGHT:
                case DOWN:
                    vMainBtnArr[iSelect]->UnSelect();
                    if (iSelect >= vMainBtnArr.size() - 1)
                        iSelect = 0;
                    else iSelect++;
                    vMainBtnArr[iSelect]->Select();
                    break;
                case LEFT:
                case UP:
                    vMainBtnArr[iSelect]->UnSelect();
                    if (iSelect <= 0)
                        iSelect = vMainBtnArr.size() - 1;
                    else iSelect--;
                    vMainBtnArr[iSelect]->Select();
                    break;
                }
                break;
            }
            else if (cInput == SPACE || cInput == ENTER) {
                if (vMainBtnArr[iSelect]->GetType() == MENU_TYPE::BACK) {
                    tCurMenu = MENU_TYPE::END;
                }
                else if ((vMainBtnArr[iSelect]->GetType() == MENU_TYPE::PLAY || vMainBtnArr[iSelect]->GetType() == MENU_TYPE::SCORE)&& sCurId == " ") {
                    bMsg = true;
                    tMsgTimer = clock();
                }
                else {
                    tCurMenu = vMainBtnArr[iSelect]->GetType();
                    bMsg = false;
                    tMsgTimer = 0;
                }
                Init();
                break;
            }
        }
    }
}

void CMain::Render()
{
    // 로그인, 회원가입, 게임 스코어, 개발자 등의 출력 화면 작업 시
    // 이 함수 안에서 Switch 문을 사용하는 것을 권장합니다
    system("cls");
    gotoxy(0, 0);
    cout << "\n\n\n";
    cout << "            ##   ##   ####    ##   #    ####        ####      ##     ##   ##  ######\n";
    cout << "            # # # #    ##     # #  #     ##        ##  ##    ####    # # # #  #     \n";
    cout << "            # ##  #    ##     # ## #     ##        ##       ##  ##   # # # #  #     \n";
    cout << "            #  #  #    ##     #  # #     ##        ## ###   ######   #  #  #  ####  \n";
    cout << "            #     #    ##     #  # #     ##        ##  ##   ##  ##   #     #  #     \n";
    cout << "            #     #    ##     #   ##     ##        ##  ##   ##  ##   #     #  #     \n";
    cout << "            #     #   ####    #   ##    ####        ####    ##  ##   #     #  ######\n\n\n";
    cout << "                                     2023 NSU COMPUTER SOFTWARE\n";
    cout << "                                              REVERSE\n\n";
    cout << "                                              VER 1.0\n\n\n";
    gotoxy(0, 18);
    if (bMsg) cout << "                                       로그인이 필요합니다!                       ";
    gotoxy(0, 21);
    for (int i = 0; i < vMainBtnArr.size(); i++) {
        gotoxy(RESOLUTION.x / 2 - 8, i + 20);
        vMainBtnArr[i]->Render();
    }
}

