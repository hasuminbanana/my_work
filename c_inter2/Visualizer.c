/* Visualizer.c */
/*
 * このファイルでは可視化関連の関数を定義する．
 */
#include "Config.h"
#include "Visualizer.h"
#include "Car.h"
#include "Link.h"
#include "Drawer.h"
#include <stdlib.h>
#include <stdlib.h>
#include <math.h>
#include <autogl.h>
#include <unistd.h>

/*######################################################################*/
/*
 * グローバル変数の定義
 *
 * グローバル変数はプログラムのどこからでも参照可能
 */

int IsNodeIdShown = 0;
int IsLinkIdShown = 0;
int IsCarIdShown  = 0;

/*######################################################################*/
/* extern宣言 (別ファイルで定義された変数の読み込み) */
extern struct Car Cars[CAR_MAX];

extern struct Link Links[LINK_MAX];

extern int StartLinkIds[START_LINK_MAX];
extern int NumStartLinks;

extern double PresentTime;
extern double DeltaT;

/*######################################################################*/
/*
 * static関数の宣言 (このファイルでのみ有効) 
 * AutoGL関連のいくつかの関数はstaticでなければならない
 */

/* 再描画関数 */
static void redrawView();

/* グローバル関数の更新 */
static void updateVariables();

/* Animateボタン用関数*/
static void animateButtonCallback();

/* アイドルイベント用関数 */
static void idleEvent();

/* Drawボタン用関数 */
static void drawButtonCallback();

/* Quitボタン用関数 */
static void quitButtonCallback();

/*
 * static変数の宣言
 */
static int isNodeIdSelected = 0;
static int isLinkIdSelected = 0;
static int isCarIdSelected  = 0;
/*
  static long int sleep_usec  = 10000; 
*/
/*######################################################################*/
void visualize()
{
    /* ビューイングパラメータを設定する */
    AutoGL_SetViewSize(110);           /* ビューの大きさ */
    AutoGL_SetViewCenter(0, 0, 0);     /* 注視点 */
    AutoGL_SetViewDirection(0, 0, 1);  /* 視線方向 */
    AutoGL_SetViewUpVector(0, 1, 0);

    /* 再描画コールバック関数としてredrawViewを指定 */
    AutoGL_SetViewRedrawCallback(redrawView); 

    /* おまじない */
    AutoGL_SetMode2D(AUTOGL_MODE_2D_SCALE);  /* マウスで回転 */
    AutoGL_SetDefaultCallbackInMode2D(NULL);    
    AutoGL_EnableDragInMode2D();             /* ドラッグ有効 */
    AutoGL_EnableIdleEvent();
    AutoGL_SetPanelInMode2D();               /* 移動拡大縮小など */

    /* 論理変数の登録 : チェックボックス */
    AutoGL_AddBoolean(&isCarIdSelected, "isCarIdSelected");
    AutoGL_SetLabel("Car ID");
    AutoGL_AddBoolean(&isNodeIdSelected, "isNodeIdSelected");
    AutoGL_SetLabel("Node ID");
    AutoGL_AddBoolean(&isLinkIdSelected, "isLinkIdSelected");
    AutoGL_SetLabel("Link ID");

    /* Drawボタンをつける */
    AutoGL_AddCallback(drawButtonCallback, "drawButtonCallback");
    AutoGL_SetLabel("Draw");

    /* Animateボタンをつける */
    AutoGL_AddCallback(animateButtonCallback, "animateButtonCallback");
    AutoGL_SetLabel("Animate");

    /* Quitボタンをつける */
    AutoGL_AddCallback(quitButtonCallback, "qiutButtonCallback");
    AutoGL_SetLabel("Quit");
}

/*######################################################################*/
void redrawView() 
{
    int i;

    /* グローバル変数の更新 */
    /*
     * AutoGLにはstatic変数しか読ませられない制約があるので，
     * 他のファイルからも参照するため別の変数に値を渡す必要がある
     */
    updateVariables();

    /* 道路の描画 */
    drawRoad();
  
    /* 車両の描画 */
    for(i=0; i<CAR_MAX; i++)
    {
        if (Cars[i].isUsed==1)
        {
            drawCar(i);
        }
    }
}

/*######################################################################*/
void animateButtonCallback(void) 
{
    static int idleEventIsOn = 0;

    if (idleEventIsOn)
    {
        /* アイドルイベント処理の無効化(アニメーションを止める) */
        AutoGL_SetIdleEventCallback(NULL);   
        idleEventIsOn = 0;
    }
    else
    {
        /* アイドルイベント処理コールバックの登録(アニメーションを始める) */
        AutoGL_SetIdleEventCallback(idleEvent);   
        idleEventIsOn = 1;
    }
}

/*######################################################################*/
void updateVariables()
{
    IsCarIdShown  = isCarIdSelected;
    IsNodeIdShown = isNodeIdSelected;
    IsLinkIdShown = isLinkIdSelected;
}

/*######################################################################*/
void idleEvent()
{
    int i;

    /*==================================================================*/
    /* 時刻ステップを進める処理 */
    PresentTime += DeltaT;

    /* 車両を発生させる */
    for (i=0; i<NumStartLinks; i++)
    {
        if (((float)rand())/RAND_MAX <= CAR_GENERATE_RATIO)
        {
            generateCar(StartLinkIds[i]);
        }
    }
    
    for (i=0; i<CAR_MAX; i++)
    {
        if (Cars[i].isUsed==0)
        {
            continue;
        }
        recognize(i);
        calcAccel(i);
        run(i);
    }
  
    /*==================================================================*/
    /* 再描画 */
    AutoGL_DrawView();

    if ((int)(PresentTime*10)%10 == 0)
    {
        printf("%d[sec]\n", (int)PresentTime);
    }

    /*
      usleep(sleep_usec);
    */
}

/*######################################################################*/
void drawButtonCallback()
{
    AutoGL_DrawView();
}

/*######################################################################*/
void quitButtonCallback()
{
    exit(EXIT_SUCCESS);
}
