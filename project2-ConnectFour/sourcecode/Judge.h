/********************************************************
bool judgeWinUrgent(const int x, const int y, const int M, const int N, int* const* board,int* const top,int nox,int noy,int user);


bool hasSpace(int y,int* const top,int nox,int noy);

int getNextX(int y,int* const top,int nox,int noy);

int getNextNextX(int y,int* const top,int nox,int noy);

//迫手
bool isToWin(const int x, const int y, const int M, const int N, int* const* board,int* const top,int nox,int noy,int user);

int valueJudge(const int x, const int y, const int M, const int N, int* const* board, int u);