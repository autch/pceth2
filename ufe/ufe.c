/*
 *	ufe.c - UFE �N���C�A���g���C�u����
 *
 *	UFE - P/ECE USB File System Emulation
 *
 *	* Wed Jun 08 20:00:00 JST 2005 Naoyuki Sawa
 *	- �V�K�쐬�B
 */
#include <piece.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "ufe.h"

/*****************************************************************************
 *	���[�J���ϐ�
 *****************************************************************************/

static UFE ufe;

static int (*pffsFileFindOpen)(FILEINFO* pfi);
static int (*pffsFileFindNext)(FILEINFO* pfi);
static int (*pffsFileFindClose)(FILEINFO* pfi);
static int (*pffsFileOpen)(FILEACC* pfa, const char* fname, int mode);
static int (*pffsFileReadSct)(FILEACC* pfa, void* ptr, int sct, int len);
static int (*pffsFileWriteSct)(FILEACC* pfa, const void* ptr, int sct, int len);
static int (*pffsFileClose)(FILEACC* pfa);
static int (*pffsFileCreate)(const char* fname, unsigned long size);

/*****************************************************************************
 *	���[�J���֐�
 *****************************************************************************/

static int
ufe_exec(int ksno)
{
	/* �J�[�l���T�[�r�X�ԍ����i�[���܂��B
	 * ���ꂪ�A�t�@�C���T�[�o�ւ̃T�[�r�X�v���ʒm�ƂȂ�܂��B
	 * �u�p�����[�^�i�[���J�[�l���T�[�r�X�ԍ��i�[�v�̏��Ԃ����炵�Ă��������B
	 */
	ufe.ksno = ksno;

	/* �t�@�C���T�[�o�̃T�[�r�X��������҂��܂��B
	 * �t�@�C���T�[�o�́A�T�[�r�X����������ƃJ�[�l���T�[�r�X�ԍ����N���A���܂��B
	 */
	while(ufe.ksno) {
		/* �t�@�C���T�[�o�Ƃ̐ڑ����ؒf����Ă��܂�����A�G���[�����Ƃ��܂��B */
		if(!(pceUSBCOMGetStat() & (1<<24))) {
			return -1;
		}
	}
	return ufe.result;
}

/*---------------------------------------------------------------------------*/

static int
ufeFileFindOpen(FILEINFO* pfi)
{
	ufe.api.find_open.pfi = pfi;
	return ufe_exec(KSNO_FileFindOpen);
}

static int
ufeFileFindNext(FILEINFO* pfi)
{
	ufe.api.find_next.pfi = pfi;
	return ufe_exec(KSNO_FileFindNext);
}

static int
ufeFileFindClose(FILEINFO* pfi)
{
	ufe.api.find_close.pfi = pfi;
	return ufe_exec(KSNO_FileFindClose);
}

static int
ufeFileOpen(FILEACC* pfa, const char* fname, int mode)
{
	ufe.api.open.pfa = pfa;
	ufe.api.open.fname = fname;
	ufe.api.open.mode = mode;
	return ufe_exec(KSNO_FileOpen);
}

static int
ufeFileReadSct(FILEACC* pfa, void* ptr, int sct, int len)
{
	ufe.api.read_sct.pfa = pfa;
	ufe.api.read_sct.ptr = ptr;
	ufe.api.read_sct.sct = sct;
	ufe.api.read_sct.len = len;
	return ufe_exec(KSNO_FileReadSct);
}

static int
ufeFileWriteSct(FILEACC* pfa, const void* ptr, int sct, int len)
{
	ufe.api.write_sct.pfa = pfa;
	ufe.api.write_sct.ptr = ptr;
	ufe.api.write_sct.sct = sct;
	ufe.api.write_sct.len = len;
	return ufe_exec(KSNO_FileWriteSct);
}

static int
ufeFileClose(FILEACC* pfa)
{
	ufe.api.close.pfa = pfa;
	return ufe_exec(KSNO_FileClose);
}

static int
ufeFileCreate(const char* fname, unsigned long size)
{
	ufe.api.create.fname = fname;
	ufe.api.create.size = size;
	return ufe_exec(KSNO_FileCreate);
}

/*---------------------------------------------------------------------------*/

int
myFileFindOpen(FILEINFO* pfi)
{
	int retval;

	retval = pffsFileFindOpen(pfi);
	if(retval == 0) {
		pfi->works[15] = FILEINFO_PFFS;
	} else {
		retval = ufeFileFindOpen(pfi);
		if(retval == 0) {
			pfi->works[15] = FILEINFO_UFE;
		} else {
			memset(pfi, 0, sizeof(FILEINFO));
		}
	}

	return retval;
}

int
myFileFindNext(FILEINFO* pfi)
{
	int retval = 0;

	if(pfi->works[15] == FILEINFO_PFFS) {
		retval = pffsFileFindNext(pfi);
		if(!retval) {
			pffsFileFindClose(pfi);
			if(ufeFileFindOpen(pfi) == 0) {
				pfi->works[15] = FILEINFO_UFE;
			} else {
				memset(pfi, 0, sizeof(FILEINFO));
			}
		}
	}
	if(pfi->works[15] == FILEINFO_UFE) {
		retval = ufeFileFindNext(pfi);
	}

	return retval;
}

int
myFileFindClose(FILEINFO* pfi)
{
	int retval;

	switch(pfi->works[15]) {
	case FILEINFO_PFFS:
		retval = pffsFileFindClose(pfi);
		break;
	case FILEINFO_UFE:
		retval = ufeFileFindClose(pfi);
		break;
	default:
		retval = -1;
		break;
	}
	if(retval == 0) {
		memset(pfi, 0, sizeof(FILEINFO));
	}

	return retval;
}

int
myFileOpen(FILEACC* pfa, const char* fname, int mode)
{
	int retval;

	retval = pffsFileOpen(pfa, fname, mode);
	if(retval != 0) {
		retval = ufeFileOpen(pfa, fname, mode);
		if(retval != 0) {
			memset(pfa, 0, sizeof(FILEACC));
		}
	}

	return retval;
}

int
myFileReadSct(FILEACC* pfa, void* ptr, int sct, int len)
{
	int retval;

	switch(pfa->valid) {
	case FILEACC_PFFS:
		retval = pffsFileReadSct(pfa, ptr, sct, len);
		break;
	case FILEACC_UFE:
		retval = ufeFileReadSct(pfa, ptr, sct, len);
		break;
	default:
		retval = 0;
		break;
	}

	return retval;
}

int
myFileWriteSct(FILEACC* pfa, const void* ptr, int sct, int len)
{
	int retval;

	switch(pfa->valid) {
	case FILEACC_PFFS:
		retval = pffsFileWriteSct(pfa, ptr, sct, len);
		break;
	case FILEACC_UFE:
		retval = ufeFileWriteSct(pfa, ptr, sct, len);
		break;
	default:
		retval = 0;
		break;
	}

	return retval;
}

int
myFileClose(FILEACC* pfa)
{
	int retval;

	switch(pfa->valid) {
	case FILEACC_PFFS:
		retval = pffsFileClose(pfa);
		break;
	case FILEACC_UFE:
		retval = ufeFileClose(pfa);
		break;
	default:
		retval = -1;
		break;
	}
	if(retval == 0) {
		memset(pfa, 0, sizeof(FILEACC));
	}

	return retval;
}

int
myFileCreate(const char* fname, unsigned long size)
{
	int retval;

	retval = pffsFileCreate(fname, size);
	if(retval != 0) {
		retval = ufeFileCreate(fname, size);
	}

	return retval;
}

/*---------------------------------------------------------------------------*/

static void
ufe_hook()
{
	if(!pffsFileFindOpen) {
		pffsFileFindOpen  = pceVectorSetKs(KSNO_FileFindOpen , myFileFindOpen );
		pffsFileFindNext  = pceVectorSetKs(KSNO_FileFindNext , myFileFindNext );
		pffsFileFindClose = pceVectorSetKs(KSNO_FileFindClose, myFileFindClose);
		pffsFileOpen      = pceVectorSetKs(KSNO_FileOpen     , myFileOpen     );
		pffsFileReadSct   = pceVectorSetKs(KSNO_FileReadSct  , myFileReadSct  );
		pffsFileWriteSct  = pceVectorSetKs(KSNO_FileWriteSct , myFileWriteSct );
		pffsFileClose     = pceVectorSetKs(KSNO_FileClose    , myFileClose    );
		pffsFileCreate    = pceVectorSetKs(KSNO_FileCreate   , myFileCreate   );
	}
}

static void
ufe_unhook()
{
	if(pffsFileFindOpen) {
		pceVectorSetKs(KSNO_FileFindOpen , pffsFileFindOpen ); pffsFileFindOpen  = NULL;
		pceVectorSetKs(KSNO_FileFindNext , pffsFileFindNext ); pffsFileFindNext  = NULL;
		pceVectorSetKs(KSNO_FileFindClose, pffsFileFindClose); pffsFileFindClose = NULL;
		pceVectorSetKs(KSNO_FileOpen     , pffsFileOpen     ); pffsFileOpen      = NULL;
		pceVectorSetKs(KSNO_FileReadSct  , pffsFileReadSct  ); pffsFileReadSct   = NULL;
		pceVectorSetKs(KSNO_FileWriteSct , pffsFileWriteSct ); pffsFileWriteSct  = NULL;
		pceVectorSetKs(KSNO_FileClose    , pffsFileClose    ); pffsFileClose     = NULL;
		pceVectorSetKs(KSNO_FileCreate   , pffsFileCreate   ); pffsFileCreate    = NULL;
	}
}

/*****************************************************************************
 *	�O���[�o���֐�
 *****************************************************************************/

int
ufe_setup()
{
	extern unsigned char _def_vbuff[DISP_X * DISP_Y];
	//
	int ucs;
	USBCOMINFO uci;
	unsigned char* save_vbuff;

	/* �܂��A�m���ɐؒf���Ă����܂��B */
	ufe_stop();

	/* USBCOM�����������A�t�@�C���T�[�o����̐ڑ���҂��܂��B
	 * USBCOM�V�O�l�`���̏����́A���̂Ƃ���ł��B
	 *	+0- 3:	"UFE1"
	 *	+4- 7:	UFE�\���̂̃A�h���X
	 *	+8-15:	���g�p
	 */
	memset(&uci, 0, sizeof uci);
	memcpy(&uci.signature[0], "UFE1", 4);
	*(int*)&uci.signature[4] = (int)&ufe;
	pceUSBCOMSetup(&uci);

	/* �ڑ��҂����b�Z�[�W��\�����܂��B */
	save_vbuff = pceLCDSetBuffer(_def_vbuff);
	pceLCDDispStart();
	pceFontSetType(0x80);
	pceFontSetTxColor(3);
	pceFontSetBkColor(0);
	pceFontSetPos(0, (DISP_Y - 10 * 5) / 2);
	/*             �O�P�Q�R�S�T�U�V�W�X�O�P�Q */
	pceFontPutStr(" +----------------------+\n"
	              " | �t�@�C���T�[�o����� |\n"
	              " | �ڑ���҂��Ă��܂��c |\n"
	              " | (SELECT�ŃL�����Z��) |\n"
	              " +----------------------+");
	pceLCDTrans();

	/* �t�@�C���T�[�o����̐ڑ����A�܂��̓L�����Z����҂��܂��B */
	while(!((ucs = pceUSBCOMGetStat()) & (1<<24))) {
		/* SELECT�{�^���������ꂽ��A�������̂�҂��Ă��甲���܂��B */
		if(pcePadGetDirect() & PAD_SELECT) {
			while(pcePadGetDirect() & PAD_SELECT) {
				/** no job **/
			}
			break;
		}
	}

	/* ���z��ʂ����ɖ߂��܂��B */
	pceLCDSetBuffer(save_vbuff);

	/* �ڑ������Ȃ�΁A�t�@�C��API���t�b�N���A0��Ԃ��܂��B */
	if(ucs & (1<<24)) {
		ufe_hook();
		return 0;
	}

	/* �ڑ����s�Ȃ�΁A-1��Ԃ��܂��B
	 * ���ԍ��Őڑ����Ă��܂��Ă���\��������̂ŁA�m����USBCOM���I�����܂��B
	 */
	ufe_stop();
	return -1;
}

void
ufe_stop()
{
	USBCOMINFO uci;

	/* �t�@�C��API���t�b�N����Ă�����A�������܂��B */
	ufe_unhook();

	/* pceUSBCOMStop()�̓V�O�l�`�����N���A���Ȃ��̂ŁA�����I�ɃN���A���܂��B */
	memset(&uci, 0, sizeof uci);
	pceUSBCOMSetup(&uci);

	/* USBCOM���I�����܂��B */
	pceUSBCOMStop();
}
