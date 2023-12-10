#pragma once

class CLand;

class CGameMgr
{
	vector<vector<CLand*>> vLandArr;
	vector<SPos> vMinePosArr;
	SPos sCurSelect;
	bool bStart;
	int iFlagCnt;
	int iOpenCnt;
	int iHighScore;
	bool bWin;
	bool bQuit;
	clock_t tTimeStart;
	clock_t tTimeScore;

	void Start();

public:
	void Init();
	void Update();
	void Render();
	void GameOver();
	bool Quit();

	CLand* GetLandInfo(int _x, int _y) { return vLandArr[_x][_y]; }

	// ===== ΩÃ±€≈Ê =====
public:
	static CGameMgr* GetInst()
	{
		static CGameMgr mgr;
		return &mgr;
	}

private:
	CGameMgr();
	~CGameMgr();
	// ==================
};