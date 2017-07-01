#if !defined( ZURAPCE_PIECE_BMP_H )
#define ZURAPCE_PIECE_BMP_H

/** @file
	PIECE_BMP 関連.
	@author zurachu
*/

#include <piece.h> // PIECE_BMP

/**
	PIECE_BMP の初期化.
	BMP コンバータで変換したデータを、PIECE_BMP に登録します。
	@param p PIECE_BMP 構造体へのポインタ
	@param source 元データ
*/
void PieceBmp_Construct( PIECE_BMP* p, unsigned char* source );

/**
	PIECE_BMP の描画.
	@param p PIECE_BMP 構造体へのポインタ
	@param dx 描画先左上 X 座標
	@param dy 描画先左上 Y 座標
	@param sx 描画元左上 X 座標
	@param sy 描画元左上 Y 座標
	@param width 幅
	@param height 高さ
	@param param パラメータ
	@return pceLCDDrawObject() の戻り値（0:描画なし, 1:描画完了）
*/
int PieceBmp_Draw( PIECE_BMP* p, int dx, int dy, int sx, int sy
				, int width, int height, int param );

#endif // !defined( ZURAPCE_PIECE_BMP_H )
