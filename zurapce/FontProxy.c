#include "FontProxy.h"

/** @file
	フォント設定の代行処理の実装.
	@author zurachu
*/

#include <piece.h>
#include "vector.h"

static int s_type = 0; ///< 種類
static int s_tx_color = 3; ///< 文字色
static int s_bk_color = 0; ///< 背景色

/// 通常の pceFontSet 系関数を退避
static void (*old_pceFontSetType)( int type );
static void (*old_pceFontSetTxColor)( int color );
static void (*old_pceFontSetBkColor)( int color );

/// pceFontSet 系 API フックフラグ
static BOOL s_pceFontSet_hooked = FALSE;

void FontProxy_SetType( int type )
{
	s_type = type;
	s_pceFontSet_hooked ? old_pceFontSetType( type ) : pceFontSetType( type );
}

int FontProxy_GetType( void )
{
	return s_type;
}

void FontProxy_SetTxColor( int color )
{
	s_tx_color = color;
	s_pceFontSet_hooked ? old_pceFontSetTxColor( color ) : pceFontSetTxColor( color );
}

int FontProxy_GetTxColor( void )
{
	return s_tx_color;
}

void FontProxy_SetBkColor( int color )
{
	s_bk_color = color;
	s_pceFontSet_hooked ? old_pceFontSetBkColor( color ) : pceFontSetBkColor( color );
}

int FontProxy_GetBkColor( void )
{
	return s_bk_color;
}

void FontProxy_Hook_Set( void )
{
	if( !s_pceFontSet_hooked )
	{
		old_pceFontSetType = pceVectorSetKs( KSNO_FontSetType, FontProxy_SetType );
		old_pceFontSetTxColor = pceVectorSetKs( KSNO_FontSetTxColor, FontProxy_SetTxColor );
		old_pceFontSetBkColor = pceVectorSetKs( KSNO_FontSetBkColor, FontProxy_SetBkColor );
		s_pceFontSet_hooked = TRUE;
	}
}

void FontProxy_Unhook_Set( void )
{
	if( s_pceFontSet_hooked )
	{
		 pceVectorSetKs( KSNO_FontSetBkColor, old_pceFontSetBkColor );
		 pceVectorSetKs( KSNO_FontSetTxColor, old_pceFontSetTxColor );
		 pceVectorSetKs( KSNO_FontSetType, old_pceFontSetType );
		s_pceFontSet_hooked = FALSE;
	}
}

