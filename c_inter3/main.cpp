#include "Config.h"
#include "Car.h"
#include "Node.h"
#include "Link.h"
#include "RoadMapBuilder.h"
#include "Visualizer.h"
#include "MathAux.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <autogl.h>
#include <vector>       // ヘッダファイルインクルード
using namespace std;

/*######################################################################*/
/*
 * グローバル変数の定義
 *
 * グローバル変数はプログラムのどこからでも参照可能
 */


//struct Car Cars[CAR_MAX];         /* 車両の配列*/
//struct Node Nodes[NODE_MAX];      /* ノード (車線の結節点) の配列 */
//int NumNodes;                     /* 実際のノード数 */
//struct Link Links[LINK_MAX];      /* リンク (車線) の配列 */
//int NumLinks;                     /* 実際のリンク数 */


//これをvector型に書き換えると
std::vector<Car> Cars(CAR_MAX);
//int NumCars;
std::vector<Node> Nodes(NODE_MAX);
int NumNodes;
std::vector<Link> Links(LINK_MAX);
int NumLinks;

int StartLinkIds[START_LINK_MAX]; /* 車両が発生するリンク */
int NumStartLinks;                /* 車両が発生するリンクの数 */

double PresentTime = 0;           /* 現在時刻[sec] */
double DeltaT = 0.1;              /* 1stepあたりの時間[sec] */

/*######################################################################*/
/* メイン関数 */
void AutoGL_SetUp (int argc, char *argv[])
{
    /*==================================================================*/
    /* シミュレーションの準備 */

    /* ネットワークの構築 */
    buildNetwork();

    /* 乱数の種の設定 */
    /* 本来定数値を与えても意味がない (毎回同じ結果になる) のだが...*/
    srand(365);

    /*==================================================================*/
    /* ウインドウを立ち上げ，以降の操作をGUIに任せる */
    visualize();
}
