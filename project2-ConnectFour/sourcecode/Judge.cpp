#include "Judge.h"
#include <iostream>
using namespace std;

bool judgeWinUrgent(const int x, const int y, const int M, const int N, int* const* board, int* const top, int nox, int noy, int user)
{
    //横向检测
    int u1 = user ? 1 : 2;
    int i, j;
    int count = 0;
    for (i = y - 1; i >= 0; i--)
        if (!(board[x][i] == u1)) {
            break;
        }
    count += (y - i);
    for (i = y + 1; i < N; i++)
        if (!(board[x][i] == u1)) {
            break;
        }
    count += (i - y - 1);
    if (count >= 4)
        return true;
    //纵向检测
    count = 0;
    for (i = x + 1; i < M; i++)
        if (!(board[i][y] == u1))
            break;
    count += (i - x);
    if (count >= 4)
        return true;

    //左下-右上
    count = 0;
    for (i = x + 1, j = y - 1; i < M && j >= 0; i++, j--)
        if (!(board[i][j] == u1)) {
            break;
        }
    count += (y - j);
    for (i = x - 1, j = y + 1; i >= 0 && j < N; i--, j++)
        if (!(board[i][j] == u1)) {
            break;
        }
    count += (j - y - 1);
    if (count >= 4)
        return true;

    //左上-右下
    count = 0;
    for (i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--)
        if (!(board[i][j] == u1)) {
            break;
        }
    count += (y - j);
    for (i = x + 1, j = y + 1; i < M && j < N; i++, j++)
        if (!(board[i][j] == u1)) {
            break;
        }
    count += (j - y - 1);
    if (count >= 4)
        return true;
    return false;
}

bool judgeWin(const int x, const int y, const int M, const int N, int* const* board, int* const top, int nox, int noy, int user)
{
    //横向检测
    int u1 = user ? 1 : 2;
    int u2 = user ? 2 : 1;
    int i, j, left, right, leftblock, rightblock, up, down;
    int count = 0;
    leftblock = rightblock = 0;
    for (i = y - 1; i >= 0; i--)
        if (!(board[x][i] == u1)) {
            if (board[x][i] == u2)
                leftblock = 1;
            break;
        }
    count += (y - i);
    left = i;
    for (i = y + 1; i < N; i++)
        if (!(board[x][i] == u1)) {
            if (board[x][i] == u2)
                rightblock = 1;
            break;
        }
    count += (i - y - 1);
    right = i;
    if (count >= 4)
        return true;
    if (leftblock == 0 && rightblock == 0 && left != -1 && right != N && count == 3 && hasSpace(left, top, nox, noy) && getNextX(left, top, nox, noy) == x && hasSpace(right, top, nox, noy) && getNextX(right, top, nox, noy) == x)
        return true;
    //纵向检测
    count = 0;
    for (i = x + 1; i < M; i++)
        if (!(board[i][y] == u1))
            break;
    count += (i - x);
    if (count >= 4)
        return true;

    //左下-右上
    count = 0;
    leftblock = rightblock = 0;
    for (i = x + 1, j = y - 1; i < M && j >= 0; i++, j--)
        if (!(board[i][j] == u1)) {
            if (board[x][i] == u2)
                leftblock = 1;
            break;
        }
    count += (y - j);
    left = j;
    down = i;
    for (i = x - 1, j = y + 1; i >= 0 && j < N; i--, j++)
        if (!(board[i][j] == u1)) {
            if (board[x][i] == u2)
                rightblock = 1;
            break;
        }
    count += (j - y - 1);
    right = j;
    up = i;
    if (count >= 4)
        return true;
    if (leftblock == 0 && rightblock == 0 && left != -1 && right != N && down != M && up != -1 && count == 3 && hasSpace(left, top, nox, noy) && getNextX(left, top, nox, noy) == x + y - left && hasSpace(right, top, nox, noy) && getNextX(right, top, nox, noy) == x + y - right)
        return true;
    //左上-右下
    count = 0;
    leftblock = rightblock = 0;
    for (i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--)
        if (!(board[i][j] == u1)) {
            if (board[x][i] == u2)
                leftblock = 1;
            break;
        }
    count += (y - j);
    left = j;
    up = i;
    for (i = x + 1, j = y + 1; i < M && j < N; i++, j++)
        if (!(board[i][j] == u1)) {
            if (board[x][i] == u2)
                rightblock = 1;
            break;
        }
    count += (j - y - 1);
    right = j;
    down = i;
    if (count >= 4)
        return true;
    if (leftblock == 0 && rightblock == 0 && left != -1 && right != N && down != M && up != -1 && count == 3 && hasSpace(left, top, nox, noy) && getNextX(left, top, nox, noy) == x - y + left && hasSpace(right, top, nox, noy) && getNextX(right, top, nox, noy) == x - y + right)
        return true;
    return false;
}

bool isTie(const int N, const int* top)
{
    bool tie = true;
    for (int i = 0; i < N; i++) {
        if (top[i] > 0) {
            tie = false;
            break;
        }
    }
    return tie;
}

bool hasSpace(int y, int* const top, int nox, int noy)
{
    return (top[y] > 1 || (top[y] == 1 && !(nox == 0 && noy == y)));
}

int getNextX(int y, int* const top, int nox, int noy)
{
    int x = top[y] - 1;
    if (x == nox && y == noy)
        x--;
    return x;
}

int getNextNextX(int y, int* const top, int nox, int noy)
{
    int topy = top[y];
    int x = topy - 1;
    if (x == nox && y == noy)
        x--;
    topy = x;
    if (topy > 1 || (topy == 1 && !(nox == 0 && noy == y))) {
        x = topy - 1;
        if (x == nox && y == noy)
            x--;
        return x;
    } else {
        return -1;
    }
}

bool isToWin(const int x, const int y, const int M, const int N, int* const* board, int* const top, int nox, int noy, int user)
{
    int u1 = user ? 1 : 2;
    int x1, x2, x3, x4, x5, x6, y1, y2, y3, y4, y5, y6;
    y1 = y - 3;
    y2 = y - 2;
    y3 = y - 1;
    y4 = y + 1;
    y5 = y + 2;
    y6 = y + 3;
    //横向检测
    x1 = x2 = x3 = x4 = x5 = x6 = x;
    //XXX_
    //1
    if (0 <= x6 && x6 < M && 0 <= y6 && y6 < N && board[x4][y4] == u1 && board[x5][y5] == u1 && hasSpace(y6, top, nox, noy) && getNextX(y6, top, nox, noy) == x6)
        return true;
    //2
    if (0 <= x5 && x5 < M && 0 <= y5 && y5 < N && 0 <= x3 && x3 < M && 0 <= y3 && y3 < N && board[x3][y3] == u1 && board[x4][y4] == u1 && hasSpace(y5, top, nox, noy) && getNextX(y5, top, nox, noy) == x5)
        return true;
    //3
    if (0 <= x2 && x2 < M && 0 <= y2 && y2 < N && 0 <= x4 && x4 < M && 0 <= y4 && y4 < N && board[x2][y2] == u1 && board[x3][y3] == u1 && hasSpace(y4, top, nox, noy) && getNextX(y4, top, nox, noy) == x4)
        return true;
    //XX_X
    //1
    if (0 <= x6 && x6 < M && 0 <= y6 && y6 < N && board[x4][y4] == u1 && board[x6][y6] == u1 && hasSpace(y5, top, nox, noy) && getNextX(y5, top, nox, noy) == x5)
        return true;
    //2
    if (0 <= x5 && x5 < M && 0 <= y5 && y5 < N && 0 <= x3 && x3 < M && 0 <= y3 && y3 < N && board[x3][y3] == u1 && board[x5][y5] == u1 && hasSpace(y4, top, nox, noy) && getNextX(y4, top, nox, noy) == x4)
        return true;
    //3
    if (0 <= x1 && x1 < M && 0 <= y1 && y1 < N && board[x1][y1] == u1 && board[x2][y2] == u1 && hasSpace(y3, top, nox, noy) && getNextX(y3, top, nox, noy) == x3)
        return true;

    //XXX_
    //1
    if (0 <= x1 && x1 < M && 0 <= y1 && y1 < N && board[x3][y3] == u1 && board[x2][y2] == u1 && hasSpace(y1, top, nox, noy) && getNextX(y1, top, nox, noy) == x1)
        return true;
    //2
    if (0 <= x2 && x2 < M && 0 <= y2 && y2 < N && 0 <= x4 && x4 < M && 0 <= y4 && y4 < N && board[x4][y4] == u1 && board[x3][y3] == u1 && hasSpace(y2, top, nox, noy) && getNextX(y2, top, nox, noy) == x2)
        return true;
    //3
    if (0 <= x5 && x5 < M && 0 <= y5 && y5 < N && 0 <= x3 && x3 < M && 0 <= y3 && y3 < N && board[x5][y5] == u1 && board[x4][y4] == u1 && hasSpace(y3, top, nox, noy) && getNextX(y3, top, nox, noy) == x3)
        return true;
    //XX_X
    //1
    if (0 <= x1 && x1 < M && 0 <= y1 && y1 < N && board[x3][y3] == u1 && board[x1][y1] == u1 && hasSpace(y2, top, nox, noy) && getNextX(y2, top, nox, noy) == x2)
        return true;
    //2
    if (0 <= x2 && x2 < M && 0 <= y2 && y2 < N && 0 <= x4 && x4 < M && 0 <= y4 && y4 < N && board[x4][y4] == u1 && board[x2][y2] == u1 && hasSpace(y3, top, nox, noy) && getNextX(y3, top, nox, noy) == x3)
        return true;
    //3
    if (0 <= x6 && x6 < M && 0 <= y6 && y6 < N && board[x6][y6] == u1 && board[x5][y5] == u1 && hasSpace(y4, top, nox, noy) && getNextX(y4, top, nox, noy) == x4)
        return true;
    //纵向检测
    if (x + 2 < M && board[x + 1][y] == u1 && board[x + 2][y] == u1 && hasSpace(y, top, nox, noy))
        return true;
    //左下-右上
    x1 = x + 3;
    x2 = x + 2;
    x3 = x + 1;
    x4 = x - 1;
    x5 = x - 2;
    x6 = x - 3;
    //XXX_
    //1
    if (0 <= x6 && x6 < M && 0 <= y6 && y6 < N && board[x4][y4] == u1 && board[x5][y5] == u1 && hasSpace(y6, top, nox, noy) && getNextX(y6, top, nox, noy) == x6)
        return true;
    //2
    if (0 <= x5 && x5 < M && 0 <= y5 && y5 < N && 0 <= x3 && x3 < M && 0 <= y3 && y3 < N && board[x3][y3] == u1 && board[x4][y4] == u1 && hasSpace(y5, top, nox, noy) && getNextX(y5, top, nox, noy) == x5)
        return true;
    //3
    if (0 <= x2 && x2 < M && 0 <= y2 && y2 < N && 0 <= x4 && x4 < M && 0 <= y4 && y4 < N && board[x2][y2] == u1 && board[x3][y3] == u1 && hasSpace(y4, top, nox, noy) && getNextX(y4, top, nox, noy) == x4)
        return true;
    //XX_X
    //1
    if (0 <= x6 && x6 < M && 0 <= y6 && y6 < N && board[x4][y4] == u1 && board[x6][y6] == u1 && hasSpace(y5, top, nox, noy) && getNextX(y5, top, nox, noy) == x5)
        return true;
    //2
    if (0 <= x5 && x5 < M && 0 <= y5 && y5 < N && 0 <= x3 && x3 < M && 0 <= y3 && y3 < N && board[x3][y3] == u1 && board[x5][y5] == u1 && hasSpace(y4, top, nox, noy) && getNextX(y4, top, nox, noy) == x4)
        return true;
    //3
    if (0 <= x1 && x1 < M && 0 <= y1 && y1 < N && board[x1][y1] == u1 && board[x2][y2] == u1 && hasSpace(y3, top, nox, noy) && getNextX(y3, top, nox, noy) == x3)
        return true;

    //XXX_
    //1
    if (0 <= x1 && x1 < M && 0 <= y1 && y1 < N && board[x3][y3] == u1 && board[x2][y2] == u1 && hasSpace(y1, top, nox, noy) && getNextX(y1, top, nox, noy) == x1)
        return true;
    //2
    if (0 <= x2 && x2 < M && 0 <= y2 && y2 < N && 0 <= x4 && x4 < M && 0 <= y4 && y4 < N && board[x4][y4] == u1 && board[x3][y3] == u1 && hasSpace(y2, top, nox, noy) && getNextX(y2, top, nox, noy) == x2)
        return true;
    //3
    if (0 <= x5 && x5 < M && 0 <= y5 && y5 < N && 0 <= x3 && x3 < M && 0 <= y3 && y3 < N && board[x5][y5] == u1 && board[x4][y4] == u1 && hasSpace(y3, top, nox, noy) && getNextX(y3, top, nox, noy) == x3)
        return true;
    //XX_X
    //1
    if (0 <= x1 && x1 < M && 0 <= y1 && y1 < N && board[x3][y3] == u1 && board[x1][y1] == u1 && hasSpace(y2, top, nox, noy) && getNextX(y2, top, nox, noy) == x2)
        return true;
    //2
    if (0 <= x2 && x2 < M && 0 <= y2 && y2 < N && 0 <= x4 && x4 < M && 0 <= y4 && y4 < N && board[x4][y4] == u1 && board[x2][y2] == u1 && hasSpace(y3, top, nox, noy) && getNextX(y3, top, nox, noy) == x3)
        return true;
    //3
    if (0 <= x6 && x6 < M && 0 <= y6 && y6 < N && board[x6][y6] == u1 && board[x5][y5] == u1 && hasSpace(y4, top, nox, noy) && getNextX(y4, top, nox, noy) == x4)
        return true;
    //左上-右下
    x1 = x - 3;
    x2 = x - 2;
    x3 = x - 1;
    x4 = x + 1;
    x5 = x + 2;
    x6 = x + 3;
    //XXX_
    //1
    if (0 <= x6 && x6 < M && 0 <= y6 && y6 < N && board[x4][y4] == u1 && board[x5][y5] == u1 && hasSpace(y6, top, nox, noy) && getNextX(y6, top, nox, noy) == x6)
        return true;
    //2
    if (0 <= x5 && x5 < M && 0 <= y5 && y5 < N && 0 <= x3 && x3 < M && 0 <= y3 && y3 < N && board[x3][y3] == u1 && board[x4][y4] == u1 && hasSpace(y5, top, nox, noy) && getNextX(y5, top, nox, noy) == x5)
        return true;
    //3
    if (0 <= x2 && x2 < M && 0 <= y2 && y2 < N && 0 <= x4 && x4 < M && 0 <= y4 && y4 < N && board[x2][y2] == u1 && board[x3][y3] == u1 && hasSpace(y4, top, nox, noy) && getNextX(y4, top, nox, noy) == x4)
        return true;
    //XX_X
    //1
    if (0 <= x6 && x6 < M && 0 <= y6 && y6 < N && board[x4][y4] == u1 && board[x6][y6] == u1 && hasSpace(y5, top, nox, noy) && getNextX(y5, top, nox, noy) == x5)
        return true;
    //2
    if (0 <= x5 && x5 < M && 0 <= y5 && y5 < N && 0 <= x3 && x3 < M && 0 <= y3 && y3 < N && board[x3][y3] == u1 && board[x5][y5] == u1 && hasSpace(y4, top, nox, noy) && getNextX(y4, top, nox, noy) == x4)
        return true;
    //3
    if (0 <= x1 && x1 < M && 0 <= y1 && y1 < N && board[x1][y1] == u1 && board[x2][y2] == u1 && hasSpace(y3, top, nox, noy) && getNextX(y3, top, nox, noy) == x3)
        return true;

    //XXX_
    //1
    if (0 <= x1 && x1 < M && 0 <= y1 && y1 < N && board[x3][y3] == u1 && board[x2][y2] == u1 && hasSpace(y1, top, nox, noy) && getNextX(y1, top, nox, noy) == x1)
        return true;
    //2
    if (0 <= x2 && x2 < M && 0 <= y2 && y2 < N && 0 <= x4 && x4 < M && 0 <= y4 && y4 < N && board[x4][y4] == u1 && board[x3][y3] == u1 && hasSpace(y2, top, nox, noy) && getNextX(y2, top, nox, noy) == x2)
        return true;
    //3
    if (0 <= x5 && x5 < M && 0 <= y5 && y5 < N && 0 <= x3 && x3 < M && 0 <= y3 && y3 < N && board[x5][y5] == u1 && board[x4][y4] == u1 && hasSpace(y3, top, nox, noy) && getNextX(y3, top, nox, noy) == x3)
        return true;
    //XX_X
    //1
    if (0 <= x1 && x1 < M && 0 <= y1 && y1 < N && board[x3][y3] == u1 && board[x1][y1] == u1 && hasSpace(y2, top, nox, noy) && getNextX(y2, top, nox, noy) == x2)
        return true;
    //2
    if (0 <= x2 && x2 < M && 0 <= y2 && y2 < N && 0 <= x4 && x4 < M && 0 <= y4 && y4 < N && board[x4][y4] == u1 && board[x2][y2] == u1 && hasSpace(y3, top, nox, noy) && getNextX(y3, top, nox, noy) == x3)
        return true;
    //3
    if (0 <= x6 && x6 < M && 0 <= y6 && y6 < N && board[x6][y6] == u1 && board[x5][y5] == u1 && hasSpace(y4, top, nox, noy) && getNextX(y4, top, nox, noy) == x4)
        return true;

    if (hasSpace(y, top, nox, noy)) {
        int xx = getNextNextX(y, top, nox, noy);
        if (xx != -1 && judgeWinUrgent(xx, y, M, N, board, top, nox, noy, user))
            return true;
    }
    return false;
}

inline int Max(int a, int b)
{
    return a > b ? a : b;
}

inline int localScore(int count, int small_count, int large_count)
{
    if (count >= 4)
        return 1000;
    switch (Max(Max(count, small_count), large_count)) {
    case 1:
        return 1;
    case 2:
        return 3;
    default:
        return 5;
    }
}

int valueJudge(const int x, const int y, const int M, const int N, int* const* board, int u)
{
    int player = u ? 1 : 2;
    int i, j, score = 0;
    bool to_continue = false;
    int count = 1, small_count = 1, large_count = 1;
    for (i = y - 1; i >= 0; i--)
        if (board[x][i] != player) {
            if (board[x][i] == 0)
                to_continue = true;
            break;
        }
    count += (y - i - 1);
    large_count += (y - i - 1);
    if (to_continue) {
        for (i = i - 1; i >= 0; i--)
            if (board[x][i] != player) {
                break;
            }
        small_count += (y - i - 2);
    }
    to_continue = false;
    for (i = y + 1; i < N; i++)
        if (board[x][i] != player) {
            if (board[x][i] == 0)
                to_continue = true;
            break;
        }
    count += (i - y - 1);
    small_count += (i - y - 1);
    if (to_continue) {
        for (i = i + 1; i < N; ++i)
            if (board[x][i] != player) {
                break;
            }
        large_count += (i - y - 2);
    }
    score += localScore(count, small_count, large_count);

    count = 1;
    for (i = x + 1; i < M; i++)
        if (board[i][y] != player)
            break;
    count += (i - x - 1);
    score += localScore(count, count, count);

    count = 1, small_count = 1, large_count = 1;
    to_continue = false;
    for (i = x + 1, j = y - 1; i < M && j >= 0; i++, j--)
        if (board[i][j] != player) {
            if (board[i][j] == 0)
                to_continue = true;
            break;
        }
    count += (y - j - 1);
    large_count += (y - j - 1);
    if (to_continue) {
        for (i = i + 1, j = j - 1; i < M && j >= 0; i++, j--) {
            if (board[i][j] != player)
                break;
        }
        small_count += (y - j - 2);
    }
    to_continue = false;
    for (i = x - 1, j = y + 1; i >= 0 && j < N; i--, j++)
        if (board[i][j] != player) {
            if (board[i][j] == 0)
                to_continue = true;
            break;
        }
    count += (j - y - 1);
    small_count += (j - y - 1);
    if (to_continue) {
        for (i = i - 1, j = j + 1; i >= 0 && j < N; i--, j++) {
            if (board[i][j] != player)
                break;
        }
        large_count += (j - y - 2);
    }
    score += localScore(count, small_count, large_count);

    count = 1, small_count = 1, large_count = 1;
    to_continue = false;
    for (i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--)
        if (board[i][j] != player) {
            if (board[i][j] == 0)
                to_continue = true;
            break;
        }
    count += (y - j - 1);
    large_count += (y - j - 1);
    if (to_continue) {
        for (i = i - 1, j = j - 1; i >= 0 && j >= 0; i--, j--) {
            if (board[i][j] != player)
                break;
        }
        small_count += (y - j - 2);
    }
    to_continue = false;
    for (i = x + 1, j = y + 1; i < M && j < N; i++, j++)
        if (board[i][j] != player) {
            if (board[i][j] == 0)
                to_continue = true;
            break;
        }
    count += (j - y - 1);
    small_count += (j - y - 1);
    if (to_continue) {
        for (i = i + 1, j = j + 1; i < M && j < N; i++, j++) {
            if (board[i][j] != player)
                break;
        }
        large_count += (j - y - 2);
    }
    score += localScore(count, small_count, large_count);

    return score;
}
