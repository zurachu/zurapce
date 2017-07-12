#include "LinearParameter.h"

/** @file
	���`���ԕω��p�����[�^�֘A�̎���.
	@author zurachu
*/

void LinearParameter_Set( LinearParameter* p, int start, int goal, int frame )
{
	p->start = start;
	p->diff = goal - start;
	p->frame_count = 0;
	p->frame_max = ( 0 < frame )? frame : 0;
}

int LinearParameter_Get( LinearParameter const* p )
{
	if( p->frame_max <= p->frame_count )
	{
		return p->start + p->diff;
	}
	return p->start + p->diff * p->frame_count / p->frame_max;
}

void LinearParameter_Update( LinearParameter* p )
{
	p->frame_count++;
	if( p->frame_max < p->frame_count )
	{
		p->frame_count = p->frame_max;
	}
}

