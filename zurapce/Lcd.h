#if !defined( ZURAPCE_LCD_H )
#define ZURAPCE_LCD_H

/** @file
	LCD �֘A.
	@author zurachu
*/

/**
	��ʊ֌W�̏�����.
	���z��ʃo�b�t�@���m�ۂ��A���̗̈���g�p����悤�ݒ肵�܂��B
	���z��ʃo�b�t�@�̃A�h���X�� pceLCDSetBuffer( INVALIDPTR ) �Ŏ��o���܂��B
	@attention
		Ldirect_Init() �Ƃ͔r���I�ɂȂ��Ă���̂ŁA�������Ăяo���Ȃ��悤�ɂ��Ă��������B
	@retval 1 ���z��ʃo�b�t�@�m�ې���
	@retval 0 ���z��ʃo�b�t�@�m�ێ��s
*/
int Lcd_Init( void );

/**
	LCD �X�V�t���O�𗧂Ă�.
*/
void Lcd_Update( void );

/**
	���z��ʃo�b�t�@�̓��e�� LCD �ɓ]��.
	LCD �X�V�t���O�������Ă��Ȃ���Γ]�����s��Ȃ��̂ŁA
	���t���[���Ăяo���ĉ������B
*/
void Lcd_Trans( void );

#endif // !defined( ZURAPCE_LCD_H )
