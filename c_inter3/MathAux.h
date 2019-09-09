/* MathAux.h */
/*
 * 追加的な数学関数を宣言する
 */

/* 多重インクルード防止 */
/*
 * 複数ファイルに分割して開発する場合，ヘッダ(.h)ファイルは
 * 複数の箇所から読み込まれる (多重インクルード)．
 * これはコンパイルエラーのもととなるため防止する．
 *
 * #ifndef~#endif，#defineマクロを利用することが多い．
 * 2回目にインクルードしようとすると処理が無視される．
 */
#ifndef __MATHAUX_H_INC__
#define __MATHAUX_H_INC__

/* 円周率の定義 */
#ifndef M_PI
#define M_PI 3.14159265
#endif

/* 値の大きい方を返す*/
double max(double arg0, double arg1);

/* 値の小さい方を返す */
double min(double arg0, double arg1);

/* (x0, y0)から(x1, y1)までの距離を計算して返す */
double length(double x0, double y0, double x1, double y1);

/* [0,1)の乱数を返す */
double rand_0_1();

#endif
