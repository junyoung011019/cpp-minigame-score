#pragma once

class CMainBtn;

class CMain
{
	vector<CMainBtn*> vMainBtnArr;
	int iSelect;
	MENU_TYPE tCurMenu;
public:
	void Init();
	void Update();
	void Render();


// ===== ΩÃ±€≈Ê =====
public:
	static CMain* GetInst()
	{
		static CMain mgr;
		return &mgr;
	}

private:
	CMain();
	~CMain();
// ==================
};

