/* Node.h */
/*
 * このファイルでは車線の結節点を表すNodeという構造体を宣言する
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
#ifndef __NODE_H_INC__
#define __NODE_H_INC__ 

/* 構造体Nodeの宣言 */
struct Node
{
    int isUsed;
    double x; /* x座標 */
    double y; /* y座標 */
};

/* 番号idのノードを初期化する */
void initNode(int id);

/* 番号id，座標(x,y)にひとつのノードを作成する */
void createNode(int id, double x, double y);

/* 番号idのノードの情報を出力する */
void printNode(int id);

#endif
