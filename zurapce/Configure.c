#include "Configure.h"

/** @file
	設定保存関連の実装.
	@author zurachu
*/

#include <piece.h>

static int s_last_bright;
static int s_last_master_att;

void Configure_Init( void )
{
	s_last_bright = pceLCDSetBright( INVALIDVAL );
	s_last_master_att = pceWaveSetMasterAtt( INVALIDVAL );
}

void Configure_Exit( void )
{
	pceLCDSetBright( s_last_bright );
	pceWaveSetMasterAtt( s_last_master_att );
}

