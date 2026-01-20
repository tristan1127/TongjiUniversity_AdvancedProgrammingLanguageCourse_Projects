#pragma once

/* 将所有const常量定义的头文件包含进来 */
#include "hanoi_const_value.h"
#include "cmd_console_tools.h"
#include "cmd_hdc_tools.h"

#include <iostream>
#include <iomanip>
#include <conio.h>
#include <Windows.h>

// 函数声明（必须与 .cpp 中定义一致）
void show_menu();
void handle_menu_item(int choice);

// hanoi.h
#ifndef HANOI_H
#define HANOI_H

// 全局变量声明
extern int g_totalMoves;
extern int g_towers[3][10];
extern int g_counts[3];
extern int g_delay;

void hanoiCore(int n, char src, char dst, char tmp, int mode);
void doMode(int mode, int n, char src, char dst, char tmp);

// 功能函数
void textHanoiDemo(int mode, int n, char src, char dst,char tmp); //mode=1~4：控制打印格式；delay 仅 mode=4 使用
void drawMode4Background(char src, char dst, int n);
void drawMode8Background(char src, char dst, int n);
void drawMode9Background(char src, char dst, int n);
void menu5_drawThreeRods();                                       //无参数，绘制三根空柱子（用 hdc_rectangle）  
void menu6_drawInitialDisks(char src, char dst, char tmp, int n); //在指定柱 src 上绘制 n 个盘子
void menu7_animateFirstMove(char src, char dst, char tmp, int n);//演示第一个盘子从 src → tmp → dst 的完整路径
void menu9_gameMode();                                            //人工操作模式（交互式输入 AB 等）

// 辅助函数
void resetState();                   //重置全局状态：清零 g_towers, g_counts, g_totalMoves
void initTowers(int n, int srcIdx);  //将 n 个盘子初始化到 srcIdx（0/1/2）柱
void moveDisk(int src, int dst);     //从 src 柱顶移动一个盘子到 dst 柱（更新 g_towers 和 g_counts）
void drawThreeRods();                //画三根柱子（供菜单5-9共用）
void drawDisksOnTower(int towerIdx,int n); //画某一柱上的所有盘子
void animateMoveDisk(int srcIdx, int dstIdx, int diskNum);
// 移动一个盘子的动画（上移→平移→下移）

int charToIndex(char c);             //'A'→0, 'B'→1, 'C'→2（自动转大写）  
char indexToChar(int i);             //0→'A', 1→'B', 2→'C'

// 文本版盘子状态打印（菜单3/4专用）
void printTowersHorizontal();      // 横向打印（如 A: 3 2 1 ...）
void printTowersVertical4();        // 纵向打印（柱状图）
void printTowersVertical8();
void printTowersVertical9();

// 输入函数
void inputN(int& n);                                  //输入层数（1–10）
void inputSrcDst(char& src, char& dst,char& tmp);     //输入起始柱和目标柱（A–C，自动转大写）    
void inputDelay(int mode);                            //输入延时（如 0–200ms）

// 菜单函数
void show_menu();                                     //显示主menu
void handle_menu_item(int choice);                    //根据 choice 调用对应 menuX_... 函数

#endif
/* ------------------------------------------------------------------------------------------------------

     本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ------------------------------------------------------------------------------------------------------ */
