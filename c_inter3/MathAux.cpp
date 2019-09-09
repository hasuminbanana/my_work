#include "MathAux.h"
#include <math.h>
#include <stdlib.h>
#include <vector>       // ヘッダファイルインクルード
using namespace std;

/*######################################################################*/
/* 値の大きい方を返す */
double max(double arg0, double arg1)
{
    if (arg0 < arg1) return arg1;
    else return arg0;
}

/*######################################################################*/
/* 値の小さい方を返す */
double min(double arg0, double arg1)
{
    if (arg0 < arg1) return arg0;
    else return arg1;
}

/*######################################################################*/
/* (x0, y0)から(x1, y1)までの距離を計算して返す */
double length(double x0, double y0, double x1, double y1)
{
    return sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0));
}

/*######################################################################*/
/* [0,1)の乱数を返す */
double rand_0_1()
{
    return (double)(rand()%65536)/65536;
}
