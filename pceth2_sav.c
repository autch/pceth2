/*
 *	pceth2 - �Z�[�u�E���[�h�֘A
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/05/30	�J���J�n
 */

#include <string.h>
#include <piece.h>
#include "ld.h"
#include "font_ex.h"

#include "common.h"
#include "pceth2_sav.h"
#include "pceth2_grp.h"
#include "pceth2_snd.h"
#include "pceth2_cal.h"
#include "pceth2_sel.h"
#include "pceth2_msg.h"
#include "pceth2_arc.h"

GLOBAL_SAVE_DATA global;
SAVE_DATA play;


//=============================================================================
//	�O���[�o���Z�[�u�t�@�C��
//=============================================================================

#define GLOBAL_SAVE_FILE_NAME	"pceth2.sav"
/*
 *	�O���[�o���Z�[�u�̓ǂݍ���
 */
BOOL pceth2_readGlobalSaveData()
{
	FILEACC pfa;

	memset(&global, 0, sizeof(GLOBAL_SAVE_DATA));
	global.bright = pceLCDSetBright(INVALIDVAL);
	global.masteratt = pceWaveSetMasterAtt(INVALIDVAL);

	if (pceFileOpen(&pfa, GLOBAL_SAVE_FILE_NAME, FOMD_RD) == 0) {
		pceFileReadSct(&pfa, &global, 0, sizeof(GLOBAL_SAVE_DATA));
		pceFileClose(&pfa);
		if (global.bright < MIN_BRIGHT) {
			global.bright = MIN_BRIGHT;
		}
		return TRUE;
	}

	// �Ȃ���΍��
	if (pceFileCreate(GLOBAL_SAVE_FILE_NAME, sizeof(GLOBAL_SAVE_DATA)) == 0) {
		return TRUE;
	}

	return FALSE;
}

/*
 *	�O���[�o���Z�[�u�̏�������
 */
BOOL pceth2_writeGlobalSaveData()
{
	FILEACC pfa;

	if (pceFileOpen(&pfa, GLOBAL_SAVE_FILE_NAME, FOMD_WR) == 0) {
		pceFileWriteSct(&pfa, &global, 0, sizeof(GLOBAL_SAVE_DATA));
		pceFileClose(&pfa);
		return TRUE;
	}

	return FALSE;
}


//=============================================================================
//	�Z�[�u�t�@�C��
//=============================================================================

#define FNAMELEN_SAV	12

/*
 *	�Z�[�u�t�@�C���̓ǂݍ���
 *
 *	num	�Z�[�u�ԍ�(0-7)
 */
BOOL pceth2_readSaveData(int num)
{
	char buf[FNAMELEN_SAV + 1];
	FILEACC pfa;

	sprintf(buf, "pceth2_%d.sav", num);
	if (pceFileOpen(&pfa, buf, FOMD_RD) == 0) {
		pceFileReadSct(&pfa, &play, 0, sizeof(SAVE_DATA));
		pceFileClose(&pfa);
		return TRUE;
	}

	return FALSE;
}


/*
 *	�Z�[�u�t�@�C���̏�������
 *
 *	num	�Z�[�u�ԍ�(0-7)
 */
BOOL pceth2_writeSaveData(int num)
{
	char buf[FNAMELEN_SAV + 1];
	FILEACC pfa;

	sprintf(buf, "pceth2_%d.sav", num);
	if (pceFileCreate(buf, sizeof(SAVE_DATA)) == 0) {
		if (pceFileOpen(&pfa, buf, FOMD_WR) == 0) {
			pceFileWriteSct(&pfa, &play, 0, sizeof(SAVE_DATA));
			pceFileClose(&pfa);
			return TRUE;
		}
	}

	return FALSE;
}

//=============================================================================
//	�^�C�g�����
//=============================================================================

#define TITLE_BG	"B001000.pgx"
#define TITLE_LOGO	"TH2_LOGO.pgx"

/*
 *	�^�C�g���摜����
 *	�ilbuff�ɕ`�悵�Ă����ɉ�����܂��j
 */
static void pceth2_drawTitleGraphic()
{
	PIECE_BMP	p_title;
	BYTE		*_title;

	_title = fpk_getEntryData(TITLE_BG, NULL, NULL);	// �w�i
	Get_PieceBmp(&p_title, _title);
	ld_DrawObject(&p_title, 0, 0);
	pceHeapFree(_title);
	_title = NULL;

	_title = fpk_getEntryData(TITLE_LOGO, NULL, NULL);	// �^�C�g��
	Get_PieceBmp(&p_title, _title);
	ld_DrawObject(&p_title, 28, 4);
	pceHeapFree(_title);
	_title = NULL;

	ld_LBuffUpdate();
}

static int index = 0;

/*
 *	�^�C�g����ʏ�����
 */
void pceth2_TitleInit()
{
	pceWaveAbort(SND_CH);
	Stop_PieceMML();

	pceth2_drawTitleGraphic();

	pceth2_writeGlobalSaveData();	// �����ŃO���[�o���Z�[�u�ۑ����������ł��邩�ɂ�H

	ld_VBuffClear(0, 0, DISP_X, DISP_Y);	// �I����
	sFontStatus.x = 38;
	sFontStatus.y = 56;
	sFontPutStr("�͂��߂���");
	sFontStatus.x = 38;
	sFontStatus.y = 68;
	sFontPutStr("�Â�����");
	sFontPut(28, 56 + index * 12, '>');
	sFontPut(28 + 65, 56 + index * 12, '<');
	ld_VBuffUpdate();

	play.gameMode = GM_TITLE;
}

/*
 *	�^�C�g���ɖ߂�	Z
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	0�i�����߂��j
 */
int pceth2_backTitle(SCRIPT_DATA *s)
{
	s->p++;
#ifndef _DEBUG	// �f�o�b�O�ł͉������Ȃ�
	pceth2_TitleInit();
#endif

	return 0;
}

void pceth2_Title()
{
	static BOOL LCDUpdate = FALSE;

	if (pcePadGet() & (TRG_UP | TRG_DN)) {
		index ^= 1;	// 0��1�؂�ւ�
		LCDUpdate = TRUE;
	}

	if (LCDUpdate) {
		ld_VBuffClear(28, 56, 8, 24);
		ld_VBuffClear(92, 56, 8, 24);
		sFontPut(28, 56 + index * 12, '>');
		sFontPut(28 + 65, 56 + index * 12, '<');
		ld_LBuffUpdate();
		ld_VBuffUpdate();
		LCDUpdate = FALSE;
	}

	if (pcePadGet() & TRG_A) {	// A
		if (index == 0) {	// �͂��߂���
			pceth2_Init();
		} else {			// �Â�����
			pceth2_SaveInit();
		}
	}
}

//=============================================================================
//	�Z�[�u���[�h���
//=============================================================================

static void pceth2_comeBack(int musplay_flag);

#define SAVE_FILE_NUM	7
int last_gameMode;


static void pceth2_drawSaveMenu()
{
	static char *date[] = {"��", "��", "��", "��", "��", "��", "�y"};
	char buf[FNAMELEN_SAV + 1];
	FILEACC pfa;
	int month, day, i;

	ld_VBuffClear(0, 0, DISP_X, DISP_Y);
	sFontStatus.x = sFontStatus.xMin + 1 + 5 * 5;
	sFontStatus.y = sFontStatus.yMin + 1;
	sFontPutStr("�Z�[�u�E���[�h\n");

	for (i = 0; i < SAVE_FILE_NUM; i++) {
		sFontPrintf("%c %d.",((i == global.save_index)? '>' : ' '), i);
		sprintf(buf, "pceth2_%d.sav", i);
		if (pceFileOpen(&pfa, buf, FOMD_RD) == 0) {
			pceFileReadSct(&pfa, NULL, 0, sizeof(SAVE_DATA));
			month = ((SAVE_DATA*)pfa.aptr)->flag[0];
			day = ((SAVE_DATA*)pfa.aptr)->flag[1];
			sFontPrintf("%2d��%2d�� %s�j��", month, day, date[pceth2_getDate(month, day)]);
		}
		sFontPrintf("\n");
	}
	ld_LBuffUpdate();
	ld_VBuffUpdate();
}

void pceth2_SaveInit()
{
	pceWaveAbort(SND_CH);

	last_gameMode = play.gameMode;
	play.gameMode = GM_SAVE;

	pceth2_drawTitleGraphic();
	pceth2_drawSaveMenu();
}

#define LOAD	0
#define SAVE	1

void pceth2_SaveMenu()
{
	static int phase = 0;
	static int mode = LOAD;
	static BOOL LCDUpdate = FALSE;

	if (phase == 0)
	{
		if (pcePadGet() & (TRG_UP)) {
			global.save_index = (global.save_index - 1 + SAVE_FILE_NUM) % SAVE_FILE_NUM;
			LCDUpdate = TRUE;
		}
		if (pcePadGet() & (TRG_DN)) {
			global.save_index = (global.save_index + 1) % SAVE_FILE_NUM;
			LCDUpdate = TRUE;
		}
	} else {
		if (pcePadGet() & (TRG_UP | TRG_DN)) {
			mode ^= 1;	// 0��1�؂�ւ�
			LCDUpdate = TRUE;
		}
	}

	if (LCDUpdate) {
		ld_VBuffClear(0, 0, sFontStatus.xMin + FONT_W / 2 + 2, DISP_Y);
		sFontPut(sFontStatus.xMin + 1, sFontStatus.yMin + 1 + global.save_index * FONT_H + FONT_H, '>');
		if (phase == 1) {
			pceLCDPaint(3, 42, 32, 12, 24);
			pceFontPut(44, 34 + mode * 10, '>');
		}
		ld_LBuffUpdate();
		ld_VBuffUpdate();
		LCDUpdate = FALSE;
	}

	if (pcePadGet() & TRG_A) {	// A
		if ((phase == 0 && last_gameMode == GM_TITLE) || (phase == 1 && mode == LOAD)) {
			if (pceth2_readSaveData(global.save_index)) {	// ���[�h
				pceth2_comeBack(1);
				phase = 0;
			}
		} else if (phase == 1) {	// �Z�[�u
			play.gameMode = last_gameMode;	// �Z�[�u�p�Ɉ�u�߂�
			pceth2_writeSaveData(global.save_index);
			play.gameMode = GM_SAVE;
			pceth2_drawSaveMenu();
			phase = 0;
		} else {	// ���[�h�E�Z�[�u�I����
/* �E�B���h�E������ */
			pceLCDPaint(3, 42, 32, 44, 24);
			pceFontSetTxColor(0);
			pceFontSetBkColor(FC_SPRITE);
			pceFontSetPos(54, 34);
			pceFontPutStr("���[�h");
			pceFontSetPos(54, 44);
			pceFontPutStr("�Z�[�u");
			pceFontPut(44, 34 + mode * 10, '>');
			ld_VBuffUpdate();
			phase = 1;
		}

	} else if (pcePadGet() & TRG_B) {	// B
		if (phase == 0) {
			if (last_gameMode == GM_TITLE) {
				pceth2_TitleInit();
			} else {
			play.gameMode = last_gameMode;
			pceth2_comeBack(0);
			}
		} else {
			pceth2_drawSaveMenu();
			phase = 0;
		}
	}
}

/*
 *	�Z�[�u�f�[�^����e���ԕ��A
 *
 *	musplay_flag	0�Ȃ特�y���Đ��������Ȃ��i�Z�[�u���j���[���畜�A���������̎��j
 */
static void pceth2_comeBack(int musplay_flag)
{
	char	buf[16];	// �t�@�C�����ޔ�p
	int		i;

	// �N���A�t���O���O���[�o���Ɠ�������
	play.flag[80] = 0;
	for (i = 1; i < GLOBAL_FLAG_NUM; i++) {
		play.flag[80 + i] |= global.flag[i];
		play.flag[80] += play.flag[80 + i];
	}

	for (i = 0; i < GRP_NUM; i++) {
		strcpy(buf, play.pgxname[i]);
		pceth2_loadGraphic(buf, i);
	}
	pceth2_DrawGraphic();	// �摜�`��
	ld_LBuffUpdate();

	if (musplay_flag) {
		strcpy(buf, play.pmdname);
		Play_PieceMML(buf);	// BGM�Đ�
	}

	play.evData.data = fpk_getEntryData(play.evData.name, &play.evData.size, NULL);	// EV
	play.scData.data = fpk_getEntryData(play.scData.name, &play.scData.size, NULL);	// �X�N���v�g

	sFontPrintf("\n");	// �Ȃ񂩒m��񂯂ǖ��ʉ��s����Ă����Ȃ��ƃ��b�Z�[�W�̌��ɉ��s������
	ld_VBuffClear(0, 0, DISP_X, DISP_Y);
	sFontStatus.x = sFontStatus.xMin + 1;
	sFontStatus.y = sFontStatus.yMin + 1;
	sFontPrintf("%s", play.msg);	// ���b�Z�[�W

	if (play.gameMode == GM_SELECT || play.gameMode == GM_MAPSELECT) {	// �I�𒆂Ȃ���`��
		pceth2_drawSelArrow();
	}
	msgView = 1;
	ld_VBuffUpdate();
}
