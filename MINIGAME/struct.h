#pragma once
#include "pch.h"

struct SPos
{
	int x;
	int y;

public:
	SPos()
		: x(0)
		, y(0)
	{}

	SPos(int _x, int _y)
		: x(_x)
		, y(_y)
	{}

	bool IsStartArea(SPos _sOther) {
		int ix, iy;

		for (int i = 0; i < 8; i++) {
			ix = (i < 3) ? 1 : ((i == 3 || i == 7) ? 0 : -1);
			iy = (i >= 2 && i <= 4) ? 1 : ((i == 1 || i == 5) ? 0 : -1);

			if (_sOther.x + ix >= 0 && _sOther.x + ix < MS_ROW && _sOther.y + iy >= 0 && _sOther.y + iy < MS_COL) {
				if (x == _sOther.x + ix && y == _sOther.y + iy)
					return true;
			}
		}
		return false;
	}

	// Operator
	bool operator == (SPos _sOther) { return (x == _sOther.x && y == _sOther.y); }
};