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

/*######################################################################*/
/* extern宣言 (別ファイルで定義された変数の読み込み) */
extern struct Node Nodes[NODE_MAX];
extern int NumNodes;

extern struct Link Links[LINK_MAX];
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
    createNode( 1, -100,   -2);
    createNode( 2,   -2, -150);
    createNode( 3,    2, -150);
    createNode( 5,  100,    2);
    createNode( 6,    2,  150);
    createNode( 7,   -2,  150);
    createNode( 9,   -4,   -2);
    createNode(10,   -2,   -4);
    createNode(11,    2,   -4);
    createNode(13,    4,    2);
    createNode(14,    2,    4);
    createNode(15,   -2,    4);

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

    /* リンクの接続関係 */
    setPrevLink( 4, 12);
    setPrevLink( 5, 11);
    setPrevLink( 6, 10);
    setPrevLink( 7,  9);
    setPrevLink( 9,  1);
    setPrevLink(10,  1);
    setPrevLink(11,  3);
    setPrevLink(12,  3);

    setNextLink( 1,  9);
    setNextLink( 1, 10);
    setNextLink( 3, 11);
    setNextLink( 3, 12);
    setNextLink( 9,  7);
    setNextLink(10,  6);
    setNextLink(11,  5);
    setNextLink(12,  4);

    /* リンクの交錯関係 */
    setPcolLink(10, 11);
    setPcolLink(12,  9);

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
