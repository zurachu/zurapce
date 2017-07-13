/** @file
	16�K���\������ pceFontPrintf() �̎���.
	Ldirect.c �Ɏ������Ă���ƃw�b�_�C���N���[�h���̊֌W��
	�ȉ��G���[���o�Ă��܂����߁A�ʃt�@�C���Œ�`�B
	C:\usr\PIECE\include/stdarg.h:25: redefinition of `va_list'
	C:\usr\PIECE\include/piece.h:349: `va_list' previously declared here
	
	@author zurachu
*/

#include <stdarg.h>
#include <piece.h>
#include <smcvals.h>

/// �ʏ�� pceFontPrintf() ��ޔ�
unsigned int (*old_pceFontPrintf)( const char *fmt, ... ) = NULL;

unsigned int new_pceFontPrintf( const char *fmt, ... )
{
	// �V�X�e���Ŋm�ۂ��Ă����ʃo�b�t�@�𕶎���W�J�ɗ��p
	extern unsigned char _def_vbuff[];
	int result;
	va_list ap;

	va_start( ap, fmt );
	result = vsprintf( _def_vbuff, fmt, ap );
	va_end( ap );

	pceFontPutStr( _def_vbuff ); // �t�b�N�ς݂̑z��
	return result;
}

