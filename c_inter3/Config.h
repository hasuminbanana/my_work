/* Config.h */
/*
 * このファイルでは複数のファイルから読み込むマクロを定義する
 */
#ifndef __CONFIG_H_INC__
#define __CONFIG_H_INC__

/* 車両の配列サイズ*/
#define CAR_MAX  50

/* ノード (車線の結節点) の配列サイズ */
#define NODE_MAX 100

/* リンク (車線) の配列サイズ */
#define LINK_MAX 100

/* 車両が発生するリンクの配列サイズ */
#define START_LINK_MAX 16

/* 車両が消滅するリンクの配列サイズ */
#define GOAL_LINK_MAX 16

/* 隣接リンク，交錯リンクの配列サイズ */
#define PREV_LINK_MAX 4
#define NEXT_LINK_MAX 4
#define PCOL_LINK_MAX 4
#define ICOL_LINK_MAX 4

/* ネットワーク端点における車両発生率 */
#define CAR_GENERATE_RATIO 0.015

#endif
