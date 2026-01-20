
#include "cmd_console_tools.h"
#include "mine_sweeper.h"
using namespace std;
/* 空文件，给出的目的是为了防止把文件名弄错 */

/* A. 基础逻辑模块 (mine_sweeper_base.cpp)
功能：初始化游戏上下文，将棋盘所有格子清零并设为覆盖状态。*/

void initGame(GameContext& game) 
{
    // 1. 嵌套循环遍历整个最大可能的棋盘 (24行 * 30列)
    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 30; c++) {
            // value = 0 表示既不是雷，也没有周围雷数统计
            game.board[r][c].value = 0;

            // state = STATE_COVERED (0) 表示格子是盖着的
            game.board[r][c].state = STATE_COVERED;
        }
    }

    // 2. 初始化游戏状态为“正在进行”
    game.status = STATUS_PLAYING;

    // 3. 将光标或鼠标逻辑坐标初始化到左上角 (0,0) 或中心
    game.cursorR = 0;
    game.cursorC = 0;

    // 4. 重置计时器 (用于任务 4/9)
    game.startTime = 0;

}

void layMines(GameContext& game) 
{
    int planted = 0;
    while (planted < game.mineCount)
    {
        int r = rand() % game.rows;  
        int c = rand() % game.cols;

        // 检查是否已经是雷 
        if (game.board[r][c].value == -1)
        {
            continue; // 原地重来
        }

        // 埋雷
        game.board[r][c].value = -1;
        planted++;
    }
}

void calculateNumbers(GameContext& game)
{
    for (int i = 0; i < game.rows; i++)      
    {
        for (int j = 0; j < game.cols; j++)  
        {
            // 如果自己是雷，就不用算周围了
            if (game.board[i][j].value == -1) // '*' -> -1
                continue;

            int count = 0;
            for (int mi = -1; mi <= 1; mi++)
            {
                for (int mj = -1; mj <= 1; mj++)
                {
                    int k = i + mi;
                    int l = j + mj;

                    // 边界检查
                    if (k >= 0 && k < game.rows && l >= 0 && l < game.cols)
                    {
                        // 检查邻居是不是雷
                        if (game.board[k][l].value == -1) // '*' -> -1
                            count++;
                    }
                }
            }

            // 赋值：注意不要加 '0'，我们要存整数
            game.board[i][j].value = count;
        }
    }
}

void Start(GameContext& game)
{
    initGame(game);
    setDifficulty(game);
    cct_cls();
    cout << "内部数组：" << endl;
    drawBoard(game);
    for (int r = 0; r < game.rows; r++)
    {
        for (int c = 0; c < game.cols; c++)
        {
            cct_gotoxy(3 + c * 2, 3 + r);
            cout << "X";
        }
    }
}

void expandBoard(GameContext& game, int r, int c) 
{
    // 1. 递归边界检查：坐标必须在棋盘范围内
    if (r < 0 || r >= game.rows || c < 0 || c >= game.cols)
    {
        return;
    }

    // 2. 只有处于“覆盖”状态且“没插旗”的格子才需要处理
    // 如果这个格子已经开了，或者是旗子，直接返回（防止死循环）
    if (game.board[r][c].state != STATE_COVERED) 
    {
        return;
    }

    // 3. 揭开当前格子
    game.board[r][c].state = STATE_OPENED;

    // 4. 判断是否继续往外“蔓延”
    // 只有当前格子的数字是 0（空白），才需要揭开周围 8 个邻居
    if (game.board[r][c].value == 0) 
    {
        // 遍历周围 8 个方向
        for (int i = -1; i <= 1; i++) 
        {
            for (int j = -1; j <= 1; j++)
            {
                // 跳过自己 (0,0)
                if (i == 0 && j == 0) continue;

                // 递归调用自己，去检查邻居
                expandBoard(game, r + i, c + j);
            }
        }
    }
}

void laymines(GameContext& game, int startR, int startC)
{
    int count = 0;
    while (count < game.mineCount) 
    {
        int r = rand() % game.rows;
        int c = rand() % game.cols;

        // 【核心条件】：新生成的雷不能在起始点及其 8 个邻居位置上
        // 即：绝对值距离必须大于 1
        bool isSafeZone = (abs(r - startR) <= 1 && abs(c - startC) <= 1);

        if (game.board[r][c].value != -1 && !isSafeZone)
        {
            game.board[r][c].value = -1;
            count++;
        }
    }
}

bool checkWin(GameContext& game)
{
    // 遍历整个棋盘
    for (int r = 0; r < game.rows; r++)
    {
        for (int c = 0; c < game.cols; c++)
        {
            // 如果有一个格子不是雷，但它还没被打开，就说明还没赢
            if (game.board[r][c].value != -1 && game.board[r][c].state != STATE_OPENED)
            {
                return false;
            }
        }
    }
    // 所有非雷格子都 state == STATE_OPENED 了，赢了
    return true;
}

void coordinatesChange(GameContext& game,char x,char y, int &startR , int &startC )
{
    startR = x - 'A';
    if (y > '0' && y <= '9')
        startC = y - '1';
    else
        startC = y - 'a' + 9;
}

void update_mouse_status(const GameContext& game, int MX, int MY)
{
    int startX = 2;
    int startY = 2;

    // 提示信息显示的位置（棋盘下方）
    int infoX = 0;
    int infoY = startY + game.rows * 3 + 2;

    cct_gotoxy(infoX, infoY);

    // 先粗略计算可能在哪个格子
    int relativeX = MX - startX - 1;  // 减去左边框
    int relativeY = MY - startY - 1;  // 减去上边框

    // 判断是否在棋盘范围内
    if (relativeX >= 0 && relativeX < game.cols * 6 &&
        relativeY >= 0 && relativeY < game.rows * 3)
    {
        // 计算在哪个格子
        int c = relativeX / 6;
        int r = relativeY / 3;

        // 计算在格子内的偏移位置
        int offsetX = relativeX % 6;  // 格子内的X偏移 (0-5)
        int offsetY = relativeY % 3;  // 格子内的Y偏移 (0-2)

        // 判断是否在小方块内（3x2的区域）
        // 小方块位置：offsetX 在 [1,3]，offsetY 在 [0,1]
        if (offsetX >= 1 && offsetX <= 3 && offsetY >= 0 && offsetY <= 1)
        {
            // 在小方块内，显示行列信息
            cout << "[当前光标] " << (char)('A' + r) << "行"
                << (char)('a' + c) << "列        ";
        }
        else
        {
            // 在格子内，但不在小方块上（边框或空白区域）
            cout << "[当前光标] 位置非法        ";
        }
    }
    else
    {
        // 完全不在棋盘内
        cout << "[当前光标] 位置非法        ";
    }
}

bool update_mouseStatus(const GameContext& game, int MX, int MY)
{
    int startX = 2;
    int startY = 2;

    // 提示信息显示的位置（棋盘下方）
    int infoX = 0;
    int infoY = startY + game.rows * 3 + 1;

    cct_gotoxy(infoX, infoY);

    // 先粗略计算可能在哪个格子
    int relativeX = MX - startX - 1;  // 减去左边框
    int relativeY = MY - startY - 1;  // 减去上边框

    // 判断是否在棋盘范围内
    if (relativeX >= 0 && relativeX < game.cols * 6 &&
        relativeY >= 0 && relativeY < game.rows * 3)
    {
        // 计算在哪个格子
        int c = relativeX / 6;
        int r = relativeY / 3;

        // 计算在格子内的偏移位置
        int offsetX = relativeX % 6;  // 格子内的X偏移 (0-5)
        int offsetY = relativeY % 3;  // 格子内的Y偏移 (0-2)

        // 判断是否在小方块内
        if (offsetX >= 1 && offsetX <= 3 && offsetY >= 0 && offsetY <= 1)
        {
            // 在小方块内，显示行列信息
            cout << "[当前光标] " << (char)('A' + r) << "行"
                << (char)('a' + c) << "列        ";
            return true;  // ← 返回 true 表示位置合法
        }
    }

    // 位置非法
    cout << "[当前光标] 位置非法        ";
    return false;  // ← 返回 false 表示位置非法
}