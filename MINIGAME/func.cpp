#include "pch.h"
#include "CGameMgr.h"
#include "CMain.h"

MYSQL Conn;                              // MySQL 정보 담을 구조체
MYSQL* ConnPtr = NULL;                  // MySQL 핸들
MYSQL_RES* Result;                      // 쿼리 성공시 결과를 담는 구조체 포인터
MYSQL_ROW Row;                  // 쿼리 성공시 결과로 나온 행의 정보를 담는 구조체



void setColor(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void cursor() { //커서 안보이게
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCusor;
	ConsoleCusor.bVisible = false;
	ConsoleCusor.dwSize = 1;
	SetConsoleCursorInfo(consoleHandle, &ConsoleCusor);
}

void MineSweeper() {
	CGameMgr::GetInst()->Init();
	while (true)
	{
		CGameMgr::GetInst()->Render();
		CGameMgr::GetInst()->Update();
		if (CGameMgr::GetInst()->Quit()) break;
	}
}

void SIGN_UP()
{

	///////////////////////////SQL 연동 확인///////////////////////////

	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, "localhost", "root", "1q2w3e4r!", "MINIGAME", 3306, NULL, 0)) {
		cout << "error\n";
	}
	else {
		cout << "success\n";
	}

	mysql_init(&Conn); // MySQL 정보 초기화  

	///////////////////////////SQL 쿼리 연동///////////////////////////

	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "1q2w3e4r!", "MINIGAME", 3306, (char*)NULL, 0);

	// 연결 결과 확인. null일 경우 실패
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
		return ;
	}

	///////////////////////////회원가입///////////////////////////

	string UserName, UserId, UserPw;


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// 하늘 색상 출력
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	std::cout << "\n\n\n";
	cout << "                ## ##     ####    ## ##   ###  ##           ##  ###  ### ##   \n";
	cout << "               ##   ##     ##    ##   ##    ## ##           ##   ##   ##  ##  \n";
	cout << "               ####        ##    ##        # ## #           ##   ##   ##  ##  \n";
	cout << "                #####      ##    ##  ###   ## ##            ##   ##   ##  ##  \n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "                   ###     ##    ##   ##   ##  ##           ##   ##   ## ##   \n";
	cout << "               ##   ##     ##    ##   ##   ##  ##           ##   ##   ##      \n";
	cout << "                ## ##     ####    ## ##   ###  ##            ## ##   ####     \n";
	cout << "\n\n\n\n";
	// 흰색으로 설정하여 다음 출력이 흰색으로 나타나도록 함
	SetConsoleTextAttribute(hConsole, 15);


	cout << "이름을 입력해주세요 >> ";
	cin >> UserName;
	while (1) {
		cout << "아이디를 입력해주세요 >> ";
		cin >> UserId;
		////아이디 중복 확인

		// ID 중복 체크를 위한 SQL 쿼리 생성
		string idCheckQuery = "SELECT ID FROM customer_table WHERE ID = '" + UserId + "'";

		// SQL 쿼리 실행
		mysql_query(&Conn, idCheckQuery.c_str());

		// 결과를 저장
		MYSQL_RES* idCheckResult = mysql_store_result(&Conn);

		// 결과 행의 개수를 확인하여 중복 여부를 판단
		if (mysql_num_rows(idCheckResult) == 0) {
			// 결과 세트 해제
			mysql_free_result(idCheckResult);

			// 중복이 없으면 반복문 종료
			break;
		}

		cout << "이미 존재하는 아이디 입니다!\n";
		mysql_free_result(idCheckResult);
	}

	cout << "비밀번호를 입력해주세요.";
	cin >> UserPw;

	cout << "가입을 진심으로 환영합니다!!!\n";
	Sleep(3000);

	//가입 인원 판별 및 No값 삽입
	int No = 0; // 앞으로 회원이 등록되면 NO(회원번호)로 회원을 파악


	// 고객 테이블의 레코드 개수를 가져오기 위한 SQL 쿼리 생성
	string AccountCountQuery = "SELECT COUNT(*) FROM customer_table";
	//고객 테이블에 있는 모든 칼럼을 검색해주세요

	// SQL 쿼리 실행
	if (mysql_query(&Conn, AccountCountQuery.c_str()) != 0) {
		// SQL 쿼리 실행 중 오류 발생 시 오류 메시지 출력
		fprintf(stderr, "SQL 문 실행 오류: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL 쿼리 실행이 성공한 경우 결과를 가져오기
		MYSQL_RES* result = mysql_use_result(&Conn);

		// 결과 행 가져오기
		MYSQL_ROW row = mysql_fetch_row(result);

		// 결과 행이 존재하는 경우
		if (row != NULL) {
			// 레코드 개수를 정수로 변환하여 No 변수에 저장
			No = atoi(row[0]); //atoi()통해 정수로 반환

			// 결과 세트 해제
			mysql_free_result(result);
		}
	}
	//가입 인원 +1 =No   ex) 고객테이블에 5명의 고객이 존재하면 다음 회원부터는 NO.6 지정
	No += 1;

	// 회원 정보를 데이터베이스에 삽입하기 위한 SQL 쿼리 생성 및 실행
	string insertQuery = "INSERT INTO `MINIGAME`.`customer_table` (`No`, `ID`, `Name`, `Password`) VALUES ('" + to_string(No) + "','" + UserId + "', '" + UserName + "', '" + UserPw + "')";

	if (mysql_query(&Conn, insertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}
}

void LOGIN()
{
	string InputId, InputPw;
	int missing = 0;
	string MemberNo = "NULL";


	mysql_init(&Conn); // MySQL 정보 초기화

	// 데이터베이스와 연결
	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "1q2w3e4r!", "MINIGAME", 3306, (char*)NULL, 0);
	// MySql DB와 연동하기 위한 문
	//  MYSQL 구조체에 대한 포인터로 즉, 연결 성공 시 구조체가 초기화 호스트명 / 사용자 이름 / 비밀번호 / 스키마 / 포트번호 / 포인터와 숫자 0 모두 C++에서 NULL로 해석

	// 연결 결과 확인. null일 경우 실패
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
		return;
	}

	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//하늘 색상 출력
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	cout << "\n\n\n";
	cout << "                            ##        ##        ## ##    ####   ###   ##\n";
	cout << "                            ##      ##   ##   ##   ##     ##      ##  ##\n";
	cout << "                            ##      ##   ##   ##          ##     # ## ##\n";
	cout << "                            ##      ##   ##   ##  ###     ##     ## ##\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "                            ##      ##   ##   ##   ##     ##     ##  ##\n";
	cout << "                            ##       ## ##     ##   ##    ##     ##  ##\n";
	cout << "                            ### ###   ##        ## ##    ####   ###  ##\n";

	SetConsoleTextAttribute(hConsole, 15);
	cout << "\n\n\n\n";


	while (1)
	{
		cout << "\n";
		cout << "아이디를 입력하세요 >> ";
		cin >> InputId;
		cout << "비밀번호를 입력하세요 >> ";
		cin >> InputPw;

		// SQL 쿼리 - customer_table에서 ID에 있는 모든 칼럼을 보여주세요
		string query = "SELECT * FROM customer_table WHERE ID = '" + InputId + "' AND password = '" + InputPw + "'";
		// SQL 쿼리를 실행하고 성공 여부 확인
		if (mysql_query(&Conn, query.c_str()) == 0) {
			// 결과를 저장
			MYSQL_RES* result = mysql_store_result(&Conn);

			// 결과가 있을 경우
			if (result) {
				// 첫 번째 행을 가져옴
				MYSQL_ROW row = mysql_fetch_row(result);

				// 행이 존재하면 로그인 성공
				if (row) {
					// 결과 세트 해제
					mysql_free_result(result);

					// 로그인한 사용자의 정보를 다시 가져오는 SQL 쿼리 생성
					string whoLoginquery = "SELECT Name, NO FROM MINIGAME.customer_table WHERE ID = '" + InputId + "'";

					// SQL 쿼리를 실행하고 성공 여부 확인
					if (mysql_query(&Conn, whoLoginquery.c_str()) == 0) {
						// 결과를 저장
						MYSQL_RES* result = mysql_store_result(&Conn);

						// 결과가 있을 경우
						if (result != NULL) {
							// 첫 번째 행을 가져옴
							MYSQL_ROW row = mysql_fetch_row(result);

							// 행이 존재하면 환영 메시지 출력
							if (row != NULL) {
								cout << "환영합니다! " << row[0] << "님!\n"; //row[0]에는 name이 저장되어 있음
								MemberNo = row[1]; //row[1]에는 No가 저장되어 있음

								CMain::GetInst()->SetID(InputId);

								// 3초 대기
								Sleep(3000);

								break;

							}
						}

						// 결과 세트 해제
						mysql_free_result(result);
					}
				}
				else {
					// 비밀번호 실패 최대 5번 가능
					missing += 1;
					cout << "비밀번호가 옳지않습니다." << endl;
					cout << "현재 비밀번호" << missing << "회 오류입니다." << endl;
					if (missing < 5) {
						continue;
					}
					cout << "로그인 5회 실패시 로그인이 제한됩니다." << endl;

					//결과 세트 해체
					mysql_free_result(result);

					//프로그램 종료
					cout << "\n";
					cout << "사유 : 로그인 5회 실패" << endl;
					cout << "\n\n\n\n";
					exit(0);

				}
			}
		}
		else {
			// 쿼리 실행중 오류 발생
			cout << "SQL query error: " << mysql_error(&Conn);
		}
	}
	cout << "\n";
}

void END() 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// 하늘 색상 출력
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	cout << "           #### ##  ###  ##    ##     ###  ##  ##  ###           ##  ##    ## ##   ##  ###\n";
	cout << "           # ## ##   ##  ##     ##      ## ##  ##  ##            ##  ##   ##   ##  ##   ##\n";
	cout << "             ##      ##  ##   ## ##    # ## #  ## ##             ##  ##   ##   ##  ##   ##\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "             ##      ## ###   ##  ##   ## ##   ## ##              ## ##   ##   ##  ##   ##\n";
	cout << "             ##      ##  ##   ## ###   ##  ##  ## ###              ##     ##   ##  ##   ##\n";
	cout << "             ##      ##  ##   ##  ##   ##  ##  ##  ##              ##     ##   ##  ##   ##\n";
	cout << "            ####    ###  ##  ###  ##  ###  ##  ##  ###             ##      ## ##    ## ##\n";
	cout << "\n\n\n\n";
	// 흰색으로 설정하여 다음 출력이 흰색으로 나타나도록 함
	SetConsoleTextAttribute(hConsole, 15);
}

/////////////////////////기록 추가///////////////////////////

void Record(int _score)
{
	int iScore;
	string Player_ID = CMain::GetInst()->GetID();

	iScore = _score;
	int record_No = 0; // 앞으로 회원이 등록되면 NO(회원번호)로 회원을 파악

	// 고객 테이블의 레코드 개수를 가져오기 위한 SQL 쿼리 생성
	string CountQuery;
	switch (CMain::GetInst()->GetCurMenu())
	{
	case MENU_TYPE::MINESWEEPER:
		CountQuery = "SELECT COUNT(*) FROM minesweeper_game";
		break;
	case MENU_TYPE::TYPINGGAME:
		CountQuery = "SELECT COUNT(*) FROM typing_word_game";
		break;
	}
	//고객 테이블에 있는 모든 칼럼을 검색해주세요

	// SQL 쿼리 실행
	if (mysql_query(&Conn, CountQuery.c_str()) != 0) {
		// SQL 쿼리 실행 중 오류 발생 시 오류 메시지 출력
		fprintf(stderr, "SQL 문 실행 오류: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL 쿼리 실행이 성공한 경우 결과를 가져오기
		MYSQL_RES* result = mysql_use_result(&Conn);

		// 결과 행 가져오기
		MYSQL_ROW row = mysql_fetch_row(result);

		// 결과 행이 존재하는 경우
		if (row != NULL) {
			// 레코드 개수를 정수로 변환하여 No 변수에 저장
			record_No = atoi(row[0]); //atoi()통해 정수로 반환

			// 결과 세트 해제
			mysql_free_result(result);
		}
	}
	//가입 인원 +1 =No   ex) 고객테이블에 5명의 고객이 존재하면 다음 회원부터는 NO.6 지정
	record_No += 2;

	string insertQuery;

	switch (CMain::GetInst()->GetCurMenu())
	{
	case MENU_TYPE::MINESWEEPER:
		insertQuery = "INSERT INTO `MINIGAME`.`minesweeper_game` (`No`, `Score`, `Player_ID`) VALUES ('" + to_string(record_No) + "','" + to_string(iScore) + "', '" + Player_ID + "')";
		break;
	case MENU_TYPE::TYPINGGAME:
		insertQuery = "INSERT INTO `MINIGAME`.`typing_word_game` (`No`, `Score`, `Player_ID`) VALUES ('" + to_string(record_No) + "','" + to_string(iScore) + "', '" + Player_ID + "')";
	}

	if (mysql_query(&Conn, insertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}

	cout << "\n";
}

///////////////////////////스코어 보드 확인///////////////////////////
void Check_Score()
{


	mysql_init(&Conn); // MySQL 정보 초기화

	// 데이터베이스와 연결
	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "1q2w3e4r!", "MINIGAME", 3306, (char*)NULL, 0);
	// MySql DB와 연동하기 위한 문
	//  MYSQL 구조체에 대한 포인터로 즉, 연결 성공 시 구조체가 초기화 호스트명 / 사용자 이름 / 비밀번호 / 스키마 / 포트번호 / 포인터와 숫자 0 모두 C++에서 NULL로 해석

	// 연결 결과 확인. null일 경우 실패
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
		return;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//하늘 색상 출력
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\n\n\n";
	cout << "\t:::::::::      :::     ::::    ::: :::    ::: ::::::::::: ::::    :::  :::::::: \n";
	cout << "\t:+:    :+:   :+: :+:   :+:+:   :+: :+:   :+:      :+:     :+:+:   :+: :+:    :+: \n";
	cout << "\t+:+    +:+  +:+   +:+  :+:+:+  +:+ +:+  +:+       +:+     :+:+:+  +:+ +:+        \n";
	cout << "\t+#++:++#:  +#++:++#++: +#+ +:+ +#+ +#++:++        +#+     +#+ +:+ +#+ :#:        \n";
	cout << "\t+#+    +#+ +#+     +#+ +#+  +#+#+# +#+  +#+       +#+     +#+  +#+#+# +#+   +#+# \n";
	cout << "\t#+#    #+# #+#     #+# #+#   #+#+# #+#   #+#      #+#     #+#   #+#+# #+#    #+# \n";
	cout << "\t###    ### ###     ### ###    #### ###    ### ########### ###    ####  ########  \n";

	SetConsoleTextAttribute(hConsole, 15);
	cout << "\n\n\n\n";

	setColor(COLOR::BLUE);
	gotoxy(RESOLUTION.x / 4 - 9, RESOLUTION.y / 2 - 1);
	cout << "지뢰찾기 역대 기록 " << endl;
	setColor(COLOR::WHITE);
	gotoxy(RESOLUTION.x / 4 - 13, RESOLUTION.y / 2);
	cout << "─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─" << endl;

	// 지뢰 찾기 테이블 출력 쿼리 요청
	const char* mineprintquery = "SELECT * FROM minesweeper_game ORDER BY Score ASC";

	int mine_stat;
	mine_stat = mysql_query(&Conn, mineprintquery);

	if (mine_stat != 0) {
		fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
		return ;
	}

	Result = mysql_store_result(&Conn); //MySQL에서 실행한 쿼리의 결과 집합을 Result에 저장
	gotoxy(RESOLUTION.x / 4 - 9, RESOLUTION.y / 2 + 1);
	cout << "등수 ";
	cout << "기록 ";
	cout << "아이디" << endl;
	gotoxy(RESOLUTION.x / 4 - 13, RESOLUTION.y / 2 + 2);
	cout << "─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─" << endl;
	int mine_rank = 1;


	// 반복문 내에서 Row 변수를 사용하여 결과 집합에서 한 행씩 데이터를 처리이때, NULL을 만날 때까지 계속 반복
	while ((Row = mysql_fetch_row(Result)) != NULL)
	{
		gotoxy(RESOLUTION.x / 4 - 9, RESOLUTION.y / 2 + mine_rank + 2);
		cout << mine_rank << "     ";
		cout << Row[0] << "  ";
		cout << Row[1] << endl;
		mine_rank++;
	}
	setColor(COLOR::MINT);
	gotoxy(RESOLUTION.x - 38, RESOLUTION.y / 2 - 1);
	cout << "타자게임 역대 기록" << endl;
	setColor(COLOR::WHITE);
	gotoxy(RESOLUTION.x - 42, RESOLUTION.y / 2);
	cout << "─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─" << endl;


	// 타자 게임 테이블 출력 쿼리 요청
	const char* typingprintquery = "SELECT * FROM typing_word_game ORDER BY Score ASC";

	int type_stat;
	type_stat = mysql_query(&Conn, typingprintquery);

	if (type_stat != 0) {
		fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
		return ;
	}

	Result = mysql_store_result(&Conn); //MySQL에서 실행한 쿼리의 결과 집합을 Result에 저장
	gotoxy(RESOLUTION.x - 38, RESOLUTION.y / 2 + 1);
	cout << "등수 ";
	cout << "기록 ";
	cout << "아이디" << endl;
	gotoxy(RESOLUTION.x - 42, RESOLUTION.y / 2 + 2);
	cout << "─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─" << endl;
	int typing_rank = 1;
	int Rank_Count = 0;

	// 반복문 내에서 Row 변수를 사용하여 결과 집합에서 한 행씩 데이터를 처리이때, NULL을 만날 때까지 계속 반복
	while ((Row = mysql_fetch_row(Result)) != NULL && typing_rank <= 5)
	{
		gotoxy(RESOLUTION.x - 38, RESOLUTION.y / 2 + typing_rank + 2);
		cout << typing_rank << "     ";
		cout << Row[0] << "  ";
		cout << Row[1] << endl;
		typing_rank++;
	}

	
	gotoxy(RESOLUTION.x / 2 - 20, 26);
	cout << "ESC or ENTER 를 눌러 뒤로 갈 수 있습니다" << endl;
	char c;
	while (true) 
	{
		if (_kbhit()) 
		{
			c = _getch();
			if (c == ESC || c== ENTER) 
			{
				return;
			}
		}
	}
	

	mysql_free_result(Result);// MySQL C API에서 사용한 메모리를 해제하는 함수
	mysql_close(ConnPtr); // MySQL 데이터베이스 연결을 닫는 함수
}
// 개발자 출력
void devsINFO()
{

	cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■";
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■                                                                                                ■" << endl;
	cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■";
	gotoxy(0, 1);
	setColor(MINT);
	cout << "\t  ::::    ::::     :::     :::::::::  ::::::::::     :::::::::  :::   ::: " << endl;
	cout << "\t  +:+: :+:+:+    :+: :+:   :+:    :+: :+:            :+:    :+: :+:   :+: " << endl;
	cout << "\t +:+ +:+:+ +:+  +:+   +:+  +:+    +:+ +:+            +:+    +:+  +:+ +:+  " << endl;
	cout << "\t +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#       +#++:++#+    +#++:   " << endl;
	cout << "\t +#+       +#+ +#+     +#+ +#+    +#+ +#+            +#+    +#+    +#+    " << endl;
	cout << "\t #+#       #+# #+#     #+# #+#    #+# #+#            #+#    #+#    #+#    " << endl;
	cout << "\t ###       ### ###     ### #########  ##########     #########     ###    " << endl;
	setColor(YELLOW);
	gotoxy(0, 8);
	cout << "\t      :::::::::  :::::::::: :::     ::: :::::::::: :::::::::   ::::::::  ::::::::::  " << endl
		<< "\t      +:+    :+: :+:        :+:     :+: :+:        :+:    :+: :+:    :+: :+:        " << endl
		<< "\t      +:+    +:+ +:+        +:+     +:+ +:+        +:+    +:+ +:+        +:+        " << endl
		<< "\t      #++:++#:   +#++:++#   +#+     +:+ +#++:++#   +#++:++#:  +#++:++#++ +#++:++#   " << endl
		<< "\t      +#+    +#+ +#+         +#+   +#+  +#+        +#+    +#+        +#+ +#+        " << endl
		<< "\t      #+#    #+# #+#          #+#+#+#   #+#        #+#    #+# #+#    #+# #+#        " << endl
		<< "\t      ###    ### ##########     ###     ########## ###    ###  ########  ########## ";
	setColor(BLUE);
	gotoxy(0, 17);
	cout << "\t\t\t   컴퓨터소프트웨어학과 |   정준영   |  20102112" << endl;
	cout << "\t\t\t  ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─" << endl;
	cout << "\t\t\t   컴퓨터소프트웨어학과 |   김성철   |  20102049" << endl;
	cout << "\t\t\t  ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─" << endl;
	cout << "\t\t\t   컴퓨터소프트웨어학과 |   안준섭   |  20102078" << endl;
	cout << "\t\t\t  ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─" << endl;
	cout << "\t\t\t   컴퓨터소프트웨어학과 |   이희찬   |  20102106" << endl;
	cout << "\t\t\t  ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─" << endl;
	cout << "\t\t\t   컴퓨터소프트웨어학과 |  장베드로  |  22102197" << endl << endl;
	setColor(WHITE);

	
	cout << "\t\t\t       ESC or ENTER 를 눌러 뒤로 갈 수 있습니다";
	char c;
	while (true)
	{
		if (_kbhit())
		{
			c = _getch();
			if (c == ESC || c == ENTER)
			{
				return;
			}
		}
	}


}