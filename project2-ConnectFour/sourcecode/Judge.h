/*********************************************************	Judge.h : 用于棋局评判的辅助函数                    **********************************************************//*	用于检测给定棋局是否为某一方的胜局或是否出现了平局	这些函数你可能会使用，也可能不需要使用这些函数，这取决于你自己的策略*/#ifndef JUDGE_H_#define JUDGE_H_/*	userWin - 判断用户是否赢	machineWin - 判断机器是否赢	isTie - 判断是否为平局	使用这几个函数时请注意以下几个前提		1. 我们在每一个落子之后(包括用户落子或计算机落子)进行是否出现胜局的判断		2. 显然，用户落子之后不可能出现计算机反而赢的情况，计算机落子之后也不可能出现用户反而赢的情况		3. 因此，在用户落子之后，我们只需判断是否用户赢或者出现平局；计算机落子之后同理		4. 由以上三点保证，每次落子之后我们只需在以落子点核心的四个方向上进行胜局检测就可以了，而不需全盘检索		5. 三个函数更具体的使用注意事项请见各自的注释*//*	intput:		(x, y) - 用户的落子点 ( x - 所在行 y - 所在列 左上角为原点 组数下标从0开始计数 )		(M, N) - 棋盘规模 ( M - 行数 N - 列数 )		board - 待检测的棋盘二维数组，注意是在落子前的棋盘基础上修改好(x,y)处的值之后的结果	output:		true - 用户赢		false - 用户未赢 注意，由于我们约定检测用户是否赢一定是在用户落子之后，所以此时不需要考虑计算机是否赢				实际上，计算机一定没有赢，此时的情况只有两种:				1. 游戏尚未结束，双方都未赢				2. 游戏以平局结束				对于以上这两种情况，我们可以用isTie函数进行进一步的判断*/
bool judgeWinUrgent(const int x, const int y, const int M, const int N, int* const* board,int* const top,int nox,int noy,int user);
bool judgeWin(const int x, const int y, const int M, const int N, int* const* board,int* const top,int nox,int noy,int user);bool isTie(const int N, const int* top);

bool hasSpace(int y,int* const top,int nox,int noy);

int getNextX(int y,int* const top,int nox,int noy);

int getNextNextX(int y,int* const top,int nox,int noy);

//迫手
bool isToWin(const int x, const int y, const int M, const int N, int* const* board,int* const top,int nox,int noy,int user);

int valueJudge(const int x, const int y, const int M, const int N, int* const* board, int u);#endif
