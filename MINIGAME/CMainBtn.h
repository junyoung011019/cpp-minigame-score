#pragma once

class CMainBtn
{
	MENU_TYPE tType;
	bool bSelect;
public:
	void Render();

	void SetType(MENU_TYPE _type) { tType = _type; }
	MENU_TYPE GetType() { return tType; }

	void Select() { bSelect = true; }
	void UnSelect() { bSelect = false; }

	CMainBtn();
	~CMainBtn();
};

