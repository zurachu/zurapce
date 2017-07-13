#include "Ldirect.h"

/** @file
	16�K���\���֘A�̎���.
	
	@author zurachu
*/

#include <piece.h>
#include <string.h>
#include "vector.h"

/// ���z��ʃo�b�t�@�i16�K���p�`��̏��4�K���p�`�悪�d�Ȃ錩���ځj
BYTE* s_vbuff = NULL;
/// 16�K���p�`��o�b�t�@
BYTE* s_16buff = NULL;
/// 4�K���p�`��o�b�t�@
BYTE* s_4buff = NULL;
/// LCD �_�C���N�g�]���p�o�b�t�@
BYTE* s_dbuff[ LDIRECT_PAGE_NUM ] = { NULL };

/// �ʏ�� pceLCDSetBuffer ��ޔ�
static unsigned char* (*old_pceLCDSetBuffer)( unsigned char* pbuff ) = NULL;
/// pceLcdSetBuffer �ݒ���e
static unsigned char* s_lcd_set_buffer = INVALIDPTR;
/// �ʏ�̕`��֐��ipceLcdSetBuffer() �܂��� vbuff ���Q�Ɓj��ޔ�
static unsigned short (*old_pceFontPut)( int x, int y, unsigned short code ) = NULL;
static unsigned int (*old_pceFontPutStr)( const char *p ) = NULL;
extern unsigned int (*old_pceFontPrintf)( const char *fmt, ... ); // Ldirect_Printf.c
static int (*old_pceLCDDrawObject)( DRAW_OBJECT obj ) = NULL;
static void (*old_pceLCDLine)( long color, long x1, long y1, long x2, long y2 ) = NULL;
static void (*old_pceLCDPaint)( long color, long x, long y, long w, long h ) = NULL;
static void (*old_pceLCDPoint)( long color, long x, long y ) = NULL;

/// 4�K���p�`��o�b�t�@�̓��e�����z��ʃo�b�t�@�ɕ`�悷��t���O
BOOL s_vbuff_view = FALSE;
/// LCD �X�V�t���O
BOOL s_lcd_update;

static BYTE* alloc_vbuff( BYTE color )
{
	static int const size = DISP_X * DISP_Y;
	BYTE* const vbuff = pceHeapAlloc( size );
	memset( vbuff, color, size );
	return vbuff;
}

int alloc_dbuff( void )
{
	static int const size = DISP_X * DISP_Y / 4;
	int i;
	for( i = 0; i < LDIRECT_PAGE_NUM; i++ )
	{
		s_dbuff[ i ] = pceHeapAlloc( size );
		memset( s_dbuff[ i ], 0, size );
		if( !s_dbuff[ i ] )
		{
			while( --i >= 0 )
			{
				pceHeapFree( s_dbuff[ i ] );
			}
			return 0;
		}
	}
	return 1;
}

static void point( BYTE* vbuff, BYTE color, int const x, int const y )
{
	if( x >= DISP_X ) return;
	if( x < 0 ) return;
	if( y >= DISP_Y ) return;
	if( y < 0 ) return;
	
	*( vbuff + ( x + DISP_X * y ) ) = color;
}

static void paint( BYTE* vbuff, BYTE color, int x, int y, int width, int height )
{
	int yy;

	if( x >= DISP_X || y >= DISP_Y ) return;
	if( x < 0 ) { width += x; x = 0; }
	if( y < 0 ) { height += y; y = 0; }
	if( width <= 0 || height <= 0 ) return;
	if( x + width > DISP_X ) { width = DISP_X - x; }
	if( y + height > DISP_Y ) { height = DISP_Y - y; }

	vbuff += x + DISP_X * y;
	for( yy = 0; yy < height; yy += 1 )
	{
		memset( vbuff, color, width );
		vbuff += DISP_X;
	}
}

// �߂�l�ŉ��z��ʃo�b�t�@�̑����4�K���p�`��o�b�t�@��Ԃ�
// �i������4�K���`��֐�����̌Ăяo����z��j
// ����4�K���p�`��o�b�t�@���w�肳�ꂽ���́A���̏�Ԃɖ߂��Ƃ݂Ȃ��ĉ��z��ʃo�b�t�@��ݒ肷��
static unsigned char* LCDSetBuffer( unsigned char* pbuff )
{
	unsigned char* p;
	if( pbuff == s_4buff )
	{
		pbuff = s_vbuff;
	}
	if( pbuff != INVALIDPTR )
	{
		s_lcd_set_buffer = pbuff;
	}
	p = old_pceLCDSetBuffer( pbuff );
	return ( p == s_vbuff ) ? s_4buff : p;
}

static unsigned short FontPut( int x, int y, unsigned short code )
{
	unsigned short ret;
	if( s_lcd_set_buffer == s_vbuff )
	{
		old_pceLCDSetBuffer( s_4buff );
	}
	ret = old_pceFontPut( x, y, code );
	old_pceLCDSetBuffer( s_lcd_set_buffer );
	return ret;
}

static unsigned int FontPutStr( const char *p )
{
	unsigned int ret;
	if( s_lcd_set_buffer == s_vbuff )
	{
		old_pceLCDSetBuffer( s_4buff );
	}
	ret = old_pceFontPutStr( p );
	old_pceLCDSetBuffer( s_lcd_set_buffer );
	return ret;
}

static int LCDDrawObject( DRAW_OBJECT obj )
{
	int ret;
	if( s_lcd_set_buffer == s_vbuff )
	{
		old_pceLCDSetBuffer( s_4buff );
	}
	ret = old_pceLCDDrawObject( obj );
	old_pceLCDSetBuffer( s_lcd_set_buffer );
	return ret;
}

static void LCDLine( long color, long x1, long y1, long x2, long y2 )
{
	if( s_lcd_set_buffer == s_vbuff )
	{
		old_pceLCDSetBuffer( s_4buff );
	}
	old_pceLCDLine( color, x1, y1, x2, y2 );
	old_pceLCDSetBuffer( s_lcd_set_buffer );
}

static void LCDPaint( long color, long x, long y, long w, long h )
{
	if( s_lcd_set_buffer == s_vbuff )
	{
		paint( s_4buff, color, x, y, w, h );
	}
	else
	{
		old_pceLCDPaint( color, x, y, w, h );
	}
}

static void LCDPoint( long color, long x, long y )
{
	if( s_lcd_set_buffer == s_vbuff )
	{
		point( s_4buff, color, x, y );
	}
	else
	{
		old_pceLCDPoint( color, x, y );
	}
}

int Ldirect_Init( void )
{
	pceLCDDispStop();
	if( !old_pceLCDSetBuffer )
	{
		s_vbuff = alloc_vbuff( 0 );
		if( s_vbuff )
		{
			s_16buff = alloc_vbuff( 0 );
			if( s_16buff )
			{
				s_4buff = alloc_vbuff( COLOR_MASK );
				if( s_4buff )
				{
					if( alloc_dbuff() )
					{
						extern unsigned int new_pceFontPrintf( const char *fmt, ... );
						old_pceLCDSetBuffer = pceVectorSetKs( KSNO_LCDSetBuffer, LCDSetBuffer );
						old_pceFontPut = pceVectorSetKs( KSNO_FontPut, FontPut );
						old_pceFontPutStr = pceVectorSetKs( KSNO_FontPutStr, FontPutStr );
						old_pceFontPrintf = pceVectorSetKs( KSNO_FontPrintf, new_pceFontPrintf );
						old_pceLCDDrawObject = pceVectorSetKs( KSNO_LCDDrawObject, LCDDrawObject );
						old_pceLCDLine = pceVectorSetKs( KSNO_LCDLine, LCDLine );
						old_pceLCDPaint = pceVectorSetKs( KSNO_LCDPaint, LCDPaint );
						old_pceLCDPoint = pceVectorSetKs( KSNO_LCDPoint, LCDPoint );
						pceLCDSetBuffer( s_vbuff );
						s_lcd_update = TRUE;
						pceLCDDispStart();
						return 1;
					}
					pceHeapFree( s_4buff );
				}
				pceHeapFree( s_16buff );
			}
			pceHeapFree( s_vbuff );
		}
	}
	pceLCDDispStart();
	return 0;
}

void Ldirect_Exit( void )
{
	if( old_pceLCDSetBuffer )
	{
		pceVectorSetKs( KSNO_LCDSetBuffer, old_pceLCDSetBuffer );
		pceVectorSetKs( KSNO_FontPut, old_pceFontPut );
		pceVectorSetKs( KSNO_FontPutStr, old_pceFontPutStr );
		pceVectorSetKs( KSNO_FontPrintf, old_pceFontPrintf );
		pceVectorSetKs( KSNO_LCDDrawObject, old_pceLCDDrawObject );
		pceVectorSetKs( KSNO_LCDLine, old_pceLCDLine );
		pceVectorSetKs( KSNO_LCDPaint, old_pceLCDPaint );
		pceVectorSetKs( KSNO_LCDPoint, old_pceLCDPoint );
	}
}

BYTE* Ldirect_Buffer( void )
{
	return s_16buff;
}

void Ldirect_Update( void )
{
	s_lcd_update = TRUE;
}

void Ldirect_VBuffView( BOOL visible )
{
	s_vbuff_view = visible;
}

void Ldirect_VBuffClear( int x, int y, int width, int height )
{
	paint( s_4buff, COLOR_MASK, x, y, width, height );
}

void Ldirect_Point( BYTE color, int const x, int const y )
{
	point( s_16buff, color, x, y );
}

void Ldirect_Paint( BYTE color, int x, int y, int w, int h )
{
	paint( s_16buff, color, x, y, w, h );
}

