#include "Strategy.h"
#include "Judge.h"
#include "Point.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

using namespace std;

const float c = 0.8, eps = 1e-6;
const int maxn = 10000000;
int nox_, noy_, M_, N_, tail;
struct UCTNode {
    int l;
    int r;
    int fa;
    int num;
    int x;
    int y;
    int state;
    int vacant;
    int available;
    int score;
    bool user;
} nodes[maxn];
struct _Point {
    int x;
    int y;
};
int** board_;
int top_[12];
/*
	策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误

	input:
 为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
 M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计， 左上角为坐标原点，行用x标记，列用y标记
 top : 当前棋盘每一列列顶的实际位置. e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0
 _board : 棋盘的一维数组表示, 为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
 你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
 board[x][y]表示第x行、第y列的点(从0开始计)
 board[x][y] == 0/1/2 分别对应(x,y)处 无落子/有用户的子/有程序的子,不可落子点处的值也为0
 lastX, lastY : 对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
 落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
 noX, noY : 棋盘上的不可落子点(注:涫嫡饫锔?龅膖op已经替你处理了不可落子点，也就是说如果某一步
 所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
 所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
 当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
 以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX lastY),你要做的就是在这些信息下给出尽可能明智的落子点
	output:
 你的落子点Point
 */
Point* getPoint(const int M, const int N, const int* top, const int* _board, const int lastX, const int lastY, const int noX, const int noY)
{
    clock_t start = clock(); //计算时间用
    srand(time(NULL));
    int x = -1, y = -1; //最终将你的落子点存到x,y中
    int** board = new int*[M];
    for (int i = 0; i < M; i++) {
        board[i] = new int[N];
        for (int j = 0; j < N; j++) {
            board[i][j] = _board[i * N + j];
        }
    }
    board_ = new int*[M];
    for (int i = 0; i < M; i++) {
        board_[i] = new int[N];
    }
    M_ = M;
    N_ = N;
    nox_ = noX;
    noy_ = noY; //大小和禁止的点
    tail = 1;
    nodes[0].l = -1;
    nodes[0].user = 1;
    nodes[0].state = -2;
    nodes[0].num = 0;
    nodes[0].score = 0;

    while (1) {
        if ((double)(clock() - start) / CLOCKS_PER_SEC > 2.8)
            break;
        if (tail > maxn - 2000)
            break;
        for (int i = 0; i < M; i++) {
            memcpy(board_[i], board[i], N * sizeof(int));
        }
        memcpy(top_, top, N * sizeof(int));
        int t = treePolicy(0);
        int res;
        if (t > 0) {
            if (nodes[t].state == -2) {
                nodes[nodes[t].fa].vacant--;
                play(nodes[t].x, nodes[t].y, nodes[t].user);
                res = isTie(N_, top_) ? 0 : -2;
                nodes[t].state = res;
                if (res == -2) {
                    defaultPolicy(t);
                }
            }
        }
    }

    float max = -(1 << 24);
    int ans;
    cerr << 0 << " " << nodes[0].score << " " << nodes[0].num << " " << nodes[0].state << " " << nodes[0].vacant << endl;
    for (int i = nodes[0].l; i < nodes[0].r; i++) //计算选出最好的落子点
    {
        cerr << i << " " << nodes[i].score << " " << nodes[i].num << " " << nodes[i].state << " " << nodes[i].vacant << endl;
        float temp = (float)nodes[i].score / ((float)nodes[i].num + eps);
        if (temp > max) {
            max = temp;
            ans = i;
        }
    }
    if (nodes[0].l == nodes[0].r) {
        for (int i = 0; i < M; i++) {
            memcpy(board_[i], board[i], N * sizeof(int));
        }
        memcpy(top_, top, N * sizeof(int));
        for (int i = 0; i < N_; i++) {
            if (hasSpace(i, top_, nox_, noy_)) {
                x = getNextX(i, top_, nox_, noy_);
                y = i;
                break;
            }
        }
    } else {
        x = nodes[ans].x;
        y = nodes[ans].y;
    }
    clearArray(M, N, board_);

    /*
     不要更改这段代码
     */
    clearArray(M, N, board);
    return new Point(x, y);
}

/*
	getPoint函数返回的Point指针是在本dll模块中声明的，为避免产生堆错误，应在外部调用本dll中的
	函数来释放空间，而不应该在外部直接delete
 */
inline void clearPoint(Point* p)
{
    delete p;
    return;
}

/*
	清除top和board数组
 */
inline void clearArray(int M, int N, int** board)
{
    for (int i = 0; i < M; i++) {
        delete[] board[i];
    }
    delete[] board;
}

inline float fastLog(float x)
{
    union {
        float f;
        uint32_t i;
    } vx = { x };
    float y = vx.i;
    y *= 8.2629582881927490e-8f;
    return y - 87.989971088f;
}

inline float fastSqrt(float x)
{
    float xhalf = 0.5f * x;
    int i = *(int*)&x; // get bits for floating VALUE
    i = 0x5f375a86 - (i >> 1); // gives initial guess y0
    x = *(float*)&i; // convert bits BACK to float
    x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
    x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
    x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy

    return 1 / x;
}

inline float randFloat()
{
    return (float)rand() / RAND_MAX;
}

inline void setDisabled(int t, int res)
{
    if (res == 1) {
        nodes[t].state = 1;
        nodes[t].score = (1 << 23);
    } else if (res == -1) {
        nodes[t].state = -1;
        nodes[t].score = -(1 << 23);
    } else
        return;
    nodes[t].num = 1;
    if (t != 0) {
        int fa = nodes[t].fa;
        nodes[fa].available--;
        if ((nodes[t].user == 1 && res == -1) || (nodes[t].user == 0 && res == 1) || nodes[fa].available == 0) {
            setDisabled(fa, res);
        }
    }
}

inline int bestChild(int t)
{
    float m = -(1 << 24), uct, score;
    int ans;
    for (int i = nodes[t].l; i < nodes[t].r; i++) {
        if (nodes[i].num == 0)
            continue;
        if (nodes[t].user == 0) {
            uct = -(float)(nodes[i].score) / ((float)nodes[i].num + eps);
        } else {
            uct = (float)(nodes[i].score) / ((float)nodes[i].num + eps);
        }
        score = uct + c * fastSqrt(2 * fastLog((float)nodes[t].num + 1 + eps) / ((float)nodes[i].num + eps));
        if (score > m) {
            m = score;
            ans = i;
        }
    }
    return ans;
}

inline void play(int x, int y, bool u)
{
    board_[x][y] = u ? 1 : 2;
    top_[y]--;
    if (x == nox_ + 1 && y == noy_)
        top_[y]--;
}

inline void deplay(int x, int y)
{
    board_[x][y] = 0;
    top_[y]++;
    if (x == nox_ + 1 && y == noy_)
        top_[y]++;
}

inline int getResultUrgent(int x, int y, bool u)
{
    play(x, y, u);
    if (u == 1 && judgeWinUrgent(x, y, M_, N_, board_, top_, nox_, noy_, 1))
        return -1;
    if (u == 0 && judgeWinUrgent(x, y, M_, N_, board_, top_, nox_, noy_, 0))
        return 1;
    if (isTie(N_, top_))
        return 0;
    return -2;
}

inline int getResult(int x, int y, bool u)
{
    play(x, y, u);
    if (u == 1 && judgeWin(x, y, M_, N_, board_, top_, nox_, noy_, 1))
        return -1;
    if (u == 0 && judgeWin(x, y, M_, N_, board_, top_, nox_, noy_, 0))
        return 1;
    if (isTie(N_, top_))
        return 0;
    return -2;
}

inline int test(int x, int y, bool u)
{
    if (u == 1 && judgeWin(x, y, M_, N_, board_, top_, nox_, noy_, 1))
        return -1;
    if (u == 0 && judgeWin(x, y, M_, N_, board_, top_, nox_, noy_, 0))
        return 1;
    return -2;
}

inline int testUrgent(int x, int y, bool u)
{
    if (u == 1 && judgeWinUrgent(x, y, M_, N_, board_, top_, nox_, noy_, 1))
        return -1;
    if (u == 0 && judgeWinUrgent(x, y, M_, N_, board_, top_, nox_, noy_, 0))
        return 1;
    return -2;
}

inline void expand(int t)
{
    nodes[t].l = tail;
    nodes[t].vacant = 0;
    nodes[t].available = 0;
    _Point feasible[12];
    int num_feasible = 0;
    bool u = !nodes[t].user;
    bool isWin = false;
    for (int i = 0; i < N_; i++) {
        if (hasSpace(i, top_, nox_, noy_)) {
            int x = getNextX(i, top_, nox_, noy_), y = i;
            if (testUrgent(x, y, u) != -2) {
                feasible[0].x = x;
                feasible[0].y = y;
                num_feasible = 1;
                isWin = true;
                break;
            }
        }
    }
    if (isWin) {
        nodes[tail].l = -1;
        nodes[tail].score = nodes[tail].num = 0;
        nodes[tail].fa = t;
        nodes[tail].user = u;
        nodes[tail].state = -2;
        nodes[tail].x = feasible[0].x;
        nodes[tail].y = feasible[0].y;
        nodes[t].vacant++;
        nodes[t].available++;
        tail++;
        if (u) {
            setDisabled(nodes[t].l, -1);
        } else {
            setDisabled(nodes[t].l, 1);
        }
    } else {
        for (int i = 0; i < N_; i++) {
            if (hasSpace(i, top_, nox_, noy_)) {
                int x = getNextX(i, top_, nox_, noy_), y = i;
                if (testUrgent(x, y, !u) != -2) {
                    feasible[0].x = x;
                    feasible[0].y = y;
                    num_feasible = 1;
                    break;
                } else {
                    int xx = getNextNextX(i, top_, nox_, noy_);
                    if (xx == -1 || testUrgent(xx, y, !u) == -2) {
                        feasible[num_feasible].x = x;
                        feasible[num_feasible].y = y;
                        num_feasible++;
                    }
                }
            }
        }
        if (num_feasible == 0) {
            if (nodes[t].user) {
                setDisabled(t, -1);
            } else {
                setDisabled(t, 1);
            }
        } else {
            _Point feasible_[12];
            int num_feasible_ = 0;
            bool isWin_ = false;
            for (int i = 0; i < num_feasible; i++) {
                int x = feasible[i].x, y = feasible[i].y;
                if (test(x, y, u) != -2) {
                    feasible_[0].x = x;
                    feasible_[0].y = y;
                    num_feasible_ = 1;
                    isWin_ = true;
                    break;
                }
            }
            if (isWin_) {
                nodes[tail].l = -1;
                nodes[tail].score = nodes[tail].num = 0;
                nodes[tail].fa = t;
                nodes[tail].user = u;
                nodes[tail].state = -2;
                nodes[tail].x = feasible_[0].x;
                nodes[tail].y = feasible_[0].y;
                nodes[t].vacant++;
                nodes[t].available++;
                tail++;
                if (u) {
                    setDisabled(nodes[t].l, -1);
                } else {
                    setDisabled(nodes[t].l, 1);
                }
            } else {
                for (int i = 0; i < num_feasible; i++) {
                    int x = feasible[i].x, y = feasible[i].y;
                    if (test(x, y, !u) != -2) {
                        play(x, y, !u);
                        num_feasible_ = 0;
                        for (int j = 0; j < num_feasible; j++) {
                            int xx = feasible[j].x, yy = feasible[j].y;
                            if ((xx == x && yy == y) || isToWin(xx, yy, M_, N_, board_, top_, nox_, noy_, u)) {
                                feasible_[num_feasible_].x = xx;
                                feasible_[num_feasible_].y = yy;
                                num_feasible_++;
                            }
                        }
                        deplay(x, y);
                        break;
                    } else {
                        int xx = getNextNextX(y, top_, nox_, noy_);
                        if (xx == -1 || test(xx, y, !u) == -2 || isToWin(x, y, M_, N_, board_, top_, nox_, noy_, u)) {
                            feasible_[num_feasible_].x = x;
                            feasible_[num_feasible_].y = y;
                            num_feasible_++;
                        }
                    }
                }
                if (num_feasible_ == 0) {
                    if (nodes[t].user) {
                        setDisabled(t, -1);
                    } else {
                        setDisabled(t, 1);
                    }
                } else {
                    for (int i = 0; i < num_feasible_; i++) {
                        nodes[tail].l = -1;
                        nodes[tail].score = nodes[tail].num = 0;
                        nodes[tail].fa = t;
                        nodes[tail].user = u;
                        nodes[tail].state = -2;
                        nodes[tail].x = feasible_[i].x;
                        nodes[tail].y = feasible_[i].y;
                        nodes[t].vacant++;
                        nodes[t].available++;
                        tail++;
                    }
                }
            }
        }
    }

    nodes[t].r = tail;
}

inline int treePolicy(int t)
{
    while (nodes[t].state == -2) {
        if (nodes[t].l == -1) {
            expand(t);
            if (nodes[t].state == -2) {
                int choice;
                int maxscore = -1, score;
                for (int i = nodes[t].l; i < nodes[t].r; i++) {
                    if (isToWin(nodes[i].x, nodes[i].y, M_, N_, board_, top_, nox_, noy_, nodes[i].user)) {
                        return i;
                    }
                }
                for (int i = nodes[t].l; i < nodes[t].r; i++) {
                    if (hasSpace(nodes[i].y, top_, nox_, noy_)) {
                        int xx = getNextNextX(nodes[i].y, top_, nox_, noy_);
                        if (xx != -1 && testUrgent(xx, nodes[i].y, nodes[i].user) != -2)
                            return i;
                    }
                }
                for (int i = nodes[t].l; i < nodes[t].r; i++) {
                    score = valueJudge(nodes[i].x, nodes[i].y, M_, N_, board_, 0) + valueJudge(nodes[i].x, nodes[i].y, M_, N_, board_, 1);
                    if (score > maxscore) {
                        choice = i;
                        maxscore = score;
                    }
                }
                return choice;
            } else {
                return -1;
            }
        } else if (nodes[t].vacant > 0) {
            int choice;
            int maxscore = -1, score;
            for (int i = nodes[t].l; i < nodes[t].r; i++) {
                if (nodes[i].num == 0) {
                    if (isToWin(nodes[i].x, nodes[i].y, M_, N_, board_, top_, nox_, noy_, nodes[i].user)) {
                        return i;
                    }
                }
            }
            for (int i = nodes[t].l; i < nodes[t].r; i++) {
                if (nodes[i].num == 0) {
                    if (hasSpace(nodes[i].y, top_, nox_, noy_)) {
                        int xx = getNextNextX(nodes[i].y, top_, nox_, noy_);
                        if (xx != -1 && testUrgent(xx, nodes[i].y, nodes[i].user) != -2)
                            return i;
                    }
                }
            }
            for (int i = nodes[t].l; i < nodes[t].r; i++) {
                if (nodes[i].num == 0) {
                    score = valueJudge(nodes[i].x, nodes[i].y, M_, N_, board_, 0) + valueJudge(nodes[i].x, nodes[i].y, M_, N_, board_, 1);
                    if (score > maxscore) {
                        choice = i;
                        maxscore = score;
                    }
                }
            }
            return choice;
        } else {
            t = bestChild(t);
            play(nodes[t].x, nodes[t].y, nodes[t].user);
        }
    }
    return t;
}

inline void defaultPolicy(int t)
{
    bool u = nodes[t].user;
    int res = -2;
    int tt = t;
    while (res == -2) {
        u = !u;
        expand(tt);
        if (nodes[tt].state == -2) {
            int score[12];
            int total = 0;
            for (int i = nodes[tt].l; i < nodes[tt].r; i++) {
                score[i - nodes[tt].l] = valueJudge(nodes[i].x, nodes[i].y, M_, N_, board_, u);
                score[i - nodes[tt].l] += valueJudge(nodes[i].x, nodes[i].y, M_, N_, board_, !u);
                total += score[i - nodes[tt].l];
            }
            int k = rand() % total;
            int limit = 0;
            for (int i = nodes[tt].l; i < nodes[tt].r; i++) {
                limit += score[i - nodes[tt].l];
                if (limit > k) {
                    tt = i;
                    break;
                }
            }
            nodes[nodes[tt].fa].vacant--;
            play(nodes[tt].x, nodes[tt].y, u);
            res = isTie(N_, top_) ? 0 : -2;
        } else {
            res = nodes[tt].state;
        }
    }
    backward(tt, res);
}

inline void backward(int t, int res)
{
    while (t != 0) {
        nodes[t].num += 1;
        nodes[t].score += res;
        t = nodes[t].fa;
    }
    nodes[0].num += 1;
}
