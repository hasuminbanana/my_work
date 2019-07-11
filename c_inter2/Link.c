/* Link.c*/

/*
 * このファイルではリンク(車線)に関する関数を定義する
 */
#include "Config.h"
#include "Node.h"
#include "Link.h"
#include "Car.h"
#include "MathAux.h"
#include <stdio.h>
#include <math.h>

/*######################################################################*/
/* extern宣言 (別ファイルで定義された変数の読み込み) */
extern struct Car Cars[CAR_MAX];
extern int NumCars;

extern struct Node Nodes[NODE_MAX];
extern int NumNodes;

extern struct Link Links[LINK_MAX];
extern int NumLinks;

/*######################################################################*/
/* 番号idのリンクを初期化する */
void initLink(int id)
{
    int i;

    if (id<0 || id>=LINK_MAX)
    {
        printf("initLink: invalid id\n");
        return;
    }

    Links[id].isUsed = 0;

    Links[id].beginNodeId = -1;
    Links[id].endNodeId   = -1;

    for (i=0; i<PREV_LINK_MAX; i++)
    {
        Links[id].prevLinkIds[i] = -1;
    }
    Links[id].numPrevLinks = 0;

    for (i=0; i<NEXT_LINK_MAX; i++)
    {
        Links[id].nextLinkIds[i] = -1;
    }
    Links[id].numNextLinks = 0;

    for (i=0; i<PCOL_LINK_MAX; i++)
    {
        Links[id].pcolLinkIds[i] = -1;
    }
    Links[id].numPcolLinks = 0;

    for (i=0; i<ICOL_LINK_MAX; i++)
    {
        Links[id].icolLinkIds[i] = -1;
    }
    Links[id].numIcolLinks = 0;
}

/*######################################################################*/
/* 番号id，始点beginNodeId, 終点endNodeIdにひとつのリンクを作成する */
void createLink(int id, int beginNodeId, int endNodeId)
{
    if (id<0 || id>=LINK_MAX)
    {
        printf("createLink: invalid id\n");
        return;
    }
    if (Nodes[beginNodeId].isUsed==0
        || Nodes[endNodeId].isUsed==0)
    {
        printf("createLink: invalid node\n");
        return;
    }
    if (Links[id].isUsed==1)
    {
        printf("createLink: duplication\n");
        return;
    }
   
    Links[id].isUsed = 1;
    Links[id].beginNodeId = beginNodeId;
    Links[id].endNodeId   = endNodeId;

    Links[id].length
        = sqrt((Nodes[Links[id].endNodeId].x
                - Nodes[Links[id].beginNodeId].x)
               * (Nodes[Links[id].endNodeId].x
                  - Nodes[Links[id].beginNodeId].x)
               + (Nodes[Links[id].endNodeId].y
                  - Nodes[Links[id].beginNodeId].y)
               * (Nodes[Links[id].endNodeId].y
                  - Nodes[Links[id].beginNodeId].y));

    Links[id].dvecX
        = (Nodes[Links[id].endNodeId].x
           - Nodes[Links[id].beginNodeId].x)
        / Links[id].length;

    Links[id].dvecY
        = (Nodes[Links[id].endNodeId].y
           - Nodes[Links[id].beginNodeId].y)
        / Links[id].length;

    Links[id].theta
        = atan2(Links[id].dvecY, Links[id].dvecX);
}

/*######################################################################*/
/* 番号idのリンクに，前のリンクの番号prevLinkIdを設定する */
void setPrevLink(int id, int prevLinkId)
{
    if (id<0
        || id>=LINK_MAX
        || prevLinkId<0
        || prevLinkId>=LINK_MAX)
    {
        printf("setPrevLink: invalid id\n");
        return;
    }
    if (Links[id].isUsed==0
        || Links[prevLinkId].isUsed==0)
    {
        printf("setPrevLink: not used\n");
        return;
    }
    if (Links[id].numPrevLinks>=PREV_LINK_MAX)
    {
        printf("setPrevLink: too many prevLink\n");
        return;
    }

    Links[id].prevLinkIds[Links[id].numPrevLinks] = prevLinkId;
    Links[id].numPrevLinks++;
}

/*######################################################################*/
/* 番号idのリンクに，次のリンクの番号nextLinkIdを設定する */
void setNextLink(int id, int nextLinkId)
{
    if (id<0
        || id>=LINK_MAX
        || nextLinkId<0
        || nextLinkId>=LINK_MAX)
    {
        printf("setNextLink: invalid id\n");
        return;
    }
    if (Links[id].isUsed==0
        || Links[nextLinkId].isUsed==0)
    {
        printf("setNextLink: not used\n");
        return;
    }
    if (Links[id].numNextLinks>=NEXT_LINK_MAX)
    {
        printf("setNextLink: too many nextLink\n");
        return;
    }

    Links[id].nextLinkIds[Links[id].numNextLinks] = nextLinkId;
    Links[id].numNextLinks++;
}

/*######################################################################*/
/* 番号idのリンクに，交錯優先リンクの番号colpLinkIdを設定する */
void setPcolLink(int id, int pcolLinkId)
{
    if (id<0
        || id>=LINK_MAX
        || pcolLinkId<0
        || pcolLinkId>=LINK_MAX)
    {
        printf("setPcolLink: invalid id\n");
        return;
    }
    if (Links[id].isUsed==0
        || Links[pcolLinkId].isUsed==0)
    {
        printf("setPcolLink: not used\n");
        return;
    }
    if (Links[id].numPcolLinks>=PCOL_LINK_MAX)
    {
        printf("setPcolLink: too many pcolLink\n");
        return;
    }

    Links[id].pcolLinkIds[Links[id].numPcolLinks] = pcolLinkId;
    Links[id].numPcolLinks++;
}

/*######################################################################*/
/* 番号idのリンクに，交錯非優先リンクの番号coliLinkIdを設定する */
void setIcolLink(int id, int icolLinkId)
{
    if (id<0
        || id>=LINK_MAX
        || icolLinkId<0
        || icolLinkId>=LINK_MAX)
    {
        printf("setIcolLink: invalid id\n");
        return;
    }
    if (Links[id].isUsed==0
        || Links[icolLinkId].isUsed==0)
    {
        printf("setIcolLink: not used\n");
        return;
    }
    if (Links[id].numIcolLinks>=ICOL_LINK_MAX)
    {
        printf("setIcolLink: too many icolLink\n");
        return;
    }
    Links[id].icolLinkIds[Links[id].numIcolLinks] = icolLinkId;
    Links[id].numIcolLinks++;
}

/*######################################################################*/
/* 番号idのリンクに存在する車両のうち先頭の番号を返す */
int headCarId(int id)
{
    int i;
    int headCarId = -1;
    double maxDistance = 0.0;

    for (i=0; i<CAR_MAX; i++)
    {
        if (Cars[i].isUsed == 0)
        {
            /* 走行していない車両は無視する */
            continue;
        }
        if (Cars[i].linkId == id
            && Cars[i].distance > maxDistance )
        {
            headCarId = i;
            maxDistance = Cars[i].distance;
        }
    }
    return headCarId;
}

/*######################################################################*/
/* 番号idのリンクの情報を出力する */
void printLink(int id)
{
    int i;

    printf("Link:%2d/ Node:%2d -> Node:%2d\n",
           id,
           Links[id].beginNodeId,
           Links[id].endNodeId);
    if (Links[id].numPrevLinks>0)
    {
        printf("  PrevLink:");
        for (i=0; i<Links[id].numPrevLinks; i++)
        {
            printf(" %2d,", Links[id].prevLinkIds[i]);
        }
        printf("\n");
    }
    if (Links[id].numNextLinks>0)
    {
        printf("  NextLink:");
        for (i=0; i<Links[id].numNextLinks; i++)
        {
            printf(" %2d,", Links[id].nextLinkIds[i]);
        }
        printf("\n");
    }
    if (Links[id].numPcolLinks>0)
    {
        printf("  PcolLink:");
        for (i=0; i<Links[id].numPcolLinks; i++)
        {
            printf(" %2d,", Links[id].pcolLinkIds[i]);
        }
        printf("\n");
    }
    if (Links[id].numIcolLinks>0)
    {
        printf("  IcolLink:");
        for (i=0; i<Links[id].numIcolLinks; i++)
        {
            printf(" %2d,", Links[id].icolLinkIds[i]);
        }
        printf("\n");
    }
}
