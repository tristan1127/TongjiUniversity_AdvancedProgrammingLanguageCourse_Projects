
#include "cmd_console_tools.h"
#include "mine_sweeper.h"
using namespace std;
/* 加入必要的头文件、命名空间、你觉得应该属于工具类的其它函数 */

/***************************************************************************
  函数名称：
  功    能：显示提示信息
  输入参数：
  返 回 值：
  说    明：如果读不懂，暂时放着，等指针和引用学习完成后再阅读
***************************************************************************/
void to_be_continued(const char* prompt, const int X = 0, const int Y = 22)
{
	cct_setcolor(); //恢复缺省颜色
	cct_gotoxy(X, Y);

	if (prompt)
		cout << prompt << "，按回车键继续...";
	else
		cout << "按回车键继续...";

	/* 忽略除回车键外的所有输入（注意：_getch的回车是\r，而getchar是\n）*/
	while (_getch() != '\r')
		;
	cout << endl;
	//cct_cls();

	return;
}

void showElapsedTime(GameContext& game) 
{
	float now = (float)clock();
	cout << "已运行时间: " << fixed << setprecision(3)<< (now - game.startTime)/ CLOCKS_PER_SEC << " 秒" << endl;
}

/*D.分任务模块(task1~9)
负责任务模块化封装。*/
void task1(GameContext& game) 
{
    initGame(game);
	setDifficulty(game);
	layMines(game);
	calculateNumbers(game);
    cct_cls();
    cout << "内部数组：" << endl;
	drawBoard(game);
	//输入内部数组
	for (int r = 0; r < game.rows; r++)
	{
		for (int c = 0; c < game.cols; c++) 
		{
			cct_gotoxy(3 + c * 2, 3 + r);
			int val = game.board[r][c].value;
			if (val == -1) 
				cout << "*";
			else 
				cout << val;
		}
	}
    cout << endl;
    to_be_continued(0, 0, game.rows + 5);
}
void task2(GameContext& game)
{
	Start(game);
	cout << endl;
	cout << endl;
	cout << endl;
	cout << "输入纵横坐标（如Gf、A1，按q退出）：" ;

	char x = 0, y = 0;
	readCoordinates(game,x,y);

	if (x == 'q')
		to_be_continued(0, 0, game.rows + 6);
	else
	{
		//初始化输入的坐标
		int startR=0, startC=0;
		startR = x - 'A';
		if (y > '0' && y <= '9')
			startC = y - '1';
		else
			startC = y - 'a' + 9;

		cout << endl;
		cout << endl;
		cout << "点开后的数组：" << endl;
		
		drawBoard(game);
		laymines(game,startR,startC);
		calculateNumbers(game);
		expandBoard(game, startR, startC);
		//  遍历并上色打印 
		for (int r = 0; r < game.rows; r++)
		{
			for (int c = 0; c < game.cols; c++)
			{
				drawCell(game, r, c,1);
			}
		}
		cout << endl;
		to_be_continued(0, 0, game.rows * 2 + 11);
	}
}
void task3(GameContext& game)
{
	Start(game);
	int count = 0;
	bool firstClick = true; // 标记是否为第一次点击

	// 注意：不要在循环外 layMines
	cout << endl;
	while (1)
	{
		count++;
		cout << endl << endl;
		cout << "输入纵横坐标（如Gf、A1，按q退出）：";

		char x = 0, y = 0;
		readCoordinates(game, x, y);

		if (x == 'q') break;

		// 1. 初始化坐标
		int startR = 0, startC = 0;
		startR = x - 'A';
		if (y > '0' && y <= '9') startC = y - '1';
		else startC = y - 'a' + 9;

		// 2. 第一次点击保护逻辑 
		if (firstClick) {
			laymines(game, startR, startC); // 保证此处值为0
			calculateNumbers(game);
			firstClick = false;
		}

		cout << endl << endl << "当前数组：" << endl;
		drawBoard(game);
		expandBoard(game, startR, startC);

		// 3. 遍历并打印
		for (int r = 0; r < game.rows; r++) {
			for (int c = 0; c < game.cols; c++) {
				drawCell(game, r, c, count);
			}
		}

		// 4. 胜负判定
		if (game.board[startR][startC].value == -1) {
			cout << endl << endl << endl << "你输了，游戏结束";
			break;
		}
		if (checkWin(game)) {
			cout << endl << endl << endl << "恭喜胜利，游戏结束";
			break;
		}
		cout << endl;
	}
	to_be_continued(0, 0, (game.rows + 7) * count - 1);
}
void task4(GameContext& game)
{
	Start(game);
	game.startTime = clock();
	int count = 0;
	int delta = 0;
	layMines(game);
	calculateNumbers(game);
	cout << endl;
	while (1)
	{
		count++;
		cout << endl;
		cout << endl;
		cout << "输入纵横坐标（如Gf、A1，按q退出，&显示时间，@A3表示标记A3为雷，#A3表示取消A3标记）：";

		char cmd, x = 0, y = 0;
		cmd = _getch();
		if (cmd == 'q')
		{
			cout << cmd;
			break;
		}
		else if (cmd == '@' || cmd == '#')
		{
			cout << cmd;
			readCoordinates(game, x, y);
			//初始化输入的坐标
			int startR = 0, startC = 0;
			coordinatesChange(game, x, y, startR, startC);
			//标记
			if (cmd == '@')
				game.board[startR][startC].state = STATE_FLAGGED;
			else if (cmd == '#')
				game.board[startR][startC].state = STATE_COVERED;

			cout << endl;
			cout << endl;
			cout << "当前数组：" << endl;

			drawBoard(game);
			//  遍历并上色打印 
			for (int r = 0; r < game.rows; r++)
			{
				for (int c = 0; c < game.cols; c++)
				{
					drawCell(game, r, c, count, delta);
				}
			}
			cout << endl;
		}
		else if (cmd == '&')
		{
			cout << cmd;
			cout << endl;
			delta += 4;
			count--;
			showElapsedTime(game);
		}
		else if (cmd >= 'A' && cmd < ('A' + game.rows))
		{
			x = cmd;
			cout << x;
			while (1)
			{
				y = _getch();
				if ((y >= '1' && y <= '9') || (y >= 'a' && y <= ('a' + game.cols - 10)))
				{
					cout << y;
					break;
				}
			}
			//初始化输入的坐标
			int startR = 0, startC = 0;
			coordinatesChange(game, x, y, startR, startC);

			cout << endl;
			cout << endl;
			cout << "当前数组：" << endl;

			drawBoard(game);
			expandBoard(game, startR, startC);
			//  遍历并上色打印 
			for (int r = 0; r < game.rows; r++)
			{
				for (int c = 0; c < game.cols; c++)
				{
					drawCell(game, r, c, count, delta);
				}
			}
			if (game.board[startR][startC].value == -1)
			{
				count++;
				cout << endl;
				cout << endl;
				cout << endl;
				cout << "你输了，游戏结束" << endl;
				delta++;
				float now = (float)clock();
				cout << "耗时: " << fixed << setprecision(3) << (now - game.startTime) / CLOCKS_PER_SEC << "秒" << endl;
				break;
			}

			if (checkWin(game))
			{
				cout << endl << endl << endl;
				cout << "恭喜胜利，游戏结束" << endl;
				delta++;
				count++;
				float now = (float)clock();
				cout << "耗时: " << fixed << setprecision(3) << (now - game.startTime) / CLOCKS_PER_SEC << "秒" << endl;
				break;
			}
			cout << endl;
		}
	}
	to_be_continued(0, 0, (game.rows + 7) * count - 1 + delta);
}
void task5(GameContext& game)
{
	initGame(game);
	setDifficulty(game);
	layMines(game);
	calculateNumbers(game);
	cct_cls();
	draw_board(game);
	for (int r = 0; r < game.rows; r++)
	{
		for (int c = 0; c < game.cols; c++)
		{
			game.board[r][c].state = STATE_OPENED;
			draw_cell(game, r, c);
		}
	}
	to_be_continued(0, 0, game.rows*3 + 5);
}
void task6(GameContext& game)
{
	initGame(game);
	setDifficulty(game);
	layMines(game);
	calculateNumbers(game);
	cct_cls();
	draw_board(game);

	for (int r = 0; r < game.rows; r++)
	{
		for (int c = 0; c < game.cols; c++)
		{
			game.board[r][c].state = STATE_OPENED;
			draw_cell(game, r, c);
		}
	}

	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);

	int MX, MY, MAction, k1, k2;
	while (true)
	{
		int event = cct_read_keyboard_and_mouse(MX, MY, MAction, k1, k2);

		if (event == CCT_MOUSE_EVENT)
		{
			// 更新状态并获取是否合法
			bool isValidPos = update_mouseStatus(game, MX, MY);

			// 如果是左键点击 且 位置合法，则退出
			if (MAction == MOUSE_LEFT_BUTTON_CLICK && isValidPos)
			{
				break;
			}
		}
	}

	cct_disable_mouse();
	to_be_continued(0, 0, game.rows * 3 + 5);
}
void task7(GameContext& game)
{ 
	initGame(game);
	setDifficulty(game);
	cct_cls();
	draw_board(game);

	for (int r = 0; r < game.rows; r++)
	{
		for (int c = 0; c < game.cols; c++)
		{
			draw_cell(game, r, c);
		}
	}

	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);

	int MX, MY, MAction, k1, k2;
	int clickR = -1, clickC = -1; // 定义变量来存储点击的行和列
	while (true)
	{
		int event = cct_read_keyboard_and_mouse(MX, MY, MAction, k1, k2);

		if (event == CCT_MOUSE_EVENT)
		{
			// 更新状态并获取是否合法
			bool isValidPos = update_mouseStatus(game, MX, MY);

			// 如果是左键点击 且 位置合法，则退出
			if (MAction == MOUSE_LEFT_BUTTON_CLICK && isValidPos)
			{
				clickC = (MX - 2) / 6;
				clickR = (MY - 2) / 3;
				laymines(game, clickR, clickC);
				calculateNumbers(game);
				expandBoard(game, clickR, clickC);
				for (int r = 0; r < game.rows; r++)
				{
					for (int c = 0; c < game.cols; c++)
					{
						draw_cell(game, r, c);
					}
				}
				break;
			}
		}
	}

	cct_disable_mouse();
	to_be_continued(0, 0, game.rows * 3 + 5);
}
void task8(GameContext& game)
{
	initGame(game);
	setDifficulty(game);
	cct_cls();
	draw_board(game);

	for (int r = 0; r < game.rows; r++)
	{
		for (int c = 0; c < game.cols; c++)
		{
			draw_cell(game, r, c);
		}
	}

	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);

	int MX, MY, MAction, k1, k2;
	int clickR = -1, clickC = -1; // 定义变量来存储点击的行和列

	//第一次点击不是雷
	while (true)
	{
		int event = cct_read_keyboard_and_mouse(MX, MY, MAction, k1, k2);

		if (event == CCT_MOUSE_EVENT)
		{
			// 更新状态并获取是否合法
			bool isValidPos = update_mouseStatus(game, MX, MY);

			// 如果是左键点击 且 位置合法，则退出
			if (MAction == MOUSE_LEFT_BUTTON_CLICK && isValidPos)
			{
				clickC = (MX - 2) / 6;
				clickR = (MY - 2) / 3;
				laymines(game, clickR, clickC);
				calculateNumbers(game);
				expandBoard(game, clickR, clickC);
				for (int r = 0; r < game.rows; r++)
				{
					for (int c = 0; c < game.cols; c++)
					{
						draw_cell(game, r, c);
					}
				}
				break;
			}
		}
	}
	while (true)
	{
		int event = cct_read_keyboard_and_mouse(MX, MY, MAction, k1, k2);

		if (event == CCT_MOUSE_EVENT)
		{
			// 更新状态并获取是否合法
			bool isValidPos = update_mouseStatus(game, MX, MY);

			// 如果是左键点击 且 位置合法，则退出
			if (MAction == MOUSE_LEFT_BUTTON_CLICK && isValidPos)
			{
				clickC = (MX - 2) / 6;
				clickR = (MY - 2) / 3;
				expandBoard(game, clickR, clickC);
				for (int r = 0; r < game.rows; r++)
				{
					for (int c = 0; c < game.cols; c++)
					{
						draw_cell(game, r, c);
					}
				}
			}
			else if (MAction == MOUSE_RIGHT_BUTTON_CLICK && isValidPos)
			{
				// 1. 计算当前点击的行列下标
				int r = (MY - 2) / 3;
				int c = (MX - 2) / 6;

				// 2. 只有在格子还没被翻开时，才能进行标记或取消标记
				if (game.board[r][c].state == STATE_COVERED)
				{
					game.board[r][c].state = STATE_FLAGGED;
					draw_cell(game, r, c); // 重新绘制为旗子状态
				}
				else if (game.board[r][c].state == STATE_FLAGGED)
				{
					game.board[r][c].state = STATE_COVERED;
					draw_cell(game, r, c); // 恢复为未翻开状态
				}
				// 注意：如果 state 是 STATE_OPENED (已翻开)，右键通常不执行任何操作
			}
		}
		int infoX = 0;
		int infoY = 2 + game.rows * 3 + 1;
		if (game.board[clickR][clickC].value == -1)
		{
			cct_gotoxy(infoX, infoY);
			cout << "你输了，游戏结束        ";
			break;
		}

		if (checkWin(game))
		{
			cct_gotoxy(infoX, infoY);
			cout << "恭喜胜利，游戏结束       ";
			break;
		}
	}

	cct_disable_mouse();
	to_be_continued(0, 0, game.rows * 3 + 5);
}
void task9(GameContext& game)
{
	initGame(game);
	setDifficulty(game);
	game.startTime = clock();  // 记录开始时间

	cct_cls();
	draw_board(game);

	// 初始化显示所有格子
	for (int r = 0; r < game.rows; r++)
	{
		for (int c = 0; c < game.cols; c++)
		{
			draw_cell(game, r, c);
		}
	}

	// 在第一行显示初始提示信息
	cct_gotoxy(0, 0);
	cout << "ESC退出，空格显示时间";

	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);

	int MX, MY, MAction, k1, k2;
	int clickR = -1, clickC = -1;
	int flagCount = 0;  // 记录插旗数量
	bool firstClick = true;  // 是否是第一次点击

	// 第一次点击不是雷
	while (true)
	{
		int event = cct_read_keyboard_and_mouse(MX, MY, MAction, k1, k2);
		float now = (float)clock();
		float elapsed = (now - game.startTime) / CLOCKS_PER_SEC;
		// 键盘事件处理
		if (event == CCT_KEYBOARD_EVENT)
		{
			if (k1 == 27)  // ESC 键
			{
				break;  // 直接退出到 to_be_continued
			}
			else if (k1 == ' ')  // 空格键 - 显示时间
			{
				now = (float)clock();
				elapsed = (now - game.startTime) / CLOCKS_PER_SEC;

				// 在第一行显示当前时间，覆盖之前的内容
				cct_gotoxy(0, 0);
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);  // 黄字黑底
				cout << "当前时间：" << fixed << setprecision(6) << elapsed<< "秒，";
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
					cout<<"ESC退出，空格显示时间                    ";
			}
		}

		// 鼠标事件处理
		if (event == CCT_MOUSE_EVENT)
		{
			bool isValidPos = update_mouseStatus(game, MX, MY);

			if (MAction == MOUSE_LEFT_BUTTON_CLICK && isValidPos)
			{
				clickC = (MX - 2) / 6;
				clickR = (MY - 2) / 3;

				if (firstClick)
				{
					// 第一次点击：布雷并计算数字
					laymines(game, clickR, clickC);
					calculateNumbers(game);
					firstClick = false;
				}

				expandBoard(game, clickR, clickC);

				// 刷新棋盘
				for (int r = 0; r < game.rows; r++)
				{
					for (int c = 0; c < game.cols; c++)
					{
						draw_cell(game, r, c);
					}
				}

				// 每次左键点击后，显示剩余雷数
				cct_gotoxy(0, 0);
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);  // 黄字黑底
				cout << "剩余雷数：" << (game.mineCount - flagCount)<< "，";
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				cout<<"ESC退出，空格显示时间                    ";

				// 检查是否踩雷
				if (game.board[clickR][clickC].value == -1)
				{
					int infoY = 2 + game.rows * 3 + 1;
					cct_gotoxy(0, infoY);
					cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);  // 黄字黑底
					cout << "共用时：" << fixed << setprecision(6) << elapsed<< "秒，";
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
					cout << "你输了，游戏结束        ";
					break;
				}

				// 检查是否胜利
				if (checkWin(game))
				{
					int infoY = 2 + game.rows * 3 + 1;
					cct_gotoxy(0, infoY);
					cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);  // 黄字黑底
					cout << "共用时：" << fixed << setprecision(6) << elapsed << "秒，";
					cct_setcolor(COLOR_BLACK, COLOR_WHITE);
					cout << "恭喜胜利，游戏结束       ";
					break;
				}
			}
			else if (MAction == MOUSE_RIGHT_BUTTON_CLICK && isValidPos)
			{
				// 右键插旗/取消旗
				int r = (MY - 2) / 3;
				int c = (MX - 2) / 6;

				if (game.board[r][c].state == STATE_COVERED)
				{
					game.board[r][c].state = STATE_FLAGGED;
					flagCount++;  // 插旗数+1
					draw_cell(game, r, c);
				}
				else if (game.board[r][c].state == STATE_FLAGGED)
				{
					game.board[r][c].state = STATE_COVERED;
					flagCount--;  // 取消旗，插旗数-1
					draw_cell(game, r, c);
				}

				// 每次右键点击后，更新剩余雷数
				cct_gotoxy(0, 0);
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);  // 黄字黑底
				cout << "剩余雷数：" << (game.mineCount - flagCount) << "，";
				cct_setcolor(COLOR_BLACK, COLOR_WHITE);
				cout<<"ESC退出，空格显示时间                    ";
			}
		}
	}

	cct_disable_mouse();
	to_be_continued(0, 0, game.rows * 3 + 5);
}