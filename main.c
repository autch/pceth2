/*
 *	pceth2
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/02/16	�D���D���^�}���o�����Ver.
 *	2005/03/11	�摜�͊O���t�@�C���ipar�`���j����ǂݍ��ނ悤��
 *	2005/04/09	�X�N���v�g�ǂݍ��ݒB��
 *	2005/04/20	���ꕶ���̏����ς݁A�n�[�g�ǉ�
 *	2005/04/21	�֑������i�A�B�j�ǉ��Aw3����
 *	2005/04/23	�摜�f�[�^�|�C���^���q�[�v�����Ɋm����NULL��
 *	2005/04/25	<S>�A<W>�ɑΉ��A\k��łȂ�\n�𖳎�
 *				w3���ɂ����ƑΉ����Ă��Ȃ������̂��C��
 *				���O�u����*nnk1��ǉ�
 *	2005/04/27	pceth2_grp.c�ɕ���
 *				���x���W�����v�i�����Ȃ��j��ǉ�
 *	2005/04/30	�t���O���[�h�A���x���u�����`�A�X�N���v�g�W�����v��ǉ�
 *	2005/05/01	�t���O�Z�[�u��ǉ�
 *				�֑������Ɂv�x��ǉ�
 *				pceth2_sys.c�ɕ���
 *	2005/05/07	pceth2_snd.c��ǉ�
 *	2005/05/08	pceth2_msg.c�ɕ���
 *	2005/06/11	�J�����_�[���[�h�̎���B�{�^����VBuff�������Ȃ��悤�ɏC��
 *	2005/06/12	B�{�㉺���E�ŃR���g���X�g�A���ʒ��߉\��
 *	2005/06/13	�f�o�b�O�p�r���h�ǉ�
 *	2005/06/15	��s�̕������̈Ⴂ�ɂ����s�̕␳
 *	2005/06/25	���O�u���������R���o�[�^���Ɉړ�
 *	2005/06/30	�R���g���X�g�A���ʒ��߂̑����ύX�i�E�B���h�E�����Ă��Ԃŏ㉺���E�j
 *	2005/07/19	calFlag�p�~�Apgxname[GRP_C]�Ŕ��f����悤��
 *				�X�N���v�g���̃J�����_�[���[�h�ɑΉ�
 *				BG�\�����߂ō�����t�ɍ��킹�Ȃ��P�[�X�i��z�Ȃǁj�ɑΉ�
 *				 *
 *	TODO		�X�N���v�g�̊��S���
 */

#include <string.h>
#include <piece.h>
#include "zurapce/zurapce.h"
#include "usbcapt.h"
#include "muslib2.h"
#include "ld.h"
#include "font_ex.h"

#include "ufe/ufe.h" /*{{2005/06/09 Naoyuki Sawa}}*/
#include "mmc_api.h" //2005/06/11 Added by Madoka

#include "common.h"
#include "pceth2_arc.h"
#include "pceth2_sys.h"
#include "pceth2_msg.h"
#include "pceth2_sel.h"
#include "pceth2_grp.h"
#include "pceth2_snd.h"
#include "pceth2_cal.h"
#include "pceth2_sav.h"

int _bright, _masteratt;	// �N�����̃R���g���X�g�A����
BOOL file_load = FALSE;		// �t�@�C�����J�������ǂ���

int speed, wait, msgView;

static PrecisionTimer s_frame_timer;
static unsigned long s_frame_us, s_proc_us;

int  pceth2_readScript(SCRIPT_DATA *s);
void pceth2_waitKey();

//=============================================================================
//=============================================================================

/*
 *	������
 */
#define ARCHIVE_FILE_NAME	"pceth2.par"	// �A�[�J�C�u�t�@�C����
#define DEBUG_FILE_NAME		"999999999.scp"	// �f�o�b�O���j���[�X�N���v�g
#define PROC_PERIOD	33	// Proc/msec

void pceAppInit(void)
{	
	FramObject_Init();
	
	/*{{2005/06/09 Naoyuki Sawa*/
	if(ufe_setup() != 0)	// UFE������
	{
		//2005/06/11 Added by Madoka
		if(mmcInit(MMC_FILESIZE_MAX) != 1) {	// MMC������
			return;
		}
	}
	/*}}2005/06/09 Naoyuki Sawa*/

	pceLCDDispStop();

	usbCaptureInit();	// pceCaps������
	hook_FontGetAdrs();	// ����t�H���g�ǉ�pceFontGetAdrs���t�b�N
	loadInst();			// �h�������F�����L�b�g������
	InitMusic();		// ���y���C�u����������


	pceAppSetProcPeriod(PROC_PERIOD);
	pceLCDSetBuffer(vbuff);
	memset(vbuff,0,DISP_X*DISP_Y);	//2005/06/11 Added by Madoka �ꉞ��ʂ��L���C�ɂ��邪
	pceLCDTrans();					//�܂��S�~���ł܂��ˁB�Ώ����[���d��Chu����
	ld_VBuffClear(0, 0, DISP_X, DISP_Y);
	// ��UFE���C�������̕\��(Trans)��ld���C�u�����ɂ��\��(TransDirect)�Ɉڂ�Ԃ̖��H

	if (pceth2_readGlobalSaveData()) {

		// ���s�O�̃R���g���X�g�A���ʂ�ۑ�
		_bright = pceLCDSetBright(global.bright);
		_masteratt = pceWaveSetMasterAtt(global.masteratt);

		// �A�[�J�C�u�ǂݍ���
		file_load = fpk_InitHandle(ARCHIVE_FILE_NAME);
		if (file_load) {
#ifdef _DEBUG
			pceth2_Init();
			pceth2_loadScript(&play.scData, DEBUG_FILE_NAME);	// 2005/06/13�ǉ�
#else
			pceth2_TitleInit();
#endif
		}
	}

	msgView = 1;
	speed = 0;

	pceLCDDispStart();
	PrecisionTimer_Construct(&s_frame_timer);
}

/*
 *	���C��
 */
void pceAppProc(int cnt)
{
	PrecisionTimer proc_timer;
	PrecisionTimer_Construct(&proc_timer);

	/*{{2005/06/09 Naoyuki Sawa*/
//	if(!hFpk) { //���������s?
	if (!file_load) {	// 2005/07/23�ύX
		pceAppReqExit(0);
		return;
	}
	/*}}2005/06/09 Naoyuki Sawa*/

	switch (play.gameMode)
	{
        case GM_TITLE:		// �^�C�g�����
			pceth2_Title();
			break;
		case GM_SAVE:	// �Z�[�u���[�h���j���[
			pceth2_SaveMenu();
			break;
		case GM_EVSCRIPT:	// EV_�`�X�N���v�g�ǂݍ���
			while (pceth2_readScript(&play.evData));
			break;
		case GM_SCRIPT:		// �X�N���v�g�ǂݍ���
			while (pceth2_readScript(&play.scData));
			break;
		case GM_SELECT:
			pceth2_Select();
			if (pcePadGet() & PAD_C) { pceth2_SaveInit(); }
			break;
		case GM_MAPSELECT:	// �}�b�v�I��
			pceth2_MapSelect();
			if (pcePadGet() & PAD_C) { pceth2_SaveInit(); }
			break;
		case GM_CALENDER:
			pceth2_calenderDrawCircle();
			break;
		case GM_KEYWAIT:	// �L�[�҂�
			pceth2_waitKey();
			if (pcePadGet() & PAD_C && !pceth2_isCalenderMode()) { pceth2_SaveInit(); }
			break;
		case GM_TIMEWAIT:	// ���ԑ҂�
			if (wait-- <= 0 || (pcePadGet() & PAD_RI)) {
				play.gameMode = GM_SCRIPT;
			}
			break;
		case GM_SLIDECHR:	// �����G�X���C�h
			pceth2_slideChara();
			break;
	}

	if (pcePadGet() & PAD_D) {
		if (play.gameMode == GM_TITLE) {
			pceAppReqExit(0);
		} else {
			pceth2_TitleInit();
		}
	}

	pceLCDPaint(0, 0, 82, DISP_X, 6);
	pceFontSetType(2);
	pceFontSetPos(0, 82);
	pceFontSetTxColor(3);
	pceFontSetBkColor(FC_SPRITE);
	pceFontPrintf("%6lu/%6luus FREE:%8d", s_proc_us, s_frame_us, pceHeapGetMaxFreeSize());
	ld_LBuffUpdate();
	ld_VBuffUpdate();

	ld_LCDTransDirect();

	s_frame_us = PrecisionTimer_Count(&s_frame_timer);
	s_proc_us = PrecisionTimer_Count(&proc_timer);
}

/*
 *	�I��
 */
void pceAppExit(void)
{
	StopMusic();
	pceWaveStop(0);

	/*** �ǂݍ��񂾃t�@�C���𖾎��I�ɉ�����ĂȂ������Ȃ��H ***/

	// �O���[�o���f�[�^�i�t���O�A�R���g���X�g�A���ʁj��ۑ�
	pceth2_writeGlobalSaveData();

	// ���s�O�̃R���g���X�g�A���ʂɖ߂�
	pceLCDSetBright(_bright);
	pceWaveSetMasterAtt(_masteratt);

	fpk_ReleaseHandle();
	unhook_FontGetAdrs();	// pceFontGetAdrs�����ɖ߂�
	usbCaptureRelease();	// pceCaps���

	//2005/06/11 Added by Madoka
	mmcExit();

	/*{{2005/06/09 Naoyuki Sawa*/
	ufe_stop();
	/*}}2005/06/09 Naoyuki Sawa*/
}

//2005/06/11 Added by Madoka
/*
 *	�V�X�e���ʒm
 */
int pceAppNotify(int type, int param)
{	
	
	//MMC�Ή��J�[�l��Ver.1.27�ȍ~�ł̏���
	//�J�[�l�����ł�MMC�������𖳌��ɂ���
	//�������Ȃ��ƁA�傫���t�@�C���������Ȃ�����
	if(type == APPNF_INITMMC)
	{
		return APPNR_REJECT;
	}

	return APPNR_IGNORE;	//�f�t�H���g�̏���
}

//=============================================================================
//	
//=============================================================================

/*
 *	�͂��߂���
 */
void pceth2_Init()
{
	int i;

	pceth2_setPageTop();
	pceth2_clearMessage();

	msgView = 1;
	speed = 0;

	memset(play, 0, sizeof(SAVE_DATA));

	MONTH	= START_MONTH;	// ��
	DAY		= START_DAY;	// ��
	TIME	= EV_MORNING;	// ����
	// �N���A�t���O���O���[�o���Ɠ�������
	memcpy(&play.flag[80], &global.flag, GLOBAL_FLAG_NUM * sizeof(unsigned short));

	memset(reg, 0, REG_NUM);	// ���W�X�^

	for (i = 0; i <= GRP_NUM; i++) {
		pceth2_clearGraphic(i);
	}
	pceth2_DrawGraphic();
	BG_TIME = BG_WEATHER = '0';	// �w�i�摜�t�@�C�����C���q

	Stop_PieceMML();

	pceth2_loadEVScript(&play.evData);

	ld_LBuffUpdate();
	ld_VBuffUpdate();

//	play.gameMode = GM_EVSCRIPT;
}

/*
 *	�L�[�҂�
 */
void pceth2_waitKey()
{
	if (msgView)	// ���b�Z�[�W�\�����
	{
		if (pcePadGet() & (TRG_A | PAD_RI)) {	// �X�N���v�g��i�߂�
			if (pceth2_isPageTop()) {
				pceth2_clearMessage();
				ld_LBuffUpdate();
				ld_VBuffUpdate();
			}
			if (pceth2_isCalenderMode()) {	// �J�����_�[���[�h��
				pceth2_clearGraphic(GRP_C);	// �J�����_�[�摜����
			}
			play.gameMode = GM_SCRIPT;
		} else if (pcePadGet() & TRG_B) {
			if (!pceth2_isCalenderMode()) {	// �J�����_�[�̎��͏����Ȃ�
				msgView = FALSE;
				ld_LBuffUpdate();	// ��������
			}
		}
	}
	else			// ���b�Z�[�W��\�����
	{
		if (pcePadGet() & (TRG_A | TRG_B)) {	
			msgView = TRUE;
			ld_VBuffUpdate();	// ���b�Z�[�W�\��
		}
		// �{�㉺���E�ŃR���g���X�g�A���ʂ̒���
		if (pcePadGet() & TRG_LF && global.bright > 0) {
			pceLCDSetBright(--global.bright);
		}
		if (pcePadGet() & TRG_RI && global.bright < 63) {
			pceLCDSetBright(++global.bright);
		}
		if (pcePadGet() & TRG_DN && global.masteratt < 127) {
			pceWaveSetMasterAtt(++global.masteratt);
		}
		if (pcePadGet() & TRG_UP && global.masteratt > 0) {
			pceWaveSetMasterAtt(--global.masteratt);
		}
	}
}

/*
 *	�X�N���v�g��ǂ�
 *	return	1�̊�pceAppProc()����J��Ԃ��ČĂяo�����
 */
int pceth2_readScript(SCRIPT_DATA *s)
{
	/* ���߉�̓e�[�u�� */
	static const struct {
		char com;
		int  (*func)(SCRIPT_DATA *);
	} com_table[] =	   {{'B',  pceth2_loadBG},		// �w�i�摜�iB6.pgx��11�����{�����G�����t���O1�����j
						{'G',  pceth2_setBGOption},	// �w�i�摜�̃I�v�V�����w��
						{'V',  pceth2_loadBG},		// �C�x���g�摜�iV6.pgx��11�����{�����G�����t���O1�����j
						{'C',  pceth2_loadChara},	// �����G�摜�iC8.pgx��13�����{�ʒu1�����{�X�V�����t���O1�����j
						{'c',  pceth2_clearChara},	// �����G�����ic3�����{�X�V�����t���O1�����j
						{'S',  pceth2_loadSE},		// SE�Đ��iSE_4.ppd��11�����{���s�[�g�t���O1�����j
						{'M',  pceth2_loadBGM},		// BGM���t�iM2.pmd��7�����j�ywin���������z
						{'w',  pceth2_wait},		// ���ԃE�F�C�g�iw3�����j
						{'m',  pceth2_addMapItem},	// �}�b�v�I������ǉ��i9.scp��13�����{�ꏊ2�����{�`�b�v�L����2�����j
						{'J',  pceth2_jumpScript},	// �X�N���v�g�W�����v�i9.scp��13�����j
						{'j',  pceth2_jumpLabel},	// ���x���W�����v
						{'b',  pceth2_branchLabel},	// �����t�����x���W�����v
						{'l',  pceth2_loadFlag},	// �t���O�����W�X�^�Ƀ��[�h
						{'s',  pceth2_saveFlag},	// �t���O����������
						{'=', pceth2_setReg},		// ���W�X�^�ɒl���Z�b�g
						{'+', pceth2_incReg},		// ���W�X�^���C���N�������g
						{'-', pceth2_decReg},		// ���W�X�^���f�N�������g
						{'q',  pceth2_addSelItem},	// �I������ǉ�
						{'Q',  pceth2_initSelect},	// �I��
						{'@',  pceth2_skipLabel},	// ���x���i�ǂݔ�΂��j
						{'<',  pceth2_procControl},	// ���b�Z�[�W����
						{'\\', pceth2_procEscape},	// �G�X�P�[�v�V�[�P���X����
						{'D', pceth2_calenderInitEx},	// �X�N���v�g����J�����_�[���[�h�Ɉڍs
						{'z', pceth2_goEpilogue},	// �G�s���[�O��
						{'Z', pceth2_backTitle},	// �^�C�g���ɖ߂�
	};
	int i;

	// �Ō�܂œǂ񂾂�I��
	if (s->p >= s->size) {
#ifdef _DEBUG	// �f�o�b�O���[�h�̏ꍇ�f�o�b�O���j���[�ɖ߂�
		pceth2_loadScript(&play.scData, DEBUG_FILE_NAME);
#else
		switch(play.gameMode)
		{
			case GM_EVSCRIPT:
				if (!pceth2_initMapSelect()) {	// �}�b�v�I����������΃}�b�v�I����
					JUMP = 0;	// 2005/06/20 �܂邵������āF1e, 4, 1��������goto�̏�����
					if (TIME > EV_NIGHT) {	// ����I��
						TIME = EV_MORNING;
						DAY++;
						pceth2_calenderInit();	// �J�����_�[
					} else {
						pceth2_loadEVScript();	// ����EV�X�N���v�g��ǂ�
					}
//					if (play.evData.size == 0) {	// �ǂ߂Ȃ�������I��
//						pceAppReqExit(0);
//					}
				}
				break;
			case GM_SCRIPT:
				pceth2_closeScript(&play.scData);
				play.gameMode = GM_EVSCRIPT;
				break;
		}
#endif
		return 0;
	}


	// ���߉��
	for (i = 0; i < array_size(com_table); i++) {
		if (*(s->data + s->p) == com_table[i].com) {
			return com_table[i].func(s);
		}
	}

	// �c��͉�ʕ\�����������̂͂��ł���
	if (pceth2_jpnHyphenation(s->data + s->p + 2) || pceth2_lineFeed(s->data + s->p)) {
		pceth2_putCR();
		if (pceth2_isPageTop()) {
			play.gameMode = GM_KEYWAIT;
			goto UPDATE;
		}
	}
	// �A���󔒂͈�����\�����Ȃ��i����Ŏ蓮�Z���^�����O������ł���H�j
	if (strncmp(play.msg + play.msglen - 2, "�@", 2) || strncmp(s->data + s->p, "�@", 2)) {
		pceth2_putKanji(s->data + s->p);
	}
	s->p += 2;
	if (pceth2_isPageTop()) {
		play.gameMode = GM_KEYWAIT;
		goto UPDATE;
	}

	if (pcePadGet() & PAD_RI) {	// ���������Ă����readScript���ČĂяo���i�X�L�b�v�\���j
		return 1;
	}

UPDATE:
	ld_VBuffUpdate();
	return 0;
}
