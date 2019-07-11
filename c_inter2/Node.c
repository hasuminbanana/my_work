#include "Node.h"
#include "Config.h"
#include <stdio.h>

/*######################################################################*/
/* extern宣言 (別ファイルで定義された変数の読み込み) */
extern struct Node Nodes[NODE_MAX];
extern int NumNodes;

/*######################################################################*/
/* 番号idのノードを初期化する */
void initNode(int id)
{
    if (id<0 || id>=NODE_MAX)
    {
        printf("initNode: invalid id\n");
        return;
    }

    Nodes[id].isUsed = 0;
    Nodes[id].x      = 0;
    Nodes[id].y      = 0;
}

/*######################################################################*/
/* 番号id，座標(x,y)にひとつのノードを作成する */
void createNode(int id, double x, double y)
{
    if (id<0 || id>=NODE_MAX)
    {
        printf("createNode: invalid id\n");
        return;
    }
    if (Nodes[id].isUsed==1)
    {
        printf("createNode: duplication\n");
        return;
    }

    Nodes[id].isUsed = 1;
    Nodes[id].x      = x;
    Nodes[id].y      = y;
}

/*######################################################################*/
/* 番号idのノードの情報を出力する */
void printNode(int id)
{
    printf("Node:%2d/ (%9.2f, %9.2f)\n", id, Nodes[id].x, Nodes[id].y);
}
