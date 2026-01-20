
#pragma once
#include "cmd_console_tools.h"
#include <ctime>   
#include <cstdlib>
#include <iostream>
#include <conio.h>
#include <iomanip>
/* 空文件，给出的目的是防止大家把文件名弄错 */

// 状态宏定义：让代码逻辑自然
#define STATE_COVERED 0   // 覆盖
#define STATE_OPENED  1   // 挖开
#define STATE_FLAGGED 2   // 插旗

// 游戏结局状态
#define STATUS_PLAYING 0
#define STATUS_WIN     1
#define STATUS_LOSE    2

struct Cell 
{
    int value; // -1: 雷, 0: 空白, 1-8: 数字
    int state; // 0: 覆盖, 1: 挖开, 2: 插旗
};

struct GameContext 
{
    Cell board[16][30];  // 棋盘数组
    int rows, cols;      // 当前关卡行列
    int mineCount;       // 总雷数
    int cursorR, cursorC;// 光标/鼠标当前位置
    int status;          // 游戏状态 (0, 1, 2)
    int menuStep;        // 记录当前处于老师要求的哪个演示阶段 (1-9)
    unsigned int startTime; // 用于任务 4/9 的计时
};

void to_be_continued(const char* prompt, const int X , const int Y );
void showElapsedTime(GameContext& game);
void coordinatesChange(GameContext& game, char x, char y, int &startR ,int &startC );

/*A.基础逻辑模块(mine_sweeper_base.cpp)
负责数据的运算，不涉及任何 cout 或颜色显示。*/
void initGame(GameContext& game);                     //全盘清零，重置状态。
void layMines(GameContext& game);                     //随机布雷（避开玩家首点更佳）。
void laymines(GameContext& game, int startR, int startC);
void calculateNumbers(GameContext& game);             //核心算法，数 8 个邻居的雷。
void expandBoard(GameContext& game, int r, int c);    //核心递归，实现 0 的连通区展开。
bool checkWin(GameContext& game);                     // 判断是否所有非雷区都被挖开。

/*B.界面渲染模块(mine_sweeper_graph.cpp)
负责所有的 cct_gotoxy 和颜色输出。*/
void drawBoard(const GameContext& game);                       // 绘制整个静态框架。
void draw_board(const GameContext& game);
void drawCell(const GameContext& game, int r, int c,int count);// 局部绘制某个格子的当前状态。
void drawCell(const GameContext& game, int r, int c, int count,int delta);
void draw_cell(const GameContext& game, int r, int c);
void refresh_screen(const GameContext& game);                   // 根据 board 数据全量刷新画面。

/*C.菜单与控制模块(mine_sweeper_menu.cpp& main.cpp)
负责输入响应和阶段切换。*/
void showMenu(GameContext& game);                                    //显示你那 0 - 9 的选择界面。
void setDifficulty(GameContext& game);                               //选择难度
void readCoordinates(GameContext& game,char &x,char &y);
void Start(GameContext& game);
void update_mouse_status(const GameContext& game, int MX, int MY);
bool update_mouseStatus(const GameContext& game, int MX, int MY);


/*D.分任务模块(task1~9)
负责任务模块化封装。*/
void task1(GameContext& game);
void task2(GameContext& game);
void task3(GameContext& game);
void task4(GameContext& game);
void task5(GameContext& game);
void task6(GameContext& game);
void task7(GameContext& game);
void task8(GameContext& game);
void task9(GameContext& game);