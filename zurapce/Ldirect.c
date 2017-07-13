#include "Ldirect.h"

/** @file
	16�K���\���֘A�̎���.
	
	@author zurachu
*/

#include <piece.h>
#include <string.h>
#include "vector.h"

#define LDIRECT_PAGE_NUM	5 ///< �؂�ւ��y�[�W��
#define LDIRECT_COLOR_NUM	16 ///< �K�����iLDIRECT_PAGE_NUM * 3 + 1�j

/// ���z��ʃo�b�t�@�i16�K���p�`��̏��4�K���p�`�悪�d�Ȃ錩���ځj
static BYTE* s_vbuff = NULL;
/// 16�K���p�`��o�b�t�@
static BYTE* s_16buff = NULL;
/// 4�K���p�`��o�b�t�@
static BYTE* s_4buff = NULL;
/// LCD �_�C���N�g�]���p�o�b�t�@
static BYTE* s_dbuff[ LDIRECT_PAGE_NUM ] = { NULL };

/// �ʏ�� pceLCDSetBuffer ��ޔ�
static unsigned char* (*old_pceLCDSetBuffer)( unsigned char* pbuff ) = NULL;
/// pceLcdSetBuffer �ݒ���e
static unsigned char* s_lcd_set_buffer = INVALIDPTR;
/// �ʏ�̕`��֐��ipceLcdSetBuffer() �܂��� vbuff ���Q�Ɓj��ޔ�
static unsigned short (*old_pceFontPut)( int x, int y, unsigned short code ) = NULL;
static int (*old_pceLCDDrawObject)( DRAW_OBJECT obj ) = NULL;
static void (*old_pceLCDLine)( long color, long x1, long y1, long x2, long y2 ) = NULL;
static void (*old_pceLCDPaint)( long color, long x, long y, long w, long h ) = NULL;
static void (*old_pceLCDPoint)( long color, long x, long y ) = NULL;


/// 4�K���p�`��o�b�t�@�̓��e�����z��ʃo�b�t�@�ɕ`�悷��t���O
static BOOL s_vbuff_view = FALSE;

/// LCD �X�V�t���O
static BOOL s_lcd_update;
/// �c��X�V�y�[�W���J�E���g
static int s_dbuff_update_count = 0;

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
						old_pceLCDSetBuffer = pceVectorSetKs( KSNO_LCDSetBuffer, LCDSetBuffer );
						old_pceFontPut = pceVectorSetKs( KSNO_FontPut, FontPut );
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

/**
	4�K���p�`��o�b�t�@���牼�z��ʃo�b�t�@�Ƀ}�X�N����]��.
*/
void trans_4buff_to_vbuff( void )
{
	static DWORD const s_color_bits = 0x03030303;
	static DWORD const s_mask_bits = ( COLOR_MASK << 24 ) | ( COLOR_MASK << 16 ) | ( COLOR_MASK << 8 ) | COLOR_MASK;
	int i;
	DWORD color, mask;
	DWORD* vbuff_ptr = (DWORD*)s_vbuff;
	DWORD const* b4buff_ptr = (DWORD*)s_4buff;
	for( i = 0; i < DISP_X * DISP_Y / sizeof(DWORD); i += 1 )
	{
		color = ( *b4buff_ptr & s_color_bits );
		color |= color << 2;
		mask = ( ( *b4buff_ptr & s_mask_bits ) >> 2 ) * 0xFF;
		*vbuff_ptr &= mask;
		*vbuff_ptr++ |= color & ~mask;
		++b4buff_ptr;
	}
}

/**
	���z��ʃo�b�t�@����w��y�[�W�̃_�C���N�g�]���p�o�b�t�@�Ƀ}�X�N�Ȃ��]��.
	@param page �_�C���N�g�]���p�o�b�t�@�̃y�[�W�ԍ�
*/
static void trans_vbuff_to_dbuff( int page )
{
	static WORD const s_color_table[ LDIRECT_PAGE_NUM ][ LDIRECT_COLOR_NUM ] =
	{
		{ 0x000, 0x100, 0x000, 0x100, 0x100, 0x100, 0x001, 0x100, 0x001, 0x001, 0x001, 0x101, 0x001, 0x101, 0x101, 0x101 },
		{ 0x000, 0x000, 0x100, 0x000, 0x100, 0x100, 0x100, 0x001, 0x100, 0x001, 0x001, 0x001, 0x101, 0x001, 0x101, 0x101 },
		{ 0x000, 0x000, 0x000, 0x100, 0x100, 0x100, 0x100, 0x100, 0x001, 0x001, 0x001, 0x001, 0x001, 0x101, 0x101, 0x101 },
		{ 0x000, 0x000, 0x100, 0x000, 0x100, 0x100, 0x100, 0x001, 0x100, 0x001, 0x001, 0x001, 0x101, 0x001, 0x101, 0x101 },
		{ 0x000, 0x000, 0x000, 0x100, 0x000, 0x100, 0x100, 0x100, 0x001, 0x100, 0x001, 0x001, 0x001, 0x101, 0x001, 0x101 },
		//    0,     1,     2,     3,     4,     5,     6,     7,     8,     9,     A,     B,     C,     D,     E,     F
	};
	int xx, yy;
	DWORD* dbuff_ptr = (DWORD*)s_dbuff[ page ];
	BYTE const* vbuff_ptr = s_vbuff;
	BYTE const* vbuff_ptr1 = vbuff_ptr + DISP_X;
	WORD const* const color_table_ptr = s_color_table[ page ];
	DWORD c, c1;

	for( xx = 0; xx < DISP_X / 8; xx += 1 )
	{
		for( yy = 0; yy < DISP_Y / 2; yy += 1 )
		{
			c = color_table_ptr[ *vbuff_ptr++ ];
			c |= color_table_ptr[ *vbuff_ptr++ ] << 1;
			c |= color_table_ptr[ *vbuff_ptr++ ] << 2;
			c |= color_table_ptr[ *vbuff_ptr++ ] << 3;
			c |= color_table_ptr[ *vbuff_ptr++ ] << 4;
			c |= color_table_ptr[ *vbuff_ptr++ ] << 5;
			c |= color_table_ptr[ *vbuff_ptr++ ] << 6;
			c |= color_table_ptr[ *vbuff_ptr++ ] << 7;
			c1 = color_table_ptr[ *vbuff_ptr1++ ];
			c1 |= color_table_ptr[ *vbuff_ptr1++ ] << 1;
			c1 |= color_table_ptr[ *vbuff_ptr1++ ] << 2;
			c1 |= color_table_ptr[ *vbuff_ptr1++ ] << 3;
			c1 |= color_table_ptr[ *vbuff_ptr1++ ] << 4;
			c1 |= color_table_ptr[ *vbuff_ptr1++ ] << 5;
			c1 |= color_table_ptr[ *vbuff_ptr1++ ] << 6;
			c1 |= color_table_ptr[ *vbuff_ptr1++ ] << 7;
			*dbuff_ptr++ = c | ( c1 << 16 );
			vbuff_ptr += DISP_X * 2 - 8;
			vbuff_ptr1 += DISP_X * 2 - 8;
		}
		vbuff_ptr += -( DISP_X * DISP_Y ) + 8;
		vbuff_ptr1 += -( DISP_X * DISP_Y ) + 8;
	}
}

void Ldirect_Update( void )
{
	s_lcd_update = TRUE;
}

void Ldirect_Trans( void )
{
	static int page = 0;

	if( s_lcd_update )
	{
		memcpy( s_vbuff, s_16buff, DISP_X * DISP_Y );
		if( s_vbuff_view )
		{
			trans_4buff_to_vbuff();
		}
		s_dbuff_update_count = LDIRECT_PAGE_NUM;
	}
	if( s_dbuff_update_count )
	{
		trans_vbuff_to_dbuff( page );
		s_dbuff_update_count--;
	}
	pceLCDTransDirect( s_dbuff[ page ] );
	page = ( page + 1 ) % LDIRECT_PAGE_NUM;
}

void Ldirect_VBuffView( BOOL visible )
{
	s_vbuff_view = visible;
}

void Ldirect_VBuffClear( int x, int y, int width, int height )
{
	paint( s_4buff, COLOR_MASK, x, y, width, height );
}

int Ldirect_DrawObject( PIECE_BMP const* p, int dx, int dy, int sx, int sy
						, int width, int height )
{
	int const mask_bit = p->header.mask;
	int const sw = p->header.w;
	int const sh = p->header.h;
	int xx, yy, x_bit;
	BYTE* const buff_base = p->buf;
	BYTE* const mask_base = p->mask;
	BYTE* lbuff_ptr;
	BYTE* buff_ptr;
	BYTE* mask_ptr;
	BYTE b = 0, m = 0;
	
	if( dx >= DISP_X || dy >= DISP_Y ) return 0;
	if( dx < 0 ) { width += dx; sx -= dx; dx = 0; }
	if( dy < 0 ) { height += dy; sy -= dy; dy = 0; }
	if( sx < 0 ) { width += sx; sx = 0; }
	if( sy < 0 ) { height += sy; sy = 0; }
	if( width <= 0 || height <= 0 ) return 0;
	if( width > sw ) { width = sw; }
	if( height > sh ) { height = sh; }
	if( dx + width > DISP_X ) { width = DISP_X - dx; }
	if( dy + height > DISP_Y ) { height = DISP_Y - dy; }

	lbuff_ptr = s_16buff + ( dx + DISP_X * dy );

	for( yy = 0; yy < height; yy += 1 )
	{
		buff_ptr = buff_base + ( ( sx + sw * ( sy + yy ) ) >> 1 );
		mask_ptr = mask_base + ( ( sx + sw * ( sy + yy ) ) >> 3 );
		x_bit = sx & 7;
		if( x_bit & 1 ) { b = *buff_ptr++ << 4; }
		if( x_bit ) { m = ( mask_bit )? ( *mask_ptr++ << x_bit ) : 0; }
		for( xx = 0; xx < width; xx += 1 )
		{
			if( !( x_bit & 1 ) ) { b = *buff_ptr++; }
			if( !x_bit ) { m = ( mask_bit )? *mask_ptr++ : 0; }
			*lbuff_ptr++ = ( m & 0x80 )? *lbuff_ptr : ( b >> 4 );
			x_bit = ( x_bit + 1 ) & 7;
			b <<= 4;
			m <<= 1;
		}
		lbuff_ptr += DISP_X - width;
	}
	return 1;
}

void Ldirect_Point( BYTE color, int const x, int const y )
{
	point( s_16buff, color, x, y );
}

void Ldirect_Paint( BYTE color, int x, int y, int w, int h )
{
	paint( s_16buff, color, x, y, w, h );
}

