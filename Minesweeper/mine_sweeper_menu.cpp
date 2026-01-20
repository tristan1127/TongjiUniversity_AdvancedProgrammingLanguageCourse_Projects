
#include "cmd_console_tools.h"
#include "mine_sweeper.h"
using namespace std;
/* 空文件，给出的目的是为了防止把文件名弄错 */

void showMenu(GameContext& game) 
{
    cout << "--------------------------------------------" << endl;
    cout << "1.选择模式，显示内部数组" << endl;
    cout << "2.输入一个位置，显示打开区域" << endl;
    cout << "3.内部数组基础版" << endl;
    cout << "4.内部数组完整版（标记、运行时间）" << endl;
    cout << "5.画出框架，显示内部数据" << endl;
    cout << "6.检测鼠标位置和合法性，以及左键点击" << endl;
    cout << "7.鼠标点击一次，显示打开区域" << endl;
    cout << "8.允许连续游戏，支持右键标记，判断游戏结束" << endl;
    cout<<"9.完整游戏，标明雷数，空格、结束显示时间"<<endl;
    cout << "0.退出游戏" << endl;
    cout << "--------------------------------------------" << endl;
    cout << "[请选择]";

    // 这里使用 _getch() 获取按键
    char sel;
    while (1)
    {
        sel = _getch();
        if (sel >= '0' && sel <= '9')
        {
            cout << sel;
            break;
        }
    }
    // 将选择存入 game 结构体的一个变量中
    game.menuStep = sel - '0'; 
}

void setDifficulty(GameContext& game) 
{
    cct_cls();
    cout << "请输入扫雷游戏的等级（数字），初级\\中级\\高级（1\\2\\3）";

    char level;
    while (1)
    {
        level = _getch();
        if (level == '1' || level == '2' || level == '3')
        {
            cout << level;
            break;
        }
    }
    switch (level) {
    case '1':
        game.rows = 9; game.cols = 9; game.mineCount = 10;
        break;
    case '2':
        game.rows = 16; game.cols = 16; game.mineCount = 40;
        break;
    case '3':
        game.rows = 16; game.cols = 30; game.mineCount = 99;
        break;
    }
}

void readCoordinates(GameContext& game,char &x,char &y)
{
    while (1)
    {
        x = _getch();
        if (x == 'q' || (x >= 'A' && x < ('A' + game.rows)))
        {
            cout << x;
            break;
        }
    }

    if (x == 'q')
        return;
    else 
    {
        while (1)
        {
            y = _getch();
            if ((y >= '1' && y <= '9') || (y >= 'a' &&y<=('a'+game.cols-10)))
            {
                cout << y;
                break;
            }
        }
    }
}