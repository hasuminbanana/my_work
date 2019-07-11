/* Drawer.c */
/*
 * このファイルでは可視化関連の関数を定義する．
 */
#include "Drawer.h"
#include "Config.h"
#include "Car.h"
#include "Link.h"
#include "Node.h"
#include "MathAux.h"
#include <math.h>
#include <autogl.h>

/*######################################################################*/
/* extern宣言 (別ファイルで定義された変数の読み込み) */
extern struct Car Cars[CAR_MAX];
extern int NumCars;

extern struct Node Nodes[NODE_MAX];
extern int NumNodes;

extern struct Link Links[LINK_MAX];
extern int NumLinks;

extern double PresentTime;
extern double DeltaT;

extern int IsNodeIdShown;
extern int IsLinkIdShown;
extern int IsCarIdShown;

/*######################################################################*/
/*
 * static関数の宣言 (このファイルでのみ有効) 
 * AutoGL関連のいくつかの関数はstaticでなければならない
 */

/* ノードを描画する */
static void drawNode(int id, double size);

/* リンクを描画する */
static void drawLink(int id, double size);

/*######################################################################*/
/* 道路を描画する */
void drawRoad()
{
    int i;

    /* 画面の表示サイズに応じて線や点のサイズを変えるために使用 */
    /* これにより，拡大縮小しても線幅が一定になる */
    double size = AutoGL_GetViewSize();

    for (i=0; i<NODE_MAX; i++)
    {
        drawNode(i, size * 0.005);
    }

    for (i=0; i<LINK_MAX; i++)
    {
        drawLink(i, size * 0.02);
    }
}

/*######################################################################*/
/* ノードを描画する */
void drawNode(int id, double size)
{
    if (id<0 || id>NODE_MAX || Nodes[id].isUsed != 1)
    {
        return;
    }

    AutoGL_SetColor(1, 1, 1);
    AutoGL_DrawCircle3D(Nodes[id].x, Nodes[id].y, 0,
                        0, 0, 1, size, 3);

    if (IsNodeIdShown)
    {
        char idStr[8];
        sprintf(idStr, "%d", id);

        AutoGL_SetColor(0, 0.5, 1);
        AutoGL_DrawString(Nodes[id].x + size,
                          Nodes[id].y + size,
                          5, idStr);
    }
}

/*######################################################################*/
/* リンクを描画する */
void drawLink(int id, double size)
{
    if (id<0 || id>LINK_MAX || Links[id].isUsed != 1)
    {
        return;
    }

    AutoGL_SetColor(1, 1, 1);
    AutoGL_DrawArrow2D(Nodes[Links[id].beginNodeId].x,
                       Nodes[Links[id].beginNodeId].y,
                       Nodes[Links[id].endNodeId].x,
                       Nodes[Links[id].endNodeId].y,
                       0, size/Links[id].length);

    if (IsLinkIdShown)
    {
        char idStr[8];
        sprintf(idStr, "%d", id);

        AutoGL_SetColor(0, 1, 0.5);
        AutoGL_DrawString((Nodes[Links[id].beginNodeId].x
                           + Nodes[Links[id].endNodeId].x) / 2.0 + size,
                          (Nodes[Links[id].beginNodeId].y
                           + Nodes[Links[id].endNodeId].y) / 2.0 + size,
                          5, idStr);
    }
}

/*######################################################################*/
/* 車両を描画する */
void drawCar(int id)
{
    double xCenter = Cars[id].x;
    double yCenter = Cars[id].y;

    /* ATTENTION!! */
    /*
     * 円で表示．四角形にしたければリンクの方向ベクトルを利用する．
     */
    AutoGL_SetColor(Cars[id].r, Cars[id].g, Cars[id].b);
    AutoGL_DrawCircle3D(xCenter, yCenter, 0.5,
                        0, 0, 1, 1.5, 3);

    if (IsCarIdShown)
    {
        char idStr[8];
        sprintf(idStr, "%d", id);

        AutoGL_SetColor(1, 0, 0.5);
        AutoGL_DrawString(xCenter+1.5, yCenter+1.5, 5, idStr);        
    }
}
