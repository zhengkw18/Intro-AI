/******************************************************** *	Strategy.h : 策略接口文件                           * *********************************************************/#ifndef STRATEGY_H_#define	STRATEGY_H_#include "Point.h"Point* getPoint(const int M, const int N, const int* top, const int* _board, const int lastX, const int lastY, const int noX, const int noY);void clearPoint(Point* p);void clearArray(int M, int N, int** board);/*	添加你自己的辅助函数 */int bestChild(int t);void play(int x,int y,bool u);
int getResultUrgent(int x,int y,bool u);int getResult(int x,int y,bool u);void expand(int t);int treePolicy(int t);void defaultPolicy(int t);
int pprPolicy(int t);void backward(int t,int res);int test(int x,int y,bool u);
int testUrgent(int x,int y,bool u);
void setDisabled(int t,int res);
int valueJudge(const int x, const int y, const int M, const int N, int* const* board, int u);#endif