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
	���Ժ����ӿ�,�ú������Կ�ƽ̨����,ÿ�δ��뵱ǰ״̬,Ҫ�����������ӵ�,�����ӵ������һ��������Ϸ��������ӵ�,��Ȼ�Կ�ƽ̨��ֱ����Ϊ��ĳ�������

	input:
 Ϊ�˷�ֹ�ԶԿ�ƽ̨ά����������ɸ��ģ����д���Ĳ�����Ϊconst����
 M, N : ���̴�С M - ���� N - ���� ����0��ʼ�ƣ� ���Ͻ�Ϊ����ԭ�㣬����x��ǣ�����y���
 top : ��ǰ����ÿһ���ж���ʵ��λ��. e.g. ��i��Ϊ��,��_top[i] == M, ��i������,��_top[i] == 0
 _board : ���̵�һά�����ʾ, Ϊ�˷���ʹ�ã��ڸú����տ�ʼ���������Ѿ�����ת��Ϊ�˶�ά����board
 ��ֻ��ֱ��ʹ��board���ɣ����Ͻ�Ϊ����ԭ�㣬�����[0][0]��ʼ��(����[1][1])
 board[x][y]��ʾ��x�С���y�еĵ�(��0��ʼ��)
 board[x][y] == 0/1/2 �ֱ��Ӧ(x,y)�� ������/���û�����/�г������,�������ӵ㴦��ֵҲΪ0
 lastX, lastY : �Է���һ�����ӵ�λ��, ����ܲ���Ҫ�ò�����Ҳ������Ҫ�Ĳ������ǶԷ�һ����
 ����λ�ã���ʱ��������Լ��ĳ����м�¼�Է������ಽ������λ�ã�����ȫȡ�������Լ��Ĳ���
 noX, noY : �����ϵĲ������ӵ�(ע:�ʵ����?���top�Ѿ����㴦���˲������ӵ㣬Ҳ����˵���ĳһ��
 ������ӵ�����ǡ�ǲ������ӵ㣬��ôUI�����еĴ�����Ѿ������е�topֵ�ֽ�����һ�μ�һ������
 ��������Ĵ�����Ҳ���Ը�����ʹ��noX��noY��������������ȫ��Ϊtop������ǵ�ǰÿ�еĶ�������,
 ��Ȼ�������ʹ��lastX,lastY�������п��ܾ�Ҫͬʱ����noX��noY��)
 ���ϲ���ʵ���ϰ����˵�ǰ״̬(M N _top _board)�Լ���ʷ��Ϣ(lastX lastY),��Ҫ���ľ�������Щ��Ϣ�¸������������ǵ����ӵ�
	output:
 ������ӵ�Point
 */
Point* getPoint(const int M, const int N, const int* top, const int* _board, const int lastX, const int lastY, const int noX, const int noY)
{
    clock_t start = clock(); //����ʱ����
    srand(time(NULL));
    int x = -1, y = -1; //���ս�������ӵ�浽x,y��
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
    noy_ = noY; //��С�ͽ�ֹ�ĵ�
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
    for (int i = nodes[0].l; i < nodes[0].r; i++) //����ѡ����õ����ӵ�
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
     ��Ҫ������δ���
     */
    clearArray(M, N, board);
    return new Point(x, y);
}

/*
	getPoint�������ص�Pointָ�����ڱ�dllģ���������ģ�Ϊ��������Ѵ���Ӧ���ⲿ���ñ�dll�е�
	�������ͷſռ䣬����Ӧ�����ⲿֱ��delete
 */
inline void clearPoint(Point* p)
{
    delete p;
    return;
}

/*
	���top��board����
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
