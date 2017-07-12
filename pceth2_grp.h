#ifndef __PCETH2_GRP_H_
#define __PCETH2_GRP_H_

#include "common.h"

#define FNAMELEN_BG		11
#define FNAMELEN_CH		13
#define FNAMELEN_CLOCK	11

#define NUMLEN_CH		3

#define GRP_L		0
#define GRP_C		1
#define GRP_R		2
#define GRP_BG		3
#define GRP_AUTO	5

#define BG_TIME		(play.bgopt[0])
#define BG_WEATHER	(play.bgopt[1])
#define BG_WEATHER_FLAG	5

//=============================================================================
//	�摜�`��
//=============================================================================
void pceth2_DrawGraphic();

//=============================================================================
//	�摜�t�@�C���ǂݍ���
//=============================================================================
void pceth2_loadGraphic(const char *fName, const int pos);
void pceth2_clearGraphic(const int pos);

//=============================================================================
//	�摜�\���E����
//=============================================================================
int  pceth2_setBGOption(SCRIPT_DATA *s);
int  pceth2_loadBG(SCRIPT_DATA *s);
int  pceth2_loadChara(SCRIPT_DATA *s);
int  pceth2_clearChara(SCRIPT_DATA *s);

//=============================================================================
//	�����G�X���C�h
//=============================================================================
void pceth2_slideChara();

int pceth2_isCalenderMode();

#endif