#include "pch.h"
#include "CMainBtn.h"


CMainBtn::CMainBtn()
	: bSelect(false)
	, tType(MENU_TYPE::END)
{
}

CMainBtn::~CMainBtn()
{
}

void CMainBtn::Render()
{
	if (bSelect)
		setColor(COLOR::BLUE);
	switch (tType)
	{
	case MENU_TYPE::PLAY:
		cout << "[ 게임 플레이 ]";
		break;
	case MENU_TYPE::LOGIN:
		cout << "[    로그인   ]";
		break;
	case MENU_TYPE::SIGNIN:
		cout << "[   회원가입  ]";
		break;
	case MENU_TYPE::SCORE:
		cout << "[ 게임 스코어 ]";
		break;
	case MENU_TYPE::DEV:
		cout << "[    개발자   ]";
		break;
	case MENU_TYPE::QUIT:
		cout << "[    종 료    ]";
		break;
	case MENU_TYPE::MINESWEEPER:
		cout << "[   지뢰찾기  ]";
		break;
	case MENU_TYPE::TYPINGGAME:
		cout << "[   타자연습  ]";
		break;
	case MENU_TYPE::BACK:
		cout << "[   뒤로가기  ]";
		break;
	default:
		break;
	}
	setColor(COLOR::WHITE);
}