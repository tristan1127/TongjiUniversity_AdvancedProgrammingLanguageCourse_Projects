
#include "cmd_console_tools.h"
#include "mine_sweeper.h"
using namespace std;
/* 空文件，给出的目的是为了防止把文件名弄错 */

void drawBoard(const GameContext& game) 
{
    // 1. 打印顶部的数字/字母坐标行 (1 2 3...a b c...)
    cout << "  |";
    for (int c = 0; c < game.cols; c++) 
    {
        if (c + 1 <= 9) 
        {
            cout << c + 1 << " "; // 1-9
        }
        else {
            // 超过9显示 a, b, c... (ASCII码: 10 + 87 = 97 是 'a')
            cout << (char)(c + 1 - 10 + 'a') << " ";
        }
    }
    cout << endl;

    // 2. 打印分隔横线 (--+----------------)
    cout << "--+";
    for (int c = 0; c <= game.cols; c++)
    {
        cout << "--";
    }
    cout << endl;

    // 3. 打印左侧字母坐标 (A, B, C...) 及左边界
    for (int r = 0; r < game.rows; r++) 
    {
        // ASCII码: 0 + 65 = 65 是 'A'
        cout << (char)(r + 'A') << " |";

        // 注意：这里只画出了坐标轴，棋盘内部的内容由 drawCell 或 refreshScreen 完成
        for (int c = 0; c < game.cols; c++)
        {
            cout << "  "; // 先留空
        }
        cout << endl;
    }
}

void drawCell(const GameContext& game, int r, int c,int count)
{
    // 定位到棋盘内部对应的坐标
    cct_gotoxy(3 + c * 2, 3 + r + (game.rows+7)*count);

    if (game.board[r][c].state == STATE_OPENED)
    {
        // --- 已翻开区域：黄色背景反显 ---
        cct_setcolor(14, game.board[r][c].value);

        int val = game.board[r][c].value;
        if (val == -1) 
        {
            cct_setcolor(0, 7);
            cout << "*"; // 虽然子题目2要求首点非雷，但逻辑上建议保留
        }
        else
        {
            cout << val; // 显示数字 (0-8)
        }
    }
    else if(game.board[r][c].state == STATE_COVERED)
    {
        // --- 未翻开区域：普通黑底白字 ---
        cct_setcolor(0, 7);
        cout << "X"; // 其余位置仍为 X 
    }
    else if (game.board[r][c].state == STATE_FLAGGED)
    {
        // --- 被插旗区域：红底白字 ---
        cct_setcolor(4, 7);
        cout << "X"; // 其余位置仍为 X 
    }
    cct_setcolor(0, 7);
}

void drawCell(const GameContext& game, int r, int c, int count,int delta)
{
    // 定位到棋盘内部对应的坐标
    cct_gotoxy(3 + c * 2, 3 + r + (game.rows + 7) * count+delta);

    if (game.board[r][c].state == STATE_OPENED)
    {
        // --- 已翻开区域：黄色背景反显 ---
        cct_setcolor(14, game.board[r][c].value);

        int val = game.board[r][c].value;
        if (val == -1)
        {
            cct_setcolor(0, 7);
            cout << "*"; // 虽然子题目2要求首点非雷，但逻辑上建议保留
        }
        else
        {
            cout << val; // 显示数字 (0-8)
        }
    }
    else if (game.board[r][c].state == STATE_COVERED)
    {
        // --- 未翻开区域：普通黑底白字 ---
        cct_setcolor(0, 7);
        cout << "X"; // 其余位置仍为 X 
    }
    else if (game.board[r][c].state == STATE_FLAGGED)
    {
        // --- 被插旗区域：红底白字 ---
        cct_setcolor(4, 7);
        cout << "X"; // 其余位置仍为 X 
    }
    cct_setcolor(0, 7);
}

void draw_board(const GameContext& game)
{
    int startX = 2;  // 棋盘左上角起点X
    int startY = 2;  // 棋盘左上角起点Y
    int cols = game.cols;
    int rows = game.rows;

    // 1. 打印顶部的列坐标 (a b c d...) - 居中对齐到格子中间
    cct_gotoxy(startX + 3, startY - 1);  // startX + 3 让字母对齐到格子中心
    for (int c = 0; c < cols; c++)
    {
        if ((c + 'a') <= 'z')
            cout << (char)(c + 'a');
        else
            cout << int(c );
        cout << "     ";  // 每个格子6字符宽，字母后5空格到下一个字母
    }

    // 2. 打印左侧的行坐标 (A B C D...)
    for (int r = 0; r < rows; r++)
    {
        cct_gotoxy(0, startY + 1 + r * 3);  // 每格高3行
        cout << (char)('A' + r);
    }

    // 3. 绘制棋盘框架
    cct_setcolor(COLOR_HWHITE, COLOR_BLACK);

    // 字符查找表 [行位置][列位置]
    const char* corner[3][3] = {
        {"╔", "╦", "╗"},  // 顶部
        {"╠", "╬", "╣"},  // 中间
        {"╚", "╩", "╝"}   // 底部
    };

    // 绘制所有行（包括边框）
    for (int r = 0; r <= rows; r++)
    {
        cct_gotoxy(startX, startY + r * 3);

        for (int c = 0; c <= cols; c++)
        {
            // 确定当前位置类型
            int rowType = (r == 0) ? 0 : (r == rows) ? 2 : 1;
            int colType = (c == 0) ? 0 : (c == cols) ? 2 : 1;

            // 输出边角字符
            cout << corner[rowType][colType];

            // 如果不是最后一列，输出横线
            if (c < cols)
            {
                cout << " ═ ═ ";  // 5个字符 (空格+═+空格+═+空格)
            }
        }
        cout << " ";  // ← 最右边加一个空格对齐

        // 如果不是最后一行，绘制内容区的竖线
        if (r < rows)
        {
            // 第一行（空白行）
            cct_gotoxy(startX, startY + r * 3 + 1);
            for (int c = 0; c <= cols; c++)
            {
                cout << "║";
                if (c < cols)
                    cout << "     ";  // 5个空格
            }
            cout << " ";  // ← 最右边加一个空格

            // 第二行（数字行）
            cct_gotoxy(startX, startY + r * 3 + 2);
            for (int c = 0; c <= cols; c++)
            {
                cout << "║";
                if (c < cols)
                    cout << "     ";  // 5个空格
            }
            cout << " ";  // ← 最右边加一个空格
        }
    }

    cct_setcolor(COLOR_BLACK, COLOR_WHITE);
}

void draw_cell(const GameContext& game, int r, int c)
{
    int startX = 2;  // 和 draw_board 保持一致
    int startY = 2;

    // 计算该格子内容区域的左上角坐标
    // 每格宽6字符，高3行
    int cellX = startX + 1 + c * 6;  // 跳过左边框 ╔ 或 ║
    int cellY = startY + 1 + r * 3;  // 跳过上边框，到内容区第一行

    if (game.board[r][c].state == STATE_OPENED)
    {
        // --- 已翻开状态 ---
        int val = game.board[r][c].value;

        if (val == -1)
        {
            // 地雷：画暗白色方块 + 黑色星号
            // 第一行：画方块上半部分
            cct_gotoxy(cellX + 1, cellY);
            cct_setcolor(7, 0);  // 黑字暗白底
            cout << "   ";  // 3个空格组成方块上部

            // 第二行：画方块下半部分 + 星号
            cct_gotoxy(cellX + 1, cellY + 1);
            cct_setcolor(7, 0);
            cout << " * ";  // 空格+星号+空格
        }
        else if (val == 0)
        {
            // 空白：只画暗白色空方块
            cct_gotoxy(cellX + 1, cellY);
            cct_setcolor(7, 0);
            cout << "   ";

            cct_gotoxy(cellX + 1, cellY + 1);
            cct_setcolor(7, 0);
            cout << "   ";
        }
        else
        {
            // 数字1-8：画暗白色方块 + 彩色数字
            // 第一行：方块上半部分
            cct_gotoxy(cellX + 1, cellY);
            cct_setcolor(7, 0);
            cout << "   ";

            // 第二行：方块下半部分 + 数字（数字颜色为val本身）
            cct_gotoxy(cellX + 1, cellY + 1);
            cct_setcolor(7, val);  // 前景色=数字值，背景=暗白(7)
            cout << " " << val << " ";  // 空格+数字+空格
        }
    }
    else if (game.board[r][c].state == STATE_COVERED)
    {
        // --- 未翻开状态：显示 X ---
        // 在格子中间画暗白色方块 + X
        cct_gotoxy(cellX + 1, cellY);
        cct_setcolor(6, 0);
        cout << "   ";

        cct_gotoxy(cellX + 1, cellY + 1);
        cct_setcolor(6, 0);
        cout << "   ";
    }
    else if (game.board[r][c].state == STATE_FLAGGED)
    {
        // --- 插旗状态：红色显示 ---
        cct_gotoxy(cellX + 1, cellY);
        cct_setcolor(12, 7);  // 红
        cout << "   ";

        cct_gotoxy(cellX + 1, cellY + 1);
        cct_setcolor(12, 7);
        cout << " # ";  
    }

    // 恢复默认颜色
    cct_setcolor(0, 7);
}

// 刷新整个棋盘
void refresh_screen(const GameContext& game)
{
    for (int r = 0; r < game.rows; r++)
    {
        for (int c = 0; c < game.cols; c++)
        {
            draw_cell(game, r, c);
        }
    }
}