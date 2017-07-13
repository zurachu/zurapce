#if !defined( ZURAPCE_LDIRECT_H )
#define ZURAPCE_LDIRECT_H

/** @file
	16�K���`��֘A.
	�t���̎c���𗘗p���āA5���̉摜�������ɐ؂�ւ��A16�K���̉摜��\�����Ă���悤�ɂ݂������܂��B
	����ɁA�ʏ��4�K���`���ʃ��C���[�Ƃ��ďd�ˍ��킹�ĕ\���ł��܂��B
	
	�������d���ł����A�R�[�h������ RAM �ɔz�u���邱�Ƃō������ł��܂��B
	project.mak �� USE_LDIRECT_ON_FRAM := true ��L���ɂ��āA
	pceAppInit() �̐擪�� FramObject_Init() ���Ăяo���ĉ������B
	
	@see @ref pgx_format

	@author zurachu
*/

#include <piece.h>

#define LDIRECT_PAGE_NUM	5 ///< �؂�ւ��y�[�W��
#define LDIRECT_COLOR_NUM	16 ///< �K�����iLDIRECT_PAGE_NUM * 3 + 1�j

/** @page pgx_format 16�K���摜�t�H�[�}�b�g
	PIECE_BMP �̊g���t�H�[�}�b�g�ł��B

	�\���̂� PIECE_BMP �����̂܂܎g�����߁A
	�ʏ�� PIECE_BMP �Ɠ��l�� PieceBmp_Construct() �ŏ������ł��܂��B
	���f�[�^�Ƃ��ẮA 16�K�� BMP �� pgd16cnv �ŕϊ��������̂�n���Ă��������B
	https://github.com/zurachu/pgd16cnv

@code
	typedef struct {
		DWORD head; // ���ʎq 'PBEX'�i�����A�v���̌듮��h�~�j
		DWORD fsize; // �t�@�C����
		BYTE bpp; // bit �[�x (4)
		BYTE mask; // �}�X�N�� bit �[�x (1)
		int w; // X �� 8 �s�N�Z���P�ʌ���
		int h; // Y ����
		DWORD buf_size; // BMP �T�C�Y = ( bpp + mask ) * w * h / 8
	} PBMP_FILEHEADER;
	
	typedef struct {
		PBMP_FILEHEADER	header;
		BYTE *buf; // 4 BIT 1 �s�N�Z��
		BYTE *mask; // 1 BIT 1 �s�N�Z��
	} PIECE_BMP;
@endcode
*/

/** @page support_capturing Windows���L���v�`���\�t�g��16�K���Ή�
	Windows����P/ECE�̉�ʂ��L���v�`������\�t�g�ŁA16�K���`��ɑΉ���������@�ł��B

	- P/ECE monitor�insawa���j http://www.piece-me.org/

	  ���s��A�E�N���b�N���j���[�Łu16�K�����[�h�v�Ƀ`�F�b�N�����Ă��������B

	- pceCaps�i�܂��ׂЂ낵���j �����N�؂�

	  readme.txt �̒ʂ�A usbcapt.h ���C���N���[�h���āA
	  Ldirect_Init() �̑O�� usbCaptureInit() ���A
	  Ldirect_Exit() �̌�� usbCaptureRelease() ���Ăяo���Ă��������B

	  pcecaps.exe ���s��́A File��option��MultiColor�ňȉ��̐ݒ���s���Ă��������B
	  - 1�t���[���̎��ԊԊu: pceAppSetProcPeriod() �̒l�i���܂��\������Ȃ��ꍇ�͒������K�v�����j
	  - �t���[������: 5
*/

/**
	������.
	���z��ʃo�b�t�@�A16�K���p�`��o�b�t�@�A4�K���p�`��o�b�t�@�A
	LCD �_�C���N�g�]���p�o�b�t�@�i5���j���m�ۂ��܂��B
	�܂��A pceLCDSetBuffer() ���t�b�N���āA�߂�l�ŉ��z��ʃo�b�t�@�̑����
	4�K���p�`��o�b�t�@�̃A�h���X��Ԃ��悤�ɂ��܂��B
	�i����ɂ��A������4�K���`��֐���4�K���p�`��o�b�t�@�ɕ`�����܂�܂��j
	16�K���p�`��o�b�t�@�̃A�h���X�� Ldirect_Buffer() �Ŏ��o���܂��B
	���z��ʃo�b�t�@�̃A�h���X�͎��o���܂��񂪁A���ڃA�N�Z�X����K�v�͂Ȃ��͂��ł��B
	@attention
		Lcd_Init() �Ƃ͔r���I�ɂȂ��Ă���̂ŁA�������Ăяo���Ȃ��悤�ɂ��Ă��������B
	@retval 1 �o�b�t�@�m�ې���
	@retval 0 �o�b�t�@�m�ێ��s
*/
int Ldirect_Init( void );

/**
	�I��������.
	pceLCDSetBuffer() �̃t�b�N�����Ƃɖ߂��܂��B
*/
void Ldirect_Exit( void );

/**
	16�K���p�`��o�b�t�@�̃A�h���X���擾.
	�o�b�t�@�̍\���́A�ʏ��4�K���p���z��ʃo�b�t�@�Ɠ����ł��B
	1�h�b�g1�o�C�g�i����4�r�b�g�̂ݎg�p��16�F�j
	��128�h�b�g�~�c88�h�b�g = 11246�o�C�g
@verbatim
Ldirect_Buffer()��+---------... ...------------+
                  |                            |
                  |                            |  ��
                  .                            .
                  .                            .  Y 88 �h�b�g
                  .                            .
                  |                            |  ��
                  |                            |
                  +---------... ...------------+
                         �� X 128 �h�b�g��
@endverbatim
*/
BYTE* Ldirect_Buffer( void );

/**
	LCD �_�C���N�g�]���p�o�b�t�@���X�V���܂�.
	���ۂɂ́A�����ōX�V�t���O�𗧂Ă邾���ŁA
	Ldirect_Trans() ��16�K���p�`��o�b�t�@��4�K���p�`��o�b�t�@�̓��e��
	���z��ʃo�b�t�@�ɏd�˕`�����A���z��ʃo�b�t�@��1�t���[����1������
	LCD �_�C���N�g�]���p�o�b�t�@�ɓ]�����܂��B
*/
void Ldirect_Update( void );

/**
	Ldirect_Trans() ��4�K���p�`��o�b�t�@�̓��e�����z��ʃo�b�t�@��
	�d�˕`�����邩�ǂ�����ݒ肵�܂�.
	@param visible �t���O
*/
void Ldirect_VBuffView( BOOL visible );

/**
	LCD �_�C���N�g�]��.
	LCD �_�C���N�g�]���p�o�b�t�@��؂�ւ��āA LCD �o�b�t�@�ɓ]�����܂��B
	���t���[���Ăяo���ĉ������B
	Ldirect_Update() ���Ăяo����Ă���܂����z��ʃo�b�t�@���X�V����Ă��Ȃ��ꍇ�A
	16�K���p�`��o�b�t�@��4�K���p�`��o�b�t�@�̓��e�����z��ʂɏd�˕`�����܂��B
	�܂��A���z��ʃo�b�t�@�̓��e�����̃t���[���Ŏg�p����� LCD �_�C���N�g�]���p
	�o�b�t�@�ɔ��f����Ă��Ȃ��ꍇ�A���f���Ă���]�����܂��B
*/
void Ldirect_Trans( void );

/**
	4�K���p�`��o�b�t�@�𓧉ߐF�iCOLOR_MASK�j�œh��ׂ�.
	@param x ���� X ���W
	@param y ���� Y ���W
	@param w ��
	@param h ����
*/
void Ldirect_VBuffClear( int x, int y, int w, int h );

/**
	16�K���p�`��o�b�t�@��16�K���摜��`��.
	@param p 16�K�� PIECE_BMP �ւ̃|�C���^
	@param dx �`��捶�� X ���W
	@param dy �`��捶�� Y ���W
	@param sx �`�挳���� X ���W
	@param sy �`�挳���� Y ���W
	@param width ��
	@param height ����
	@retval 0 �`��Ȃ�
	@retval 1 �`�抮��
	
	@see @ref pgx_format
*/
int Ldirect_DrawObject( PIECE_BMP const* p, int dx, int dy, int sx, int sy
						, int width, int height );

/**
	16�K���p�`��o�b�t�@�ɓ_��`��.
	@param color �F
	@param x X���W
	@param y Y���W
*/
void Ldirect_Point( BYTE color, int const x, int const y );

/**
	16�K���p�`��o�b�t�@�ɋ�`��`��.
	@param color �F
	@param x ����X���W
	@param y ����Y���W
	@param width ��
	@param height ����
*/
void Ldirect_Paint( BYTE color, int x, int y, int width, int height );

#endif	// !defined( ZURAPCE_LDIRECT_H )
