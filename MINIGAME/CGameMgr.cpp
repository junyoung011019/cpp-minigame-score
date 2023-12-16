#include "pch.h"
#include "CGameMgr.h"
#include "CLand.h"

CGameMgr::CGameMgr()
	: sCurSelect(SPos(0, 0))
	, bStart(false)
	, iFlagCnt(0)
	, iOpenCnt(0)
	, tTimeStart(0)
	, tTimeScore(0)
	, bWin(false)
	, bQuit(false)
	, iHighScore(-1)
{
}

CGameMgr::~CGameMgr()
{
}

void CGameMgr::Init()
{
	PlaySound(TEXT("GameBGM.wav"), NULL, SND_ASYNC || SND_LOOP);
	// === ¸â¹ö ÃÊ±âÈ­ ===
	sCurSelect = SPos(0, 0);
	vLandArr.clear();
	vMinePosArr.clear();
	tTimeScore = 0;
	bWin = false;
	iFlagCnt = 0;
	iOpenCnt = 0;
	// ===================

	vector<CLand*> vSet;
	for (int i = 0; i < MS_ROW; i++) {
		vLandArr.push_back(vSet);
		for (int j = 0; j < MS_COL; j++) {
			CLand* pLandObj = new CLand;
			pLandObj->SetPos(i, j);
			vLandArr[i].push_back(pLandObj);
		}
	}
	vLandArr[sCurSelect.x][sCurSelect.y]->Select();
}

void CGameMgr::Start()
{
	for (int i = 0; i < MS_ROW; i++) {
		for (int j = 0; j < MS_COL; j++) {
			vLandArr[i][j]->Init();
		}
	}
	srand((unsigned)time(NULL));
	while (vMinePosArr.size() < MINE_NUM) {
		SPos sMinePos = SPos(rand() % MS_ROW, rand() % MS_COL);
		if (find(vMinePosArr.begin(), vMinePosArr.end(), sMinePos) != vMinePosArr.end() || sMinePos == sCurSelect || sMinePos.IsStartArea(sCurSelect)) continue;
		vMinePosArr.push_back(sMinePos);
	}
	for (int i = 0; i < vMinePosArr.size(); i++) {
		SPos sMinePos = vMinePosArr[i];
		vLandArr[sMinePos.x][sMinePos.y]->SetMine();
	}
	for (int i = 0; i < MS_ROW; i++) {
		for (int j = 0; j < MS_COL; j++) {
			vLandArr[i][j]->CheckNearMine();
		}
	}
}

void CGameMgr::Update()
{
	char cInput;
	static bool bExcpt = false;
	CLand* curLndObj = vLandArr[sCurSelect.x][sCurSelect.y];
	while (true)
	{
		iOpenCnt = 0;
		for (int i = 0; i < vLandArr.size(); i++) {
			for (int j = 0; j < vLandArr[i].size(); j++) {
				vLandArr[i][j]->Update();
				if (!vLandArr[i][j]->IsMine()) {
					if (vLandArr[i][j]->IsOpen()) iOpenCnt++;
					if (!bStart) break;
				}
			}
			if (!bStart) {
				curLndObj = vLandArr[sCurSelect.x][sCurSelect.y];
				break;
			}
		}
		if ((MS_ROW * MS_COL - MINE_NUM) == iOpenCnt) {
			bWin = true;
			GameOver();
			curLndObj = vLandArr[sCurSelect.x][sCurSelect.y];
		}
		curLndObj = vLandArr[sCurSelect.x][sCurSelect.y];
		if (_kbhit()) {
			cInput = _getch();
			if (cInput == -32) {
				cInput = _getch();
				switch (cInput) {
				case LEFT:
					if (sCurSelect.x != 0)
						sCurSelect.x--;
					break;
				case RIGHT:
					if (sCurSelect.x < (MS_ROW - 1))
						sCurSelect.x++;
					break;
				case UP:
					if (sCurSelect.y != 0)
						sCurSelect.y--;
					break;
				case DOWN:
					if (sCurSelect.y < (MS_COL - 1))
						sCurSelect.y++;
					break;
				default:
					bExcpt = true;
				}
				if (!bExcpt) {
					curLndObj->UnSelect();
					vLandArr[sCurSelect.x][sCurSelect.y]->Select();
				}
				bExcpt = false;
				break;
			}
			else if (cInput == SPACE) {
				if (!curLndObj->IsFlag() && !bStart) {
					bStart = true;
					Start();
					tTimeStart = clock();
				}
				if (!curLndObj->IsFlag()) {
					curLndObj->Open();
					if (!curLndObj->IsMine()) {
					}
				}
				break;
			}
			else if (cInput == TAB) {
				if (!curLndObj->IsOpen()) {
					curLndObj->Flagging();
					if (curLndObj->IsFlag()) iFlagCnt++;
					else iFlagCnt--;
					break;
				}
			}
			else if (cInput == ESC) {
				bQuit = true;
			}
		}
		if ((int)tTimeScore != (int)((clock() - tTimeStart) / CLOCKS_PER_SEC) && bStart) {
			tTimeScore = (clock() - tTimeStart) / CLOCKS_PER_SEC;
			break;
		}
		if (bQuit) break;
	}
}

void CGameMgr::Render()
{
	// Title: MINE SWEEPER
	setColor(COLOR::BLUE);
	gotoxy(RESOLUTION.x / 2 - 36, 0);
	cout << " __  __  ____  _  _  ____    ___  _    _  ____  ____  ____  ____  ____ ";
	gotoxy(RESOLUTION.x / 2 - 36, 1);
	cout << "(  \\/  )(_  _)( \\( )( ___)  / __)( \\/\\/ )( ___)( ___)(  _ \\( ___)(  _ \\";
	gotoxy(RESOLUTION.x / 2 - 36, 2);
	cout << " )    (  _)(_  )  (  )__)   \\__ \\ )    (  )__)  )__)  )___/ )__)  )   /";
	gotoxy(RESOLUTION.x / 2 - 36, 3);
	cout << "(_/\\/\\_)(____)(_)\\_)(____)  (___/(__/\\__)(____)(____)(__)  (____)(_)\\_)";
	setColor(COLOR::WHITE);

	// TIME | MINE LEFT
	gotoxy((RESOLUTION.x / 2) - 12, 5);
	cout << "TIME : " << tTimeScore << " | " << "MINE LEFT :";
	if (MINE_NUM - iFlagCnt <= 99 && MINE_NUM - iFlagCnt >= 10 || MINE_NUM - iFlagCnt <= -1 && MINE_NUM - iFlagCnt >= -9)
		cout << " ";
	else if (MINE_NUM - iFlagCnt <= 9 && MINE_NUM - iFlagCnt >= 0)
		cout << "  ";
	cout << MINE_NUM - iFlagCnt;

	// BOARD
	for (int i = 0; i < vLandArr.size(); i++) {
		for (int j = 0; j < vLandArr[i].size(); j++) {
			vLandArr[i][j]->Render();
		}
		cout << endl;
	}

	// HIGH SCORE
	gotoxy((RESOLUTION.x / 2) - 6, MS_COL + 9);
	cout << "HIGH SCORE : ";
	if (iHighScore == -1) cout << "--";
	else cout << iHighScore << endl;
}

void CGameMgr::GameOver()
{
	char cInput;
	bStart = false;
	if (bWin) {
		PlaySound(TEXT("Gameclear.wav"), NULL, SND_ASYNC);
		if (iHighScore == -1 || iHighScore > tTimeScore)
			iHighScore = tTimeScore;
		Record(tTimeScore);
		Render();
		gotoxy((RESOLUTION.x / 2) - 4, MS_COL + 7);
		setColor(COLOR::BLUE);
		cout << "You Win!";
	}
	else {
		PlaySound(TEXT("Gameover.wav"), NULL, SND_ASYNC);
		for (int i = 0; i < MS_ROW; i++) {
			for (int j = 0; j < MS_COL; j++) {
				if (vLandArr[i][j]->IsMine())
					vLandArr[i][j]->Open();
			}
		}
		Render();
		gotoxy((RESOLUTION.x / 2) - 5, MS_COL + 7);
		setColor(COLOR::RED);
		cout << "You Lose...";
	}
	setColor(COLOR::WHITE);
	gotoxy((RESOLUTION.x / 2) - 19, MS_COL + 11);
	cout << "Press Enter to Restart OR ESC to Exit";
	while (true) {
		if (_kbhit()) {
			cInput = _getch();
			if (cInput == ENTER) {
				system("cls");
				for (int i = 0; i < MS_ROW; i++) {
					for (int j = 0; j < MS_COL; j++) {
						delete vLandArr[i][j];
					}
				}
				Init();
				Render();
				break;
			}
			else if (cInput == ESC) {
				bQuit = true;
				break;
			}
		}
	}
}

bool CGameMgr::Quit()
{
	if (bQuit) {
		system("cls");
		for (int i = 0; i < MS_ROW; i++) {
			for (int j = 0; j < MS_COL; j++) {
				delete vLandArr[i][j];
			}
		}
		bStart = false;
		bQuit = false;
		return true;
	}
	else return false;
}
