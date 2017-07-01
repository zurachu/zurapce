#include "PieceBmp.h"

/** @file
	PIECE_BMP ŠÖ˜A‚ÌŽÀ‘•.
	@author zurachu
*/

#include <piece.h>
#include <string.h>

void PieceBmp_Construct( PIECE_BMP* p, unsigned char* source )
{
	memcpy( &p->header, source, sizeof(PBMP_FILEHEADER) );
	p->buf = source + sizeof(PBMP_FILEHEADER);
	p->mask = p->buf + ( p->header.w * p->header.h * p->header.bpp >> 3 );
}

int PieceBmp_Draw( PIECE_BMP* p, int dx, int dy, int sx, int sy
				, int width, int height, int param )
{
	DRAW_OBJECT draw_object;
	pceLCDSetObject( &draw_object, p, dx, dy, sx, sy, width, height, param );
	return pceLCDDrawObject( draw_object );
}

