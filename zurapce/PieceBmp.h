#if !defined( ZURAPCE_PIECE_BMP_H )
#define ZURAPCE_PIECE_BMP_H

/** @file
	PIECE_BMP �֘A.
	@author zurachu
*/

#include <piece.h> // PIECE_BMP

/**
	PIECE_BMP �̏�����.
	BMP �R���o�[�^�ŕϊ������f�[�^���APIECE_BMP �ɓo�^���܂��B
	@param p PIECE_BMP �\���̂ւ̃|�C���^
	@param source ���f�[�^
*/
void PieceBmp_Construct( PIECE_BMP* p, unsigned char* source );

/**
	PIECE_BMP �̕`��.
	@param p PIECE_BMP �\���̂ւ̃|�C���^
	@param dx �`��捶�� X ���W
	@param dy �`��捶�� Y ���W
	@param sx �`�挳���� X ���W
	@param sy �`�挳���� Y ���W
	@param width ��
	@param height ����
	@param param �p�����[�^
	@return pceLCDDrawObject() �̖߂�l�i0:�`��Ȃ�, 1:�`�抮���j
*/
int PieceBmp_Draw( PIECE_BMP* p, int dx, int dy, int sx, int sy
				, int width, int height, int param );

#endif // !defined( ZURAPCE_PIECE_BMP_H )
