#include "zurapce/zurapce.h"

static BOOL s_initialize_succeed = FALSE;

extern BYTE GRP[];
static PIECE_BMP s_bmp;

PrecisionTimer g_timer;

/// 初期化.
void pceAppInit( void )
{
	FramObject_Init();
	pceAppSetProcPeriod( 33 );
	Configure_Init();
	FontProxy_Hook_Set();
	FontExtend_Hook_GetAdrs();
	
	if( Lcd_Init() )
	{
		if( Ldirect_Init() )
		{
			PieceBmp_Construct( &s_bmp, FilePack_Data( "B001000.pgx", GRP ) );
			Ldirect_DrawObject( &s_bmp, 0, 0, 0, 0, s_bmp.header.w, s_bmp.header.h );
			Ldirect_VBuffView( TRUE );
			
			FontFuchi_SetType( 0 );
			FontFuchi_SetTxColor( 0 );
			FontFuchi_SetBdColor( 3 );
			FontFuchi_SetPos( 1, 1 );
			FontFuchi_SetRange( 1, 0, DISP_X - 1, DISP_Y - 1 );
			FontFuchi_Printf( "Hello, World\n"
								"\xF0\x40\xF0\x41\xF0\x42\xF0\x46\xF0\x47" );
			PrecisionTimer_Construct( &g_timer );
			
			s_initialize_succeed = TRUE;
		}
	}
}

/// メイン.
void pceAppProc( int cnt )
{
	if( !s_initialize_succeed || pcePadGet() & TRG_D )
	{
		pceAppReqExit( 0 );
	}
	
	pceLCDPaint( 0, 0, 80, DISP_X, 8 );
	FontFuchi_SetType( 2 );
	FontFuchi_SetPos( 1, 80 );
	FontFuchi_Printf( "%8luus FREE:%8d", PrecisionTimer_Count( &g_timer ), pceHeapGetMaxFreeSize() );
	
	Ldirect_Update();
	Ldirect_Trans();
}

/// 終了.
void pceAppExit( void )
{
	FontExtend_Unhook_GetAdrs();
	FontProxy_Unhook_Set();
	Configure_Exit();
}

