#pragma once

class CLand
{
	bool bMine;
	int iNearMineNum;
	bool bOpen;
	bool bSelect;
	bool bFlag;
	int iNearFlagNum;
	SPos sPos;
public:
	void SetPos(int _x, int _y) { sPos = SPos(_x, _y); }

	void SetMine() { bMine = true; }
	bool IsMine() { return bMine; }

	void SetNMN(int _nmn) { iNearMineNum = _nmn; }
	int GetNMN() { return iNearMineNum; }
	void CheckNearMine();

	void Open();
	bool IsOpen() { return bOpen; }

	void Select() { bSelect = true; }
	void UnSelect() { bSelect = false; }
	bool IsSelect() { return bSelect; }

	void Flagging() { bFlag = !bFlag; }
	bool IsFlag() { return bFlag; }

	void SetNFN(int _nfn) { iNearFlagNum = _nfn; }
	int GetNFN() { return iNearFlagNum; }
	void CheckNearFlag();

	void Init();
	void Update();
	void Render();

	CLand();
	~CLand();
};