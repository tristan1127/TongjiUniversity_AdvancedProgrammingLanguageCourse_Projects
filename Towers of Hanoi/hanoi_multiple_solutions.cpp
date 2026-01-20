
#include <iostream>
#include <Windows.h> 
#include <iomanip>
#include "hanoi.h"
using namespace std;
/* ----------------------------------------------------------------------------------

     本文件功能：
	1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

     本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */
int g_totalMoves;    //全局计数器，初始为 0
int g_towers[3][10]; //g_towers[0]=A柱，[1]=B，[2]=C；值为盘子编号（1~n），0 表示空
int g_counts[3];     //g_counts[i] 表示第 i 根柱上的盘子数（也是下一个插入位置）
int g_delay;         //用于 Sleep(g_delay)，菜单 4/7/8/9 设置

void hanoiCore(int n, char src, char dst, char tmp, int mode)
{
    if (n > 1)
        hanoiCore(n - 1, src, tmp, dst, mode);  
    g_totalMoves++;
    doMode(mode, n, src, dst,tmp);                   
    if (n > 1)
        hanoiCore(n - 1, tmp, dst, src, mode);
}

void doMode(int mode, int n, char src,char dst,char tmp)
{

    if (mode == 1)
    {
        cout << n << "# " << src << "---->" << dst << endl;
    }
    else if (mode == 2)
    {
        cout<<"第"<<setw(4)<< g_totalMoves<<" 步( "<<n<<"#: "<<src<<"-->"<<dst<<")" << endl;
    }
    else if (mode == 3) 
    {
        int srcIdx = charToIndex(src);
        int dstIdx = charToIndex(dst);
        moveDisk(srcIdx, dstIdx);

        cout << "第" << setw(4) << g_totalMoves << " 步( " << n << "#: " << src << "-->" << dst << ") ";
        printTowersHorizontal();
    }
    else if (mode == 4)
    {
        // 处理延时
        if (g_delay == 0)
        {
            // 按回车单步演示
            while (_getch() != '\r');
        }
        else
        {
            // 延时
            Sleep(g_delay);
        }

        int srcIdx = charToIndex(src);
        int dstIdx = charToIndex(dst);

        int diskNum = g_towers[srcIdx][g_counts[srcIdx] - 1];  // 要移动的盘子编号
        int srcOldHeight = g_counts[srcIdx] - 1;  // 源柱上该盘子的位置（从0开始）
        int dstNewHeight = g_counts[dstIdx];      // 目标柱上的新位置

        int srcCol = MenuItem4_Start_X + Underpan_A_X_OFFSET + srcIdx * Underpan_Distance;  
        cct_gotoxy(srcCol, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2 - srcOldHeight);
        cout << " ";  // 清除原位置

        // 执行移动
        moveDisk(srcIdx, dstIdx);

        int dstCol = MenuItem4_Start_X+Underpan_A_X_OFFSET + dstIdx * Underpan_Distance;
        cct_gotoxy(dstCol, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2 - dstNewHeight);
        cout << diskNum;  // 绘制新位置

        // 在底部显示步骤信息和横向数组
        cct_gotoxy(MenuItem4_Start_X, MenuItem4_Start_Y);
        cout << "第" << setw(4) << g_totalMoves << " 步( "
            << setw(2) << n << "#: " << src << "-->" << dst << ")";
        printTowersHorizontal();
    }
    else if (mode == 8)
    {
        int srcIdx = charToIndex(src);
        int dstIdx = charToIndex(dst);

        //此时 moveDisk 还没调，g_counts[srcIdx] 还是旧的，正好用来取顶层盘子
        int diskNum = g_towers[srcIdx][g_counts[srcIdx] - 1];

        // A. 先出控制台文字 
        cct_gotoxy(MenuItem8_Start_X, MenuItem8_Start_Y);
        cout << "第" << setw(4) << g_totalMoves << " 步( " << setw(2) << n << "#: " << src << "-->" << dst << ")";

        // B. 执行图形动画
        animateMoveDisk(srcIdx, dstIdx, diskNum);

        // C. 动画播完了，现在正式修改逻辑数组数据
        moveDisk(srcIdx, dstIdx);

        // D. 数据更新后，打印底部的横向数组
        printTowersHorizontal();
    }
}

//输入函数
void inputN(int& n)
{
    while (1)
    {
        cout << "请输入汉诺塔的层数(1-10)" << endl;
        cin >> n;
        if (n >= 1 && n <= 10)
            break;
    }

}
void inputSrcDst(char& src, char& dst,char& tmp)
{
    while (1)
    {
        cout << "请输入起始柱(A-C)" << endl;
        cin >> src;
        if (src >= 'A' && src <= 'C')
            break;
        else if (src >= 'a' && src <= 'c')
        {
            src = src - 'a' + 'A';
            break;
        }
    }
    while (1)
    {
        cout << "请输入目标柱(A-C)" << endl;
        cin >> dst;
        if (dst >= 'A' && dst <= 'C')
        {
            if (dst != src)
                break;
            else
                cout << "目标柱("<<dst<<")不能与起始柱("<<src<<")相同" << endl;
        }
        else if (dst >= 'a' && dst <= 'c')
        {
            dst = dst - 'a' + 'A';
            if (dst != src)
                break;
            else
                cout << "目标柱(" << dst << ")不能与起始柱(" << src << ")相同" << endl;
        }
    }
    tmp = 'A' + 'B' + 'C' - src - dst;
}     
void inputDelay(int mode) 
{
    if (mode == 4)
    {
        while (1)
        {
            cout << "请输入移动速度(0-200：0-按回车单步演示 1-200:延时1-200ms) ";
            cin >> g_delay;
            if (g_delay >= 0 && g_delay <= 200)
                break;
        }
    }
    if (mode == 7 || mode == 8 || mode == 9)
    {
        while (1)
        {
            cout << "请输入移动速度(0-20：0-按回车单步演示 1-20:延时1-20ms) ";
            cin >> g_delay;
            if (g_delay >= 0 && g_delay <= 20)
                break;
        }
    }
}

// 辅助函数
void resetState() 
{
    g_totalMoves = 0;
    for (int i = 0; i < 3; i++) 
    {
        g_counts[i] = 0;
        for (int j = 0; j < 10; j++) 
        {
            g_towers[i][j] = 0;
        }
    }
    g_delay=0;
}     
void drawThreeRods()
{
    Sleep(HDC_Init_Delay);  // 初始化延时

    // 2. 从左往右画三个底座
    for (int i = 0; i < 3; i++)
    {
        int base_x = HDC_Start_X + i * (HDC_Base_Width * 23 + HDC_Underpan_Distance);

        hdc_rectangle
        (
            base_x,
            HDC_Start_Y,
            HDC_Base_Width * 23,
            HDC_Base_High,
            HDC_COLOR[11]
        );

        Sleep(HDC_Init_Delay);  // 每画一个底座延时
    }

    // 3. 从左往右画三根立柱
    for (int i = 0; i < 3; i++)
    {
        int base_x = HDC_Start_X + i * (HDC_Base_Width * 23 + HDC_Underpan_Distance);

        hdc_rectangle(
            base_x + 11 * HDC_Base_Width,
            HDC_Start_Y - 12 * HDC_Base_High,
            HDC_Base_Width,
            HDC_Base_High * 12,
            HDC_COLOR[11]
        );

        Sleep(HDC_Init_Delay);  // 每画一根立柱延时
    }
}
void drawDisksOnTower(int towerIdx,int n)
{
    for (int i = 0; i < n; i++)
    {
        int diskNum = g_towers[towerIdx][i];
        int diskWidth = HDC_Base_Width * (2 * diskNum + 1);
        int disk_x = HDC_Start_X + HDC_Base_Width * 11 - diskNum * HDC_Base_Width;
        int disk_y = HDC_Start_Y - HDC_Base_High * (1 + i);

        hdc_rectangle(disk_x, disk_y, diskWidth, HDC_Base_High, HDC_COLOR[diskNum]);
        Sleep(HDC_Init_Delay);  // 每画一个盘子延时
    }
}
void animateMoveDisk(int srcIdx, int dstIdx, int diskNum)
{
    int diskWidth = HDC_Base_Width * (2 * diskNum + 1);
    int rodWidth = HDC_Base_Width;
    int rodHigh = HDC_Base_High * 12;

    // 柱子中心线 X 坐标
    int rod_x_src = HDC_Start_X + srcIdx * (HDC_Base_Width * 23 + HDC_Underpan_Distance) + 11 * HDC_Base_Width;
    int rod_x_dst = HDC_Start_X + dstIdx * (HDC_Base_Width * 23 + HDC_Underpan_Distance) + 11 * HDC_Base_Width;

    // 当前位置和目标位置
    int current_x = rod_x_src - diskNum * HDC_Base_Width;
    int current_y = HDC_Start_Y - HDC_Base_High * g_counts[srcIdx];
    int target_x = rod_x_dst - diskNum * HDC_Base_Width;
    int target_y = HDC_Start_Y - HDC_Base_High * (g_counts[dstIdx] + 1);

    // --- 阶段1：上移 ---
    while (current_y > HDC_Top_Y) 
    {
        // 1. 将下部 HDC_Step_Y 行用底色抹除
        hdc_rectangle(current_x, current_y + HDC_Base_High - HDC_Step_Y, diskWidth, HDC_Step_Y, HDC_COLOR[0]);

        // 2. 将刚抹除区域的中间部分恢复为立柱颜色
        if (current_y + HDC_Base_High - HDC_Step_Y >= HDC_Start_Y - rodHigh)
        {
            hdc_rectangle(rod_x_src, current_y + HDC_Base_High - HDC_Step_Y, rodWidth, HDC_Step_Y, HDC_COLOR[11]);
        }

        // 3. 计算新位置并把上部新出的行补上盘子颜色
        current_y -= HDC_Step_Y;
        if (current_y < HDC_Top_Y) current_y = HDC_Top_Y;
        hdc_rectangle(current_x, current_y, diskWidth, HDC_Step_Y, HDC_COLOR[diskNum]);

        Sleep(g_delay == 0 ? 1 : g_delay);
    }
    // --- 阶段2：平移 ---
    int step_x = (target_x > current_x) ? HDC_Step_X : -HDC_Step_X;
    while (current_x != target_x)
    {
        if (step_x > 0)
        { // 向右移
            // 抹除左边空出的 1 列
            hdc_rectangle(current_x, current_y, HDC_Step_X, HDC_Base_High, HDC_COLOR[0]);

            current_x += HDC_Step_X;
            // 补右边新出的 1 列
            hdc_rectangle(current_x + diskWidth - HDC_Step_X, current_y, HDC_Step_X, HDC_Base_High, HDC_COLOR[diskNum]);
        }
        else 
        { // 向左移
            // 抹除右边空出的 1 列
            hdc_rectangle(current_x + diskWidth - HDC_Step_X, current_y, HDC_Step_X, HDC_Base_High, HDC_COLOR[0]);

            current_x -= HDC_Step_X;
            // 补左边新出的 1 列
            hdc_rectangle(current_x, current_y, HDC_Step_X, HDC_Base_High, HDC_COLOR[diskNum]);
        }
        Sleep(g_delay == 0 ? 1 : g_delay);
    }

    // --- 阶段3：下移 ---
    while (current_y < target_y) 
    {
        // 1. 将上部空出的 HDC_Step_Y 行用底色抹除
        hdc_rectangle(current_x, current_y, diskWidth, HDC_Step_Y, HDC_COLOR[0]);

        // 2. 恢复立柱
        if (current_y >= HDC_Start_Y - rodHigh)
        {
            hdc_rectangle(rod_x_dst, current_y, rodWidth, HDC_Step_Y, HDC_COLOR[11]);
        }

        // 3. 计算新位置并在底部画出新出的 HDC_Step_Y 行
        current_y += HDC_Step_Y;
        if (current_y > target_y) current_y = target_y;
        hdc_rectangle(current_x, current_y + HDC_Base_High - HDC_Step_Y, diskWidth, HDC_Step_Y, HDC_COLOR[diskNum]);

        Sleep(g_delay == 0 ? 1 : g_delay);
    }
}
void initTowers(int n, int srcIdx)
{
    g_counts[srcIdx] = n;
    for (int i = 0; i < n; i++)
        g_towers[srcIdx][i] = n - i;  // 最大的在底部
}
void moveDisk(int srcIdx, int dstIdx)
{
    // 从src柱取出顶部盘子
    int disk = g_towers[srcIdx][--g_counts[srcIdx]];
    g_towers[srcIdx][g_counts[srcIdx]] = 0;

    // 放到dst柱顶部
    g_towers[dstIdx][g_counts[dstIdx]++] = disk;
}
void printTowersHorizontal()
{
    cout << " A:";
    for (int i = 0; i < 10; i++) {
        if (g_towers[0][i] == 0)
            cout << "  ";
        else
            cout << setw(2) << g_towers[0][i];
    }
    cout << " B:";
    for (int i = 0; i < 10; i++) {
        if (g_towers[1][i] == 0)
            cout << "  ";
        else
            cout << setw(2) << g_towers[1][i];
    }
    cout << " C:";
    for (int i = 0; i < 10; i++) {
        if (g_towers[2][i] == 0)
            cout << "  ";
        else
            cout << setw(2) << g_towers[2][i];
    }
    cout << endl;
}
void printTowersVertical4()
{
    // 清空柱子区域（10行）
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            cct_gotoxy(Underpan_A_X_OFFSET + (Underpan_Distance+1) * j, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2 - i);
            cout << " ";
        }
    }

    // 打印abc柱
    for (int j = 0; j <= 2; j++)
    {
        for (int i = 0; i < g_counts[j]; i++)
        {
            cct_gotoxy(MenuItem4_Start_X+Underpan_A_X_OFFSET + (Underpan_Distance+1) * j-1, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2 - i);
            cout <<setw(2)<< g_towers[j][i];
        }
    }
}
void printTowersVertical8()
{
    // 清空柱子区域（10行）
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            cct_gotoxy(Underpan_A_X_OFFSET + (Underpan_Distance + 1) * j, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2 - i);
            cout << " ";
        }
    }

    // 打印abc柱
    for (int j = 0; j <= 2; j++)
    {
        for (int i = 0; i < g_counts[j]; i++)
        {
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + (Underpan_Distance + 1) * j - 1, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2 - i);
            cout << setw(2) << g_towers[j][i];
        }
    }
}
void printTowersVertical9()
{
    // 清空柱子区域（10行）
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            cct_gotoxy(MenuItem9_Start_X + Underpan_A_X_OFFSET + (Underpan_Distance)*j,
                MenuItem9_Start_Y + Underpan_A_Y_OFFSET - 2 - i);
            cout << "  ";  // 两个空格，覆盖两位数字
        }
    }

    // 打印ABC柱
    for (int j = 0; j <= 2; j++)
    {
        for (int i = 0; i < g_counts[j]; i++)
        {
            cct_gotoxy(MenuItem9_Start_X + Underpan_A_X_OFFSET + (Underpan_Distance)*j,
                MenuItem9_Start_Y + Underpan_A_Y_OFFSET - 2 - i);
            cout << setw(2) << g_towers[j][i];
        }
    }
}
int charToIndex(char c)
{
    if (c >= 'a' && c <= 'c')
        c = c - 'a' + 'A';  // 转大写
    return c - 'A';
}
char indexToChar(int i)
{
    return 'A' + i;
}
/***************************************************************************
  函数名称：textHanoiDemo
  功    能：mode=1~4：控制打印格式；delay 仅 mode=4 使用
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void textHanoiDemo(int mode, int n, char src, char dst,char tmp)
{
    if (mode != 5 && mode != 6 && mode != 7 && mode != 9)
    {
        inputN(n);
        inputSrcDst(src, dst, tmp);
        if (mode >= 3)
        {
            resetState();
            int srcIdx = charToIndex(src);
            initTowers(n, srcIdx);
        }
        if (mode == 4)
        {
            inputDelay(mode);
            drawMode4Background(src, dst, n);

            // 显示初始状态
            printTowersVertical4();
            cct_gotoxy(MenuItem4_Start_X, MenuItem4_Start_Y);
            cout << "初始:  ";
            printTowersHorizontal();
        }
        if (mode == 8)
        {
            inputDelay(mode);
            drawMode8Background(src, dst, n);

            // 显示初始状态
            printTowersVertical8();
            cct_gotoxy(MenuItem8_Start_X, MenuItem8_Start_Y);
            cout << "初始:  ";
            printTowersHorizontal();
            int srcIdx = charToIndex(src);
            initTowers(n, srcIdx);
            hdc_init(HDC_COLOR[0], HDC_COLOR[0], 960, 600);
            hdc_cls();
            cct_gotoxy(0, 38);
            cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层，延时设置为" << setw(2) << g_delay << "ms（前7步，后面自动变为0ms）";
            // 4. 画三根柱子
            drawThreeRods();

            drawDisksOnTower(srcIdx, n);
        }
        hanoiCore(n, src, dst, tmp, mode);
        if (mode == 8)
        {
            hdc_release(); 
        }
    }
    else if (mode == 5)
        menu5_drawThreeRods();
    else if (mode == 6)
        menu6_drawInitialDisks(src, dst, tmp, n);
    else if (mode == 7)
        menu7_animateFirstMove(src, dst, tmp, n);
    else if (mode == 9)
        menu9_gameMode();

    if (mode == 4)
        cct_gotoxy(0, 38);
    else
        cout << endl;

    cout << "按回车键继续";
    while (1)
    {
        if (_getch() == '\r')
            break;
    }
}
void drawMode4Background(char src, char dst, int n)
{
    cct_cls();
    cct_gotoxy(0, 37);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层"<<"，延时设置为"<<setw(4)<<g_delay<<"ms";

    // 绘制底座
    cct_gotoxy(MenuItem4_Start_X+ Underpan_A_X_OFFSET-2, MenuItem4_Start_Y-3);
    for (int i = 0; i < Underpan_Distance*2+7; i++)
        cout << "=";

    // 标注A、B、C柱
    cct_gotoxy(MenuItem4_Start_X + 0 * Underpan_Distance+ Underpan_A_X_OFFSET, MenuItem4_Start_Y + Underpan_A_Y_OFFSET );
    cout << "A";
    cct_gotoxy(MenuItem4_Start_X + 1 * Underpan_Distance + Underpan_A_X_OFFSET, MenuItem4_Start_Y + Underpan_A_Y_OFFSET );
    cout << "B";
    cct_gotoxy(MenuItem4_Start_X + 2 * Underpan_Distance + Underpan_A_X_OFFSET, MenuItem4_Start_Y + Underpan_A_Y_OFFSET );
    cout << "C";
}
void drawMode8Background(char src, char dst, int n)
{
    cct_cls();
    cct_gotoxy(0, 38);

    // 绘制底座
    cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET - 2, MenuItem8_Start_Y - 3);
    for (int i = 0; i < Underpan_Distance * 2 + 7; i++)
        cout << "=";

    // 标注A、B、C柱
    cct_gotoxy(MenuItem8_Start_X + 0 * Underpan_Distance + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET);
    cout << "A";
    cct_gotoxy(MenuItem8_Start_X + 1 * Underpan_Distance + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET);
    cout << "B";
    cct_gotoxy(MenuItem8_Start_X + 2 * Underpan_Distance + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET);
    cout << "C";

}
void drawMode9Background(char src, char dst, int n)
{
    cct_cls();

    // 绘制底座
    cct_gotoxy(MenuItem9_Start_X + Underpan_A_X_OFFSET - 2, MenuItem9_Start_Y - 3);
    for (int i = 0; i < Underpan_Distance * 2 + 7; i++)
        cout << "=";

    // 标注A、B、C柱
    cct_gotoxy(MenuItem9_Start_X + 0 * Underpan_Distance + Underpan_A_X_OFFSET,
        MenuItem9_Start_Y + Underpan_A_Y_OFFSET);
    cout << "A";
    cct_gotoxy(MenuItem9_Start_X + 1 * Underpan_Distance + Underpan_A_X_OFFSET,
        MenuItem9_Start_Y + Underpan_A_Y_OFFSET);
    cout << "B";
    cct_gotoxy(MenuItem9_Start_X + 2 * Underpan_Distance + Underpan_A_X_OFFSET,
        MenuItem9_Start_Y + Underpan_A_Y_OFFSET);
    cout << "C";
}
void menu5_drawThreeRods()
{
    cct_cls();
    // 1. 初始化图形界面
    hdc_init(HDC_COLOR[0], HDC_COLOR[0], 960, 600);
    hdc_cls();
    cct_gotoxy(0, 38);
    drawThreeRods();
    hdc_release();
}
void menu6_drawInitialDisks(char src,char dst,char tmp, int n)
{
    //1.输出
    inputN(n);
    inputSrcDst(src, dst, tmp);
    // 2. 初始化数组
    resetState();
    int srcIdx = charToIndex(src);
    initTowers(n, srcIdx);

    // 3. 初始化图形界面
    cct_cls();
    hdc_init(HDC_COLOR[0], HDC_COLOR[0], 960, 600);
    hdc_cls();
    cct_gotoxy(0, 38);
    cout<< "从 "<<src<<" 移动到 "<<dst<<"，共 "<<n<<" 层"<<endl;

    // 4. 画三根柱子
    drawThreeRods();

    drawDisksOnTower(srcIdx, n);

    hdc_release();
}
void menu7_animateFirstMove(char src, char dst,char tmp, int n)
{
    //1.输出
    inputN(n);
    inputSrcDst(src, dst, tmp);
    inputDelay(7);
    // 2. 初始化数组
    resetState();
    int srcIdx = charToIndex(src);
    initTowers(n, srcIdx);

    // 3. 初始化图形界面
    cct_cls();
    hdc_init(HDC_COLOR[0], HDC_COLOR[0], 960, 600);
    hdc_cls();
    cct_gotoxy(0, 38);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层" << endl;

    // 4. 画三根柱子
    drawThreeRods();

    drawDisksOnTower(srcIdx, n);


    // 计算实际移动的目标柱（第一次移动不一定是src->dst）
    int actualDstIdx;
    if (n % 2 == 1) 
    {
        // 奇数层：第一次移动 src -> dst
        actualDstIdx = charToIndex(dst);
    }
    else 
    {
        // 偶数层：第一次移动 src -> tmp
        actualDstIdx = charToIndex(tmp);
    }

    // 6. 移动第一个盘子（最小的1号盘）
    int diskNum = g_towers[srcIdx][g_counts[srcIdx] - 1];
    animateMoveDisk(srcIdx, actualDstIdx, diskNum);
    moveDisk(srcIdx, actualDstIdx);

    hdc_release();
}
void menu9_gameMode()
{
    int n;
    char src, dst, tmp;

    // 1. 输入
    inputN(n);
    inputSrcDst(src, dst, tmp);
    inputDelay(9);

    // 2. 初始化
    resetState();
    int srcIdx = charToIndex(src);
    int dstIdx = charToIndex(dst);
    initTowers(n, srcIdx);

    // 3. 初始化控制台背景
    drawMode9Background(src, dst, n);

    // 4. 显示初始纵向数组
    printTowersVertical9();

    // 5. 显示初始横向数组
    cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y);
    cout << "初始:  ";
    printTowersHorizontal();

    // 6. 初始化图形界面
    hdc_init(HDC_COLOR[0], HDC_COLOR[0], 960, 600);
    hdc_cls();
    cct_gotoxy(0, 38);
    cout << "从 " << src << " 移动到 " << dst << "，共 " << n << " 层，延时设置为" << setw(2) << g_delay << "ms";

    // 7. 画三根柱子和盘子
    drawThreeRods();
    drawDisksOnTower(srcIdx, n);

    // 8. 游戏主循环
    while (1)
    {
        // 检查是否完成
        if (g_counts[dstIdx] == n)
        {
            cct_gotoxy(0, 39);
            cout << "游戏结束!!!";
            break;
        }

        // 提示输入
        cct_gotoxy(0, 39);
        cout << "请输入移动的柱号(命令形式: AC=A顶端的盘子移动到C, Q=退出) : ";

        // 读取输入
        char input[10];
        cin >> input;

        // 处理退出
        if (input[0] == 'Q' || input[0] == 'q')
        {
            cct_gotoxy(0, 39);
            cout << "游戏中止                                                          ";
            break;
        }

        // 解析输入
        char from = input[0];
        char to = input[1];

        // 转大写
        if (from >= 'a' && from <= 'c')
            from = from - 'a' + 'A';
        if (to >= 'a' && to <= 'c')
            to = to - 'a' + 'A';

        // 验证输入格式
        if (from < 'A' || from > 'C' || to < 'A' || to > 'C' || from == to)
        {
            cct_gotoxy(0, 39);
            cout << "                                                                  ";
            continue;
        }

        int fromIdx = charToIndex(from);
        int toIdx = charToIndex(to);

        // 检查源柱是否为空
        if (g_counts[fromIdx] == 0)
        {
            cct_gotoxy(0, 39);
            cout << "                                                                  ";
            continue;
        }

        // 检查是否违反规则
        int movingDisk = g_towers[fromIdx][g_counts[fromIdx] - 1];
        if (g_counts[toIdx] > 0 && g_towers[toIdx][g_counts[toIdx] - 1] < movingDisk)
        {
            cct_gotoxy(0, 39);
            cout << "                                                                  ";
            continue;
        }

        // 执行移动
        g_totalMoves++;

        // 动画移动
        animateMoveDisk(fromIdx, toIdx, movingDisk);
        moveDisk(fromIdx, toIdx);

        // 更新纵向显示
        printTowersVertical9();

        // 更新横向显示
        cct_gotoxy(MenuItem9_Start_X, MenuItem9_Start_Y);
        cout << "第" << setw(4) << g_totalMoves << " 步("
            << setw(2) << movingDisk << "#: " << from << "-->" << to << ")";
        printTowersHorizontal();

        // 清空提示行
        cct_gotoxy(0, 39);
        cout << "                                                                  ";
    }

    hdc_release();
}