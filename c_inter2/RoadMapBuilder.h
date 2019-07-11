/* RoadMapBuilder.h */
/*
 * このファイルでは道路構造を作成するための関数を宣言する
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
#ifndef __ROADMAP_BUILDER_H_INC__
#define __ROADMAP_BUILDER_H_INC__ 

/* ネットワークを初期化する */
void buildNetwork();

/* ノード（車線の結節点）を作成する */
void buildNodes();

/* リンク（車線）を作成する */
void buildLinks();

#endif
