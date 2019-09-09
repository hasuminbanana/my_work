/* RoadMapBuilder.c*/

/*
 * このファイルでは道路の生成に関する関数を定義する
 */
#include "Config.h"
#include "Node.h"
#include "Link.h"
#include "RoadMapBuilder.h"
#include "MathAux.h"
#include <stdio.h>
#include <math.h>
#include <vector>       // ヘッダファイルインクルード
using namespace std;
/*######################################################################*/
/* extern宣言 (別ファイルで定義された変数の読み込み) */
/*
extern struct Node Nodes[NODE_MAX];
extern int NumNodes;

extern struct Link Links[LINK_MAX];
extern int NumLinks;
*/

extern std::vector<Node> Nodes;
extern int NumNodes;

extern std::vector<Link> Links;
extern int NumLinks;


extern int StartLinkIds[START_LINK_MAX];
extern int NumStartLinks;

/*######################################################################*/
/* static関数の宣言 (このファイルでのみ有効) */

/* 車両が発生するリンクを調べる */
static void searchStartLinks();

/*######################################################################*/
void buildNetwork()
{
    int i;

    /* 初期化 */
    for (i=0; i<NODE_MAX; i++)
    {
        initNode(i);
    }
    NumNodes = 0;
    for (i=0; i<LINK_MAX; i++)
    {
        initLink(i);
    }
    NumLinks = 0;
    for (i=0; i<START_LINK_MAX; i++)
    {
        StartLinkIds[i] = -1;
    }
    NumStartLinks = 0;

    /* ノード作成 */
    /*
     * 必ずノード->リンクの順
     */
    buildNodes();

    /* リンク作成 */
    buildLinks();

    /* 車両が発生するリンクの格納 */
    searchStartLinks();
}

/*######################################################################*/
/* ノード（車線の結節点）を作成する */
void buildNodes()
{
    int i;

    /* ATTENTION!! */
    /*
     * 直書き．汎用性を持たせるためにはファイルから入力する．
     */
    //交差点を大きくすする、５倍に
    createNode( 1, -100,   -10);
    createNode( 2,   -10, -150);
    createNode( 3,    10, -150);
    createNode( 5,  100,    10);
    createNode( 6,    10,  150);
    createNode( 7,   -10,  150);
    createNode( 9,   -20,   -10);
    createNode(10,   -10,   -20);
    createNode(11,    10,   -20);
    createNode(13,    20,    10);
    createNode(14,    10,    20);
    createNode(15,   -10,    20);

    //左折レーンの追加
    createNode(16,    10,    10);
    createNode(17,   -10,    -10);

    printf("*** Node Information ***\n");
    for (i=0; i<NODE_MAX; i++)
    {
        if (Nodes[i].isUsed!=1)
        {
            continue;
        }
        printNode(i);
    }
}

/*######################################################################*/
/* リンク（車線）を作成する */
void buildLinks()
{
    int i;

    /* ATTENTION!! */
    /*
     * 直書き．汎用性を持たせるためにはファイルから入力する．
     */

    /* 番号id，始点beginNodeId, 終点endNodeIdにひとつのリンクを作成する */
    createLink( 1,  2, 10);
    createLink( 3,  6, 14);
    createLink( 4,  9,  1);
    createLink( 5, 11,  3);
    createLink( 6, 13,  5);
    createLink( 7, 15,  7);
    createLink( 9, 10, 15);
    createLink(10, 10, 13);
    createLink(11, 14, 11);
    createLink(12, 14,  9);

    //左折レーンの追加
    createLink(13, 10, 9);
    createLink(14, 14, 13);

    /* リンクの接続関係 */
    /* 番号idのリンクに，前のリンクの番号prevLinkIdを設定する */
    //void setPrevLink(int id, int prevLinkId);
    setPrevLink( 4, 12);
    setPrevLink( 5, 11);
    setPrevLink( 6, 10);
    setPrevLink( 7,  9);
    setPrevLink( 9,  1);
    setPrevLink(10,  1);
    setPrevLink(11,  3);
    setPrevLink(12,  3);

    //左折レーン
    setPrevLink(13,  1);
    setPrevLink(4,  13);
    setPrevLink(14,  3);
    setPrevLink(6,  14);

    /* 番号idのリンクに，次のリンクの番号nextLinkIdを設定する */
    //void setNextLink(int id, int nextLinkId);
    setNextLink( 1,  9);
    setNextLink( 1, 10);
    setNextLink( 3, 11);
    setNextLink( 3, 12);
    setNextLink( 9,  7);
    setNextLink(10,  6);
    setNextLink(11,  5);
    setNextLink(12,  4);

    //左折レーン
    setNextLink( 13,  4);
    setNextLink(1,  13);
    setNextLink(14,  6);
    setNextLink(3,  14);

    /* リンクの交錯関係 */
    /* 番号idのリンクに，交錯優先リンクの番号colpLinkIdを設定する */
    //void setPcolLink(int id, int pcolLinkId);
    setPcolLink(10, 11);
    setPcolLink(12,  9);

    /* 番号idのリンクに，交錯非優先リンクの番号coliLinkIdを設定する */
    //void setIcolLink(int id, int icolLinkId);
    setIcolLink( 9, 12);
    setIcolLink(11, 10);

    printf("*** Link Information ***\n");
    for (i=0; i<LINK_MAX; i++)
    {
        if (Links[i].isUsed!=1)
        {
            continue;
        }
        printLink(i);
    }
}

/*######################################################################*/
/* 車両が発生するリンクの格納 */
static void searchStartLinks()
{
    int i;
    for (i=0; i<LINK_MAX; i++)
    {
        if (Links[i].isUsed==1
            && Links[i].numPrevLinks==0)
        {
            if (NumStartLinks>=START_LINK_MAX)
            {
                printf("too many startLink\n");
                break;
            }
            StartLinkIds[NumStartLinks] = i;
            NumStartLinks++;
        }
    }
}
