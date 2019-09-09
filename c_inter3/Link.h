/* Link.h */
/*
 * このファイルでは車線の情報を集約するLinkという構造体を宣言する
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
#ifndef __LINK_H_INC__
#define __LINK_H_INC__

#include "Config.h"

/* 構造体Linkの宣言 */
struct Link
{
    int isUsed;

    int beginNodeId; /* 始点ノードのID */
    int endNodeId;   /* 終点ノードのID */

    int prevLinkIds[PREV_LINK_MAX]; /* 上流リンクID */
    int numPrevLinks;               /* 上流リンクの個数 */

    int nextLinkIds[NEXT_LINK_MAX]; /* 下流リンクID */
    int numNextLinks;               /* 下流リンクの個数 */

    int pcolLinkIds[PCOL_LINK_MAX]; /* 交錯優先リンクのID */
    int numPcolLinks;               /* 交錯優先リンクの個数 */
                                    /* 相手が優先 */

    int icolLinkIds[ICOL_LINK_MAX]; /* 交錯非優先リンクのID */
    int numIcolLinks;               /* 交錯非優先リンクの個数 */
                                    /* 自分が優先 */

    double length;                  /* 始点から終点までの距離   */

    double dvecX;    /* 方向ベクトルのx成分 */
    double dvecY;    /* 方向ベクトルのy成分 */
    double theta;    /* x軸正の方向となす角(-M_PI〜M_PI)[rad] */
};

/* 番号idのリンクを初期化する */
void initLink(int id);

/* 番号id，始点beginNodeId, 終点endNodeIdにひとつのリンクを作成する */
void createLink(int id, int beginNodeId, int endNodeId);

/* 番号idのリンクに，前のリンクの番号prevLinkIdを設定する */
void setPrevLink(int id, int prevLinkId);

/* 番号idのリンクに，次のリンクの番号nextLinkIdを設定する */
void setNextLink(int id, int nextLinkId);

/* 番号idのリンクに，交錯優先リンクの番号colpLinkIdを設定する */
void setPcolLink(int id, int pcolLinkId);

/* 番号idのリンクに，交錯非優先リンクの番号coliLinkIdを設定する */
void setIcolLink(int id, int icolLinkId);

/* 番号idのリンクに存在する車両のうち先頭の番号を返す */
int headCarId(int id);

/* 番号idのリンクの情報を出力する */
void printLink(int id);

#endif
