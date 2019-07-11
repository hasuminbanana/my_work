/* Car.c */
/*
 * このファイルでは車両の挙動に関する関数を定義する
 */
#include "Config.h"
#include "Car.h"
#include "Node.h"
#include "Link.h"
#include "MathAux.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/*######################################################################*/
/* extern宣言 (別ファイルで定義された変数の読み込み) */
extern struct Car Cars[CAR_MAX];
extern int NumCars;

extern struct Node Nodes[NODE_MAX];
extern int NumNodes;

extern struct Link Links[LINK_MAX];
extern int NumLinks;

extern int StartLinkIds[START_LINK_MAX];
extern int NumStartLinks;

extern double DeltaT;

/*######################################################################*/
/* static関数の宣言 (このファイルでのみ有効) */

/* 番号thisIdの車両が番号thatIdの車両に道を譲るかどうか */
int isYielding(int thisId, int thatId);

/*######################################################################*/
/* 車両を初期化する */
void initCar()
{
    int i;

    for (i=0; i<CAR_MAX; i++)
    {
        Cars[i].isUsed = 0;
        Cars[i].canTransferLinks = 1;
    }
}

/*######################################################################*/
/* 番号linkIdのリンク始点に車両を発生させる */
void generateCar(int linkId)
{
    int i;
    int id = -1;

    /* 発生可能な車両IDを調べる */
    /*
     * isUsedフラグが立っていないIDが最小の車両
     */
    for (i=0; i<CAR_MAX; i++)
    {
        if (Cars[i].isUsed == 0)
        {
            id = i;
            break;
        }
    }
    if (id == -1)
    {
        return;
    }

    /* 車両の発生 */
    Cars[id].isUsed = 1;

    /* 車線の登録 */
    Cars[id].linkId = linkId;

    /* 車体色の設定 */
    Cars[id].r = 1.0;
    Cars[id].g = 0.0;
    Cars[id].b = 0.0;

    /* 次に走行する車線の決定 */
    if (Links[linkId].numNextLinks == 0)
    {
        Cars[id].nextLinkId = -1;
    }
    else
    {
        int r = rand() % Links[linkId].numNextLinks;
        if (r%2 == 1)
        {
            /* 次のリンクによって車体色を変更する */
            Cars[id].g = 0.5;
        }
        Cars[id].nextLinkId
            = Links[linkId].nextLinkIds[r];
    }

    /* 次の次に走行する車線の決定 */
    if (Cars[id].nextLinkId == -1
        || Links[Cars[id].nextLinkId].numNextLinks == 0)
    {
        Cars[id].next2LinkId = -1;
    }
    else
    {
        Cars[id].next2LinkId
            = Links[Cars[id].nextLinkId]
            .nextLinkIds[rand() % Links[Cars[id].nextLinkId].numNextLinks];
    }

    /* 車線の始点に配置 */
    Cars[id].distance = 0;

    /* 車両の速度 */
    Cars[id].speed    = 30.0 * 1000.0/3600;
    Cars[id].maxSpeed = 60.0 * 1000.0/3600; /* km/h -> m/s */
    Cars[id].accel = 0;
}

/*######################################################################*/
/* 番号idの車両について，前方の車や衝突しそうな車を認識する */
void recognize(int id)
{
    int i;
    int leadCarId = -1;
    double minDistance = INFINITY;

    /*==================================================================*/
    /* 先行車を取得する*/
    for (i=0; i<CAR_MAX; i++)
    {
        if (Cars[i].isUsed == 0)
        {
            continue;
        }
        /* 同じレーンの前方にいる車両 */
        if (Cars[i].linkId == Cars[id].linkId
            && Cars[i].distance > Cars[id].distance
            && Cars[i].distance - Cars[id].distance < minDistance)
        {
            leadCarId = i;
            minDistance = Cars[i].distance - Cars[id].distance;
        }
        /* 1つ先のレーンにいる車両 */
        else if (Cars[i].linkId == Cars[id].nextLinkId
                 && Cars[i].distance - Cars[id].distance
                 + Links[Cars[id].linkId].length
                 < minDistance)
        {
            leadCarId = i;
            minDistance
                = Cars[i].distance - Cars[id].distance
                + Links[Cars[id].linkId].length;
        }
        /* 2つ先のレーンにいる車両 */
        else if (Cars[i].linkId == Cars[id].next2LinkId
                 && Cars[i].distance - Cars[id].distance
                 + Links[Cars[id].linkId].length
                 + Links[Cars[id].nextLinkId].length
                 < minDistance)
        {
            leadCarId = i;
            minDistance
                = Cars[i].distance - Cars[id].distance
                + Links[Cars[id].linkId].length
                + Links[Cars[id].nextLinkId].length;
        }
        
    }

    Cars[id].leadCarId = leadCarId;
    Cars[id].distanceToLeadCar = minDistance;

    /*==================================================================*/
    /* 1つ先のレーンに交錯優先レーンがある場合，レーンを移れるかどうか判定する */
    Cars[id].canTransferLinks = 1;

    if (Links[Cars[id].nextLinkId].numPcolLinks == 0
        && Links[Cars[id].nextLinkId].numIcolLinks == 0)
    {
        /* 交錯レーンがなければ何もしない*/
        return;
    }

    for (i=0; i<Links[Cars[id].nextLinkId].numPcolLinks; i++)
    {
        int j;

        /* 交錯レーンを調査する */
        if (headCarId(Links[Cars[id].nextLinkId].pcolLinkIds[i]) != -1)
        {
            Cars[id].canTransferLinks = 0;
            break;
        }

        /* 交錯レーンの上流を調査する */
        for (j=0;
             j<Links[Links[Cars[id].nextLinkId].pcolLinkIds[i]].numPrevLinks;
             j++)
        {
            int pcolCarId
                = headCarId
                (Links[Links[Cars[id].nextLinkId].pcolLinkIds[i]].prevLinkIds[j]);

            if (pcolCarId!=-1
                && isYielding(id, pcolCarId) == 1)
            {
                Cars[id].canTransferLinks = 0;
                /* printf("Car:%d has pcolCar\n", id);*/
                break;
            }
        }
    }
    if (Cars[id].canTransferLinks == 0)
    {
        return;
    }

    for (i=0; i<Links[Cars[id].nextLinkId].numIcolLinks; i++)
    {
        /* 相手が交錯非優先レーンの場合，交錯レーンのみを調査する */
        if (headCarId(Links[Cars[id].nextLinkId].icolLinkIds[i]) != -1)
        {
            Cars[id].canTransferLinks = 0;
            break;
        }
    }
}

/*######################################################################*/
/* 番号idの車両の加速度を計算する */
void calcAccel(int id)
{
    /*
     * 追従(自由走行含む)用の加速度と交錯回避用の加速度を別々に出し，
     * 小さい値を採用する
     */
    double aCandForFollowing = INFINITY;
    double aCandForYielding  = INFINITY;

    double idm_a     = 1.0; /* [m/s^2] */
    double idm_b     = 3.0; /* [m/s^2] */
    double idm_delta = 4.0;
    double idm_T     = 1.5; /* [s] */
    double idm_s0    = 2.0; /* [m] */ 
    double idm_sa;
    double idm_s;

    /*==================================================================*/
    /* 通常の追従モデル */
    /*
     * ここではIntelligent Driver Modelを用いる．
     */
    if (Cars[id].leadCarId == -1)
    {
        /* 先行車なし：制限速度まで加速 */
        aCandForFollowing
            = idm_a
            * (1 - pow((Cars[id].speed/Cars[id].maxSpeed), idm_delta));
    }
    else
    {
        /* 先行車あり：追従 */
        idm_sa
            = Cars[id].distanceToLeadCar - 5.0;
        idm_s
            = idm_s0
            + max(0,
                  Cars[id].speed * idm_T
                  + (Cars[id].speed
                     * (Cars[id].speed - Cars[Cars[id].leadCarId].speed))
                  / (2 * sqrt (idm_a * idm_b)));
        aCandForFollowing
            = idm_a
            * (1
               - pow((Cars[id].speed/Cars[id].maxSpeed), idm_delta)
               - pow(idm_s/idm_sa, 2.0));
    }

    /*==================================================================*/
    /* レーン終端での停止 */
    if (Cars[id].canTransferLinks == 0)
    {
        idm_sa
            = Links[Cars[id].linkId].length - Cars[id].distance - 2.5;
        idm_s
            = idm_s0
            + Cars[id].speed * idm_T
            + (Cars[id].speed * Cars[id].speed)
            / (2 * sqrt (idm_a * idm_b));
        aCandForYielding
            = idm_a
            * (1
               - pow((Cars[id].speed/Cars[id].maxSpeed), idm_delta)
               - pow(idm_s/idm_sa, 2.0));
    }

    /*==================================================================*/
    /* 小さい方の加速度候補を採用する */
    if (aCandForFollowing < aCandForYielding)
    {
        Cars[id].accel = aCandForFollowing;
    }
    else
    {
        Cars[id].accel = aCandForYielding;
    }
}

/*######################################################################*/
/* 番号idの車両を移動させる */
void run(int id)
{
    /* 位置の更新 */
    Cars[id].distance
        += max(0,
               Cars[id].speed * DeltaT
               + 0.5 * Cars[id].accel * DeltaT * DeltaT);

    /* 速度の更新 */
    Cars[id].speed += Cars[id].accel * DeltaT;
    if (Cars[id].speed > Cars[id].maxSpeed)
    {
        Cars[id].speed = Cars[id].maxSpeed;
    }
    else if (Cars[id].speed < 0)
    {
        Cars[id].speed = 0;
    }

    /* リンクの終点を超えたら次のリンクへ */
    if (Cars[id].distance >= Links[Cars[id].linkId].length)
    {
        if (Cars[id].nextLinkId==-1)
        {
            /* 次の車線がないので消去 */
            Cars[id].isUsed = 0;
        }
        else
        {
            /* 次の車線に移る */
            Cars[id].distance -= Links[Cars[id].linkId].length;
            Cars[id].linkId = Cars[id].nextLinkId;

            /* 更に次の車線を決定する */
            if (Links[Cars[id].linkId].numNextLinks == 0)
            {
                Cars[id].nextLinkId = -1;
            }
            else
            {
                Cars[id].nextLinkId
                    = Links[Cars[id].linkId]
                    .nextLinkIds[rand() % Links[Cars[id].linkId].numNextLinks];
            }

            /* 更に次の次の車線を決定する */
            if (Cars[id].nextLinkId == -1
                || Links[Cars[id].nextLinkId].numNextLinks == 0)
            {
                Cars[id].next2LinkId = -1;
            }
            else
            {
                Cars[id].next2LinkId
                    = Links[Cars[id].nextLinkId]
                    .nextLinkIds[rand() % Links[Cars[id].nextLinkId].numNextLinks];
            }
        }
    }

    Cars[id].x = Nodes[Links[Cars[id].linkId].beginNodeId].x
        + Links[Cars[id].linkId].dvecX
        * Cars[id].distance;

    Cars[id].y = Nodes[Links[Cars[id].linkId].beginNodeId].y
        + Links[Cars[id].linkId].dvecY
        * Cars[id].distance;
}

/*######################################################################*/
/* 番号thisIdの車両が番号thatIdの車両に道を譲るかどうか */
int isYielding(int thisId, int thatId)
{
    double thisTime, thatTime;
    int i;
    int hasCollision = 0;

    /* 互いの動線が交錯するか調べる */
    for (i=0;
         i<Links[Cars[thisId].nextLinkId].numPcolLinks;
         i++)
    {
        if (Links[Cars[thisId].nextLinkId].pcolLinkIds[i]
            == Cars[thatId].nextLinkId)
        {
            hasCollision = 1;
            break;
        }
    }
    if (hasCollision == 0)
    {
        /* 動線が交錯しない場合は譲らない */
        return 0;
    }

    /*
     * 自分と相手の車両が交差点に到達するまでの時間を予測する
     * (ただし加減速を前提としない)
     */

    /* 自分が交差点に到達するまでの時間 */
    if (Cars[thisId].speed < 1.0*1000/3600) /* [km/h]->[m/s] */
    {
        thisTime
            = (Links[Cars[thisId].linkId].length - Cars[thisId].distance)
            / (1.0*1000/3600);
    }
    else
    {
        thisTime
            = (Links[Cars[thisId].linkId].length - Cars[thisId].distance)
            / Cars[thisId].speed;
    }

    /* 相手が到達するまでの時間 */
    if (fabs(Cars[thatId].speed) < 1.0e-6)
    {
        return 0;
    }
    else
    {
        thatTime
            = (Links[Cars[thatId].linkId].length - Cars[thatId].distance)
            / Cars[thatId].speed;
    }

    /* 自分が先に交差点に到着できない場合は道を譲る */
    if (thisTime + 3.0 > thatTime)
    {
        return 1;
    }
    return 0;
}


