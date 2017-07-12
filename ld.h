/*
 *	16�K���\���֘A���C�u����
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/02/04	����
 *	2005/02/17	ld_DrawObject�ŏ㉺���E�ɂ͂ݏo�����ꍇ�̏��������
 *	2005/02/22	ld_LCDPoint�Ald_LCDTransDirect��ǉ�
 *	2005/03/11	16�K���摜��lbuff�ɕ`���āAlbuff�A�t�B���^�Avbuff�̏���
 *				�X�V�t���O�ɉ�����1������1��ʂ���dbuff�ɕ`�悷��悤�ɂ��܂����B
 *				128*88=11,264bytes�]�v�Ƀ�������H���܂����A�`�������������ł��B
 *				ld_Clear��ld_LCDPaint
 *	2005/05/20	ld_DrawObject��x=8�̔{�����������
 *				ld_DrawObject�Ń}�X�N�t���`�掞�Ƀo�b�t�@��1�h�b�g���炵�Ă��܂��Ă����s����C��
 *				ld_VBuffCopy��vbuff�ɕ`�����t�H���g�ɃO���[��������s����C���H
 */

#ifndef __LDIRECT_H__
#define __LDIRECT_H__

#include <piece.h>
#include <string.h>

/*
 *	void pceAppInit(void)�̐擪��
 *	FRAMCOPY(V_FRAMC, fram_ld_code);
 *	�Ƃ���flam_ld.c�̃R�[�h������RAM�ɃR�s�[����B
 *	�܂��Amakefile��LDFLAGS��
 *	+codeblock FRAMC {fram_ld.o} \
 *	+bssblock FRAMB {fram_ld.o} \
 *	+addr 0x1000 {@FRAMC FRAMB}
 *	�������邱�ƁB		
 */
#define FRAMCOPY(dstsect, srcsect)\
	do {\
		extern unsigned long __START_##dstsect[];\
		extern unsigned long __START_##srcsect[];\
		extern unsigned long __SIZEOF_##srcsect[];\
		memcpy(&__START_##dstsect, &__START_##srcsect, (unsigned)&__SIZEOF_##srcsect);\
	} while(0)

#define PAGE_NUM	5
#define COLOR_MAX	15
#define V_TRANS		0x04
extern BYTE vbuff[DISP_X * DISP_Y];
extern BYTE lbuff[DISP_X * DISP_Y];
extern BYTE dbuff[][DISP_X * DISP_Y >> 2];

/* fram_ld.c */
void ld_LBuffUpdate();
void ld_VBuffClear(int x, int y, int w, int h);
void ld_VBuffUpdate();
//void ld_FilterDark();
void ld_LCDTransDirect();
void ld_DrawObject(const PIECE_BMP *pbmp, int dx, int dy);
void ld_LCDPoint(const BYTE c, const int x, const int y);
void ld_LCDPaint(BYTE c, int x, int y, int w, int h);

/* ld.c */
void Get_PieceBmp(PIECE_BMP *pbmp, BYTE *data);
//void Draw_Object(PIECE_BMP *pbmp, const int dx, const int dy);
//void VBuffReverse();


/*
 *	�\���̂�PIECE_BMP�����̂܂܎g���܂��B

typedef struct {
	DWORD	head;		//	HEADER			'PBEX'�i�����A�v���̌듮��h�~
	DWORD	fsize;		//	�t�@�C����
	BYTE	bpp;		//	bit�[�x			(4)
	BYTE	mask;		//	�}�X�N��bit�[�x	(1)
	int	w;			//	X��				8�s�N�Z���P�ʌ���
	int	h;			//	Y����
	DWORD	buf_size;	//	BMP�T�C�Y = (bpp + mask) * w * h / 8
}PBMP_FILEHEADER;

typedef struct{
	PBMP_FILEHEADER	header;
	BYTE			*buf;	//4BIT 1�s�N�Z��
	BYTE			*mask;	//1BIT 1�s�N�Z��
}PIECE_BMP;

 */

#endif	// #ifndef __LDIRECT_H__
