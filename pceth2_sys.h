#ifndef __PCETH2_SYS_H_
#define __PCETH2_SYS_H_

#include "common.h"

#define FNAMELEN_SCP	13
#define FNAMELEN_EV		12

//=============================================================================
//	�X�N���v�g�ǂݍ���
//=============================================================================
void pceth2_loadScript(SCRIPT_DATA *s, const char *fName);
void pceth2_closeScript(SCRIPT_DATA *s);

//=============================================================================
//	�X�N���v�g
//=============================================================================
void pceth2_loadEVScript();
int pceth2_jumpScript(SCRIPT_DATA *s);

//=============================================================================
//	���x���W�����v
//=============================================================================
int pceth2_skipLabel(SCRIPT_DATA *s);
int pceth2_jumpLabel(SCRIPT_DATA *s);
int pceth2_branchLabel(SCRIPT_DATA *s);

//=============================================================================
//	�t���O�E���W�X�^����
//=============================================================================
int pceth2_loadFlag(SCRIPT_DATA *s);
int pceth2_saveFlag(SCRIPT_DATA *s);
int pceth2_setReg(SCRIPT_DATA *s);
int pceth2_incReg(SCRIPT_DATA *s);
int pceth2_decReg(SCRIPT_DATA *s);

//=============================================================================
//	���̑�
//=============================================================================
int pceth2_wait(SCRIPT_DATA *s);
int pceth2_goEpilogue(SCRIPT_DATA *s);
int pceth2_backTitle(SCRIPT_DATA *s);

//=============================================================================
//	���l�v�Z
//=============================================================================
int pceth2_calcExpression(SCRIPT_DATA *s);

#endif