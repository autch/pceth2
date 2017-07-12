/*
	* libfpk -- using FPK archive on P/ECE
	* 	By Yui N., 2003.
	* http://www.autch.net/
*/

#include <piece.h>
#include <muslib.h>
#include <string.h>
#include "../libfpk.h"

// ���z���
unsigned char g_vScreen[128 * 88];
// �_�[�e�B�t���O
unsigned char g_fDraw;

// par ����W�J���� pmd �f�[�^������
unsigned char *g_pSeq = NULL;
// par �t�@�C���n���h��
HFPK g_hFpk = NULL;
// �Đ����̃t�@�C���G���g���ƁA�I�𒆂̃t�@�C���G���g��
FPKENTRY g_fpkPlaying, g_fpkSelection;
// �I�� par �t�@�C���C���f�b�N�X�ƁA�z�[���h�t���O
int g_nMusicIndex, g_fHold;

// �������Ă���Ȗ���\��
void ShowNowPlaying();
// ���S�������ɋȂ��~�߂�
void musStopImmediately();
// ���I�𒆂̋Ȃ�\��
void PrintSelection();
// ���g�p�F���͂� pmd ����^�C�g��������Ă���
void ReadTitle(unsigned char* pSeq, char* szTitle, int nTitleSize,
                                    char* szTitle2, int nTitle2Size);
// par �p PlayMusic()
void fpkPlayMusic(int nIndex);

// �Ӗ��� VB �Ƃ� BASIC �Ɠ��� :)
void Cls()
{
	memset(g_vScreen, 0, 128 * 88);
	pceFontSetPos(0, 0);
	g_fDraw = 1;
}

// �Ӗ��� VB �Ɠ��� :)
void Refresh()
{
	if(g_fDraw)
	{
		pceLCDTrans();
		g_fDraw = 0;
	}
}

void pceAppInit()
{
	pceLCDDispStop();
	pceLCDSetBuffer(g_vScreen);
	pceAppSetProcPeriod(50);

	InitMusic();
	Cls();

	g_nMusicIndex = 0;
	g_fHold = 0;

	if((g_hFpk = fpkOpenArchive("mypmds.par")))
	{
		// par �̍ŏ��̃t�@�C�����Ƃ��Ă���
		fpkGetFileInfoN(g_hFpk, g_nMusicIndex, &g_fpkPlaying);
		g_fpkSelection = g_fpkPlaying;

		fpkPlayMusic(g_nMusicIndex);

		pceLCDDispStart();
	}
}

void pceAppProc(int cnt)
{
	// par ���J�����Ȃ��Ă����狭���I��
	if(!g_hFpk) pceAppReqExit(-1);

	// START �{�^��
	if(pcePadGet() & TRG_C)
	{
		if(!g_fHold)
			pceLCDDispStop();
		else
			pceLCDDispStart();
		g_fHold ^= 1;
	}
	// �z�[���h��ԂłȂ����
	if(!g_fHold)
	{
		if(pcePadGet() & TRG_A)
		{
			musStopImmediately();
			fpkPlayMusic(g_nMusicIndex);
		}
		if(pcePadGet() & TRG_B)
		{
			musStopImmediately();
			Cls();
			PrintSelection();
		}
		if(pcePadGet() & TRG_LF)
		{
			if(g_nMusicIndex > 0)	g_nMusicIndex--;
			fpkGetFileInfoN(g_hFpk, g_nMusicIndex, &g_fpkSelection);
			PrintSelection();
		}
		if(pcePadGet() & TRG_RI)
		{
			if(g_nMusicIndex < g_hFpk->fpkHeader.dwFilesAmount - 1)	g_nMusicIndex++;
			fpkGetFileInfoN(g_hFpk, g_nMusicIndex, &g_fpkSelection);
			PrintSelection();
		}
	}

	pceAppActiveResponse(MusicCheck() ? AAR_NOACTIVE : AAR_ACTIVE);

	Refresh();
}

void pceAppExit()
{
	musStopImmediately();
	fpkCloseArchive(g_hFpk);
}

void PrintSelection()
{
	pceFontSetPos(0, 0);	pceFontPrintf("seek: %-16s", g_fpkSelection.szFileName);
	g_fDraw = 1;
}

void musStopImmediately()
{
	// �܂��h���C�o���~�߂�
	StopMusic();
	// �Đ��҂��o�b�t�@���Ȃ��Ȃ�̂�҂�
	while(pceWaveCheckBuffs(music_wch));
	// �J�[�l���ɒ�~�v�����o��
	pceWaveAbort(music_wch);

	// �V�[�P���X�̉��
	if(g_pSeq)
	{
		pceHeapFree(g_pSeq);
		g_pSeq = NULL;
	}
}

void ShowNowPlaying()
{
	Cls();

	pceLCDLine(3, 0, 10, 127, 10);
	pceFontSetPos(0, 12); pceFontPrintf("playing: %-16s", g_fpkPlaying.szFileName);
	pceLCDLine(3, 0, 22, 127, 22);
	pceFontSetPos(0, 24); pceFontPrintf(title);
	pceLCDLine(3, 0, 46, 127, 46);
	pceFontSetPos(0, 48); pceFontPrintf(title2);

	PrintSelection();

	g_fDraw = 1;
}

// par �p PlayMusic()
void fpkPlayMusic(int nIndex)
{
	musStopImmediately();
	fpkGetFileInfoN(g_hFpk, nIndex, &g_fpkPlaying);
	if(strcmp(strrchr(g_fpkPlaying.szFileName, '.'), ".pmd") == 0)
	{
		g_pSeq = fpkExtractToBuffer(g_hFpk, &g_fpkPlaying);
		PlayMusic(g_pSeq);
		ShowNowPlaying();
	}
}

#define AS_WORD(p) ((WORD)*((WORD*)p))

// ���g�p�F���͂� pmd ����^�C�g��������Ă���
void ReadTitle(unsigned char* pSeq, char* szTitle, int nTitleSize,
                                    char* szTitle2, int nTitle2Size)
{
	BYTE* p = pSeq;

	// db 0
	if(!*p) p++;

  // partn X
	p += *p++ << 1;

	p += 2;

	if(AS_WORD(p))
		strncpy(szTitle, pSeq + AS_WORD(p), nTitleSize);
	p += 2;
	if(AS_WORD(p))
		strncpy(szTitle2, pSeq + AS_WORD(p), nTitle2Size);
}
