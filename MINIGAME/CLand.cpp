#include "pch.h"

#include "CLand.h"
#include "CGameMgr.h"

CLand::CLand()
	: bMine(false)
	, iNearMineNum(0)
	, bOpen(false)
	, bSelect(false)
	, bFlag(false)
	, iNearFlagNum(0)
{
}

CLand::~CLand()
{
}

void CLand::Open()
{
	if (IsMine()) {
		bOpen = true;
	}
	else if (IsOpen() && GetNMN() - GetNFN() == 0) {
		int ix, iy;

		for (int i = 0; i < 8; i++) {
			ix = (i < 3) ? 1 : ((i == 3 || i == 7) ? 0 : -1);
			iy = (i >= 2 && i <= 4) ? 1 : ((i == 1 || i == 5) ? 0 : -1);

			if (sPos.x + ix >= 0 && sPos.x + ix < MS_ROW && sPos.y + iy >= 0 && sPos.y + iy < MS_COL) {
				if (!CGameMgr::GetInst()->GetLandInfo(sPos.x + ix, sPos.y + iy)->IsOpen() && !CGameMgr::GetInst()->GetLandInfo(sPos.x + ix, sPos.y + iy)->IsFlag()) {
					CGameMgr::GetInst()->GetLandInfo(sPos.x + ix, sPos.y + iy)->Open();
				}
			}
		}
	}
	else if (!IsOpen()) {
		bOpen = true;
		if (GetNMN() == 0) {
			int ix, iy;

			for (int i = 0; i < 8; i++) {
				ix = (i < 3) ? 1 : ((i == 3 || i == 7) ? 0 : -1);
				iy = (i >= 2 && i <= 4) ? 1 : ((i == 1 || i == 5) ? 0 : -1);

				if (sPos.x + ix >= 0 && sPos.x + ix < MS_ROW && sPos.y + iy >= 0 && sPos.y + iy < MS_COL) {
					if (!CGameMgr::GetInst()->GetLandInfo(sPos.x + ix, sPos.y + iy)->IsOpen() && !CGameMgr::GetInst()->GetLandInfo(sPos.x + ix, sPos.y + iy)->IsFlag()) {
						CGameMgr::GetInst()->GetLandInfo(sPos.x + ix, sPos.y + iy)->Open();
					}
				}
			}
		}
	}
}

void CLand::CheckNearMine()
{
	int ix, iy;
	int nCnt = 0;
	for (int i = 0; i < 8; i++) {
		ix = (i < 3) ? 1 : ((i == 3 || i == 7) ? 0 : -1);
		iy = (i >= 2 && i <= 4) ? 1 : ((i == 1 || i == 5) ? 0 : -1);

		if (sPos.x + ix >= 0 && sPos.x + ix < MS_ROW && sPos.y + iy >= 0 && sPos.y + iy < MS_COL) {
			if (CGameMgr::GetInst()->GetLandInfo(sPos.x + ix, sPos.y + iy)->IsMine()) {
				nCnt += 1;
			}
		}
	}
	SetNMN(nCnt);
}

void CLand::CheckNearFlag()
{
	int ix, iy;
	int nCnt = 0;
	for (int i = 0; i < 8; i++) {
		ix = (i < 3) ? 1 : ((i == 3 || i == 7) ? 0 : -1);
		iy = (i >= 2 && i <= 4) ? 1 : ((i == 1 || i == 5) ? 0 : -1);

		if (sPos.x + ix >= 0 && sPos.x + ix < MS_ROW && sPos.y + iy >= 0 && sPos.y + iy < MS_COL) {
			if (CGameMgr::GetInst()->GetLandInfo(sPos.x + ix, sPos.y + iy)->IsFlag()) {
				nCnt += 1;
			}
		}
	}
	SetNFN(nCnt);
}

void CLand::Init()
{
	bMine = false;
	iNearMineNum = 0;
	bOpen = false;
	bFlag = false;
	iNearFlagNum = 0;
}

void CLand::Update()
{
	CheckNearFlag();
	if (IsMine() && IsOpen()) {
		CGameMgr::GetInst()->GameOver();
	}
}

void CLand::Render()
{
	gotoxy((RESOLUTION.x / 2) + (sPos.x - (MS_ROW / 2)) * 3, sPos.y + 7);
	if (IsSelect() || (IsMine() && IsOpen()))
		setColor(COLOR::RED);
	else if (IsFlag())
		setColor(COLOR::YELLOW);
	else if (IsOpen())
		setColor(COLOR::GREEN);
	cout << "[";
	if (IsOpen() && IsMine())
		cout << "*";
	else if (IsOpen())
	{
		if (GetNMN() == 0) cout << " ";
		else cout << GetNMN();
	}
	else if (IsFlag())
		cout << "!";
	else
		cout << " ";
	cout << "]";
	setColor(COLOR::WHITE);
}
