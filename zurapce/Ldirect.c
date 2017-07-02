#include "Ldirect.h"

/** @file
	16�K���\���֘A���C�u������ SRAM �z�u�R�[�h.
	���� RAM �ɒu���Ȃ��傫�ȃf�[�^����сA�������̕K�v���Ȃ��֐��B
	
	@author zurachu
*/

/// 16�K���p���z��ʃo�b�t�@
BYTE* g_lbuff = NULL;

/// LCD �_�C���N�g�]���p�o�b�t�@
BYTE* g_dbuff[ LDIRECT_PAGE_NUM ] = { NULL };

int Ldirect_Init( void )
{
	int i;
	
	g_lbuff = pceHeapAlloc( DISP_X * DISP_Y );
	if( !g_lbuff ) { return 0; }
	
	for( i = 0; i < LDIRECT_PAGE_NUM; i++ )
	{
		g_dbuff[ i ] = pceHeapAlloc( DISP_X * DISP_Y / 4 );
		if( !g_dbuff[ i ] )
		{
			while( --i >= 0 )
			{
				pceHeapFree( g_dbuff[ i ] );
				g_dbuff[ i ] = NULL;
			}
			pceHeapFree( g_lbuff );
			g_lbuff = NULL;
			return 0;
		}
	}
	memset( pceLCDSetBuffer( INVALIDPTR ), COLOR_MASK, DISP_X * DISP_Y );
	memset( g_lbuff, 0, DISP_X * DISP_Y );
	for( i = 0; i < LDIRECT_PAGE_NUM; i++ )
	{
		memset( g_dbuff[ i ], 0, DISP_X * DISP_Y / 4 );
	}
	return 1;
}

