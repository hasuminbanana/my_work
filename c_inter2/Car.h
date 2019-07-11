/* Car.h */
/*
 * このファイルでは車両の情報を集約するCarという構造体と，
 * 車両に関する関数を宣言する
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
#ifndef __CAR_H_INC__
#define __CAR_H_INC__ 

/* 構造体Carの宣言 */
struct Car
{
    int isUsed;

    int linkId;      /* 現在走行中の車線ID */
    int nextLinkId;  /* 次に走行する車線ID */
    int next2LinkId; /* 次の次に走行する車線ID */
    double distance; /* 現在走行中の車線の始点からの距離 */

    double speed;    /* 速度[m/s]     */
    double maxSpeed; /* 最高速度[m/s] */
    double accel;    /* 加速度[m/s^2] */

    int leadCarId;            /* 先行車のID*/
    double distanceToLeadCar; /* 先行車までの距離 */

    int canTransferLinks;     /* リンク終端から次のリンクに移れるか */

    double x; /* x座標 */
    double y; /* y座標 */

    double r; /* 車体色の赤成分 */
    double g; /* 車体色の緑成分 */
    double b; /* 車体色の青成分 */
};

/*======================================================================*/
/* 車両の制御に関するもの */

/* 車両を初期化する */
void initCar();

/* 番号linkIdのリンク始点に車両を発生させる */
void generateCar(int linkId);

/*======================================================================*/
/* 車両の挙動に関するもの */

/* 番号idの車両について，前方の車や衝突しそうな車を認識する */
void recognize(int id);

/* 番号idの車両の加速度を計算する */
void calcAccel(int id);

/* 番号idの車両を移動させる */
void run(int id);

#endif
