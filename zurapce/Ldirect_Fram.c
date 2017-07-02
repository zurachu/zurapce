#include "Ldirect.h"

/** @file
	16階調表示関連ライブラリの高速 RAM 配置コード.
	高速 RAM に配置しなくても動作するが、当然処理速度は劣ります。
	
	@author zurachu
*/

#include <piece.h>
#include <string.h>

/// ４階調バッファ描画フラグ
static BOOL s_vbuff_view = FALSE;

/// 残り更新ページ数カウント
static int s_dbuff_update_count = 0;

/**
	16階調用仮想画面バッファから指定ページのダイレクト転送用バッファにマスクなし転送.
	@param page ダイレクト転送用バッファのページ番号
*/
static void lbuff_trans( int const page )
{
	static BYTE const s_color_table[ LDIRECT_PAGE_NUM ][ LDIRECT_COLOR_NUM ] =
	{
		{ 0, 1, 0, 1, 1, 1, 2, 1, 2, 2, 2, 3, 2, 3, 3, 3 },
		{ 0, 0, 1, 0, 1, 1, 1, 2, 1, 2, 2, 2, 3, 2, 3, 3 },
		{ 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3 },
		{ 0, 0, 1, 0, 1, 1, 1, 2, 1, 2, 2, 2, 3, 2, 3, 3 },
		{ 0, 0, 0, 1, 0, 1, 1, 1, 2, 1, 2, 2, 2, 3, 2, 3 },
		//0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	};
	int xx, yy;
	BYTE* dbuff_ptr = g_dbuff[ page ];
	BYTE* lbuff_ptr = g_lbuff;
	BYTE const* const color_table_ptr = s_color_table[ page ];
	BYTE c, high_bit, low_bit;

	for( xx = 0; xx < DISP_X / 8; xx += 1 )
	{
		for( yy = 0; yy < DISP_Y; yy += 1 )
		{
			c = color_table_ptr[ *lbuff_ptr++ ]; high_bit = c >> 1; low_bit = c & 1;
			c = color_table_ptr[ *lbuff_ptr++ ]; high_bit |= c & 2; low_bit |= ( c & 1 ) << 1;
			c = color_table_ptr[ *lbuff_ptr++ ]; high_bit |= ( c & 2 ) << 1; low_bit |= ( c & 1 ) << 2;
			c = color_table_ptr[ *lbuff_ptr++ ]; high_bit |= ( c & 2 ) << 2; low_bit |= ( c & 1 ) << 3;
			c = color_table_ptr[ *lbuff_ptr++ ]; high_bit |= ( c & 2 ) << 3; low_bit |= ( c & 1 ) << 4;
			c = color_table_ptr[ *lbuff_ptr++ ]; high_bit |= ( c & 2 ) << 4; low_bit |= ( c & 1 ) << 5;
			c = color_table_ptr[ *lbuff_ptr++ ]; high_bit |= ( c & 2 ) << 5; low_bit |= ( c & 1 ) << 6;
			c = color_table_ptr[ *lbuff_ptr++ ]; high_bit |= ( c & 2 ) << 6; low_bit |= ( c & 1 ) << 7;
			*dbuff_ptr++ = high_bit;
			*dbuff_ptr++ = low_bit;
			lbuff_ptr += DISP_X - 8;
		}
		lbuff_ptr += -( DISP_X * DISP_Y ) + 8;
	}
}

/**
	４階調用仮想画面バッファから指定ページのダイレクト転送用バッファにマスクあり転送.
	@param page ダイレクト転送用バッファのページ番号
*/
static void vbuff_trans( int const page )
{
	int xx, yy;
	BYTE* dbuff_ptr = g_dbuff[page];
	BYTE* vbuff_ptr = pceLCDSetBuffer( INVALIDPTR );
	BYTE c, mask_bit, high_bit, low_bit;

	for( xx = 0; xx < DISP_X / 8; xx += 1 )
	{
		for( yy = 0; yy < DISP_Y; yy += 1 )
		{
			c = *vbuff_ptr++; mask_bit  = ( c & LDIRECT_VBUFF_TRANS ) >> 6;	high_bit = c >> 1;	low_bit = c & 1;
			c = *vbuff_ptr++; mask_bit |= ( c & LDIRECT_VBUFF_TRANS ) >> 5;	high_bit |= c & 2;	low_bit |= ( c & 1 ) << 1;
			c = *vbuff_ptr++; mask_bit |= ( c & LDIRECT_VBUFF_TRANS ) >> 4; high_bit |= ( c & 2 ) << 1;	low_bit |= ( c & 1 ) << 2;
			c = *vbuff_ptr++; mask_bit |= ( c & LDIRECT_VBUFF_TRANS ) >> 3; high_bit |= ( c & 2 ) << 2; low_bit |= ( c & 1 ) << 3;
			c = *vbuff_ptr++; mask_bit |= ( c & LDIRECT_VBUFF_TRANS ) >> 2; high_bit |= ( c & 2 ) << 3; low_bit |= ( c & 1 ) << 4;
			c = *vbuff_ptr++; mask_bit |= ( c & LDIRECT_VBUFF_TRANS ) >> 1; high_bit |= ( c & 2 ) << 4; low_bit |= ( c & 1 ) << 5;
			c = *vbuff_ptr++; mask_bit |= ( c & LDIRECT_VBUFF_TRANS ); high_bit |= ( c & 2 ) << 5; low_bit |= ( c & 1 ) << 6;
			c = *vbuff_ptr++; mask_bit |= ( c & LDIRECT_VBUFF_TRANS ) << 1; high_bit |= ( c & 2 ) << 6; low_bit |= ( c & 1 ) << 7;
			high_bit &= ~mask_bit;	*dbuff_ptr &= mask_bit;	*dbuff_ptr++ |= high_bit;
			low_bit &= ~mask_bit;	*dbuff_ptr &= mask_bit;	*dbuff_ptr++ |= low_bit;
			vbuff_ptr += DISP_X - 8;
		}
		vbuff_ptr += -( DISP_X * DISP_Y ) + 8;
	}
}

void Ldirect_Update( void )
{
	s_dbuff_update_count = LDIRECT_PAGE_NUM;
}

void Ldirect_Trans( void )
{
	static int page = 0;
	
	if( s_dbuff_update_count )
	{
		lbuff_trans( page );
		if( s_vbuff_view )
		{
			vbuff_trans( page );
		}
		s_dbuff_update_count--;
	}
	pceLCDTransDirect( g_dbuff[ page ] );
	page = ( page + 1 ) % LDIRECT_PAGE_NUM;
}

void Ldirect_VBuffView( BOOL visible )
{
	s_vbuff_view = visible;
}

void Ldirect_VBuffClear( int x, int y, int width, int height )
{
	BYTE* vbuff_ptr = pceLCDSetBuffer( INVALIDPTR );
	int yy;

	if( x >= DISP_X || y >= DISP_Y ) return;
	if( x < 0 ) { width += x; x = 0; }
	if( y < 0 ) { height += y; y = 0; }
	if( width <= 0 || height <= 0 ) return;
	if( x + width >= DISP_X ) { width = DISP_X - x; }
	if( y + height >= DISP_Y ) { height = DISP_Y - y; }

	vbuff_ptr += x + DISP_X * y;
	for( yy = 0; yy < height; yy += 1 )
	{
		memset( vbuff_ptr, LDIRECT_VBUFF_TRANS, width );
		vbuff_ptr += DISP_X;
	}
}

int Ldirect_DrawObject( PIECE_BMP const* p, int dx, int dy, int sx, int sy
						, int width, int height )
{
	int mask_bit = p->header.mask;
	int sw = p->header.w;
	int sh = p->header.h;
	int xx, yy, x_bit;
	BYTE* buff_base = p->buf;
	BYTE* mask_base = p->mask;
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

	lbuff_ptr = g_lbuff + ( dx + DISP_X * dy );

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
	if( x >= DISP_X ) return;
	if( x < 0 ) return;
	if( y >= DISP_Y ) return;
	if( y < 0 ) return;
	
	*( g_lbuff + ( x + DISP_X * y ) ) = color;
}

void Ldirect_Paint( BYTE color, int x, int y, int width, int height )
{
	BYTE* lbuff_ptr;
	int yy;

	if( x >= DISP_X || y >= DISP_Y ) return;
	if( x < 0 ) { width += x; x = 0; }
	if( y < 0 ) { height += y; y = 0; }
	if( width <= 0 || height <= 0 ) return;
	if( x + width > DISP_X ) { width = DISP_X - x; }
	if( y + height > DISP_Y ) { height = DISP_Y - y; }

	lbuff_ptr = g_lbuff + ( x + DISP_X * y );
	for( yy = 0; yy < height; yy += 1 )
	{
		memset( lbuff_ptr, color, width );
		lbuff_ptr += DISP_X;
	}
}

