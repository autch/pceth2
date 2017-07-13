/*
 *	�t�H���g�g�����C�u����
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/05/01	�n�[�g�}�[�N�̃t�H���g���C��
 *				�W���t�H���g�̉���������
 *	2005/06/15	sFontStatus�̏����l��ύX
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <piece.h>
#include <smcvals.h>

#include "font_ex.h"

// 0xF042: �n�[�g�}�[�N
static const unsigned char __font_heart[] = {
	0x00,0x03,0x18, 0x7b,0xc7,0xfc, 0x3f,0x83,0xf8,
	0x1f,0x00,0xe0, 0x04,0x00,0x00, 0x00,0x00,0x00,
};
#define FONT_HEART ((unsigned char *)__font_heart)

/*
 *	pceFontGetAdrs�ɓ��ꕶ����ǉ����邽�߂̃��[�`��
 */
#define KSNO_FontGetAdrs       24
const unsigned char *(*old_pceFontGetAdrs)(unsigned short code);
const unsigned char *new_pceFontGetAdrs(unsigned short code)
{
	unsigned char h = code>>8;
	unsigned char l = code;

	if (h == 0xf0 && l == 0x42) {
		return FONT_HEART;
	}

	return old_pceFontGetAdrs(code);
}
void hook_FontGetAdrs() { old_pceFontGetAdrs = pceVectorSetKs(KSNO_FontGetAdrs, new_pceFontGetAdrs); }
void unhook_FontGetAdrs() { pceVectorSetKs(KSNO_FontGetAdrs, old_pceFontGetAdrs); }


/*
 *	�i�ȉ��AGENGE�����sfont.c�����ρj
 */

//SFONT sFontStatus = { 0,0,0,127,0,87,0 };	//8*10�^�C�vFONT�̕\���ݒ�
SFONT sFontStatus = {4, 4, 3, 124, 3, 84, 7};

const unsigned short SFONT_TBL3[32] = {		//�t�H���g�����p�e�[�u��
	0x0000, 0x0070, 0x0038, 0x0078,
	0x001c, 0x007c, 0x003c, 0x007c,
	0x000e, 0x007e, 0x003e, 0x007e,
	0x001e, 0x007e, 0x003e, 0x007e,
	0x0007, 0x0077, 0x003f, 0x007f,
	0x001f, 0x007f, 0x003f, 0x007f,
	0x000f, 0x007f, 0x003f, 0x007f,
	0x001f, 0x007f, 0x003f, 0x007f
};

unsigned short sFontPutFuchi(int x, int y, unsigned short code);
unsigned short sFontPutFuchi( int x, int y, unsigned short code )
{
	int i,j,xe;
	int xs = (x<1)?-x+1:0;
	int ys = (y<1)?-y+1:0;
	int ye = (y>77)?89-y:12;
	const unsigned char *p = pceFontGetAdrs( code );
	unsigned char *q = pceLCDSetBuffer(INVALIDPTR)+((y+ys-1)<<7);
	char wkcolor = (sFontStatus.spr&2)?3:0;
	unsigned short fsv[12],fsvb,ret;
	
	fsv[0] = 0;
	fsv[1] = 0;
	if ( (int)p & 0x80000000 ) {//���p
		xe = (x>122)?129-x:7;
		ret = (10<<8)|5;
		
		//��ʔ͈͓��ɏo�͂��������I��
		if ( xs >= xe || ys >= ye ) return ret;
		
		//�����p�̈�쐬
		for ( j = 0 ; j < 10 ; j += 1 ) {
			fsv[j+2] = SFONT_TBL3[*p++>>3];
			fsv[j] |= fsv[j+2];
			fsv[j+1] |= fsv[j+2];
		}
	}
	else {//�S�p
		unsigned long sv;
		unsigned char *svp = (unsigned char *)(&sv);
		xe = (x>117)?129-x:12;
		ret = (10<<8)|10;
		
		//��ʔ͈͓��ɏo�͂��������I��
		if ( xs >= xe || ys >= ye ) return ret;
		
		//�����p�̈�쐬
		for ( j = 0 ; j < 10 ; j += 2 ) {
			svp[0] = p[2];
			svp[1] = p[1];
			svp[2] = p[0];
			svp[3] = 0;
			
			//�S�p�Q�s��
			sv>>=2;
			fsv[j+3] = SFONT_TBL3[sv & 0x1f]<<5;
			sv>>=5;
			fsv[j+3] |= SFONT_TBL3[sv & 0x1f];
			
			//�S�p�P�s��
			sv>>=7;
			fsv[j+2] = SFONT_TBL3[sv & 0x1f]<<5;
			sv>>=5;
			fsv[j+2] |= SFONT_TBL3[sv];
			
			fsv[j] |= fsv[j+2];
			fsv[j+1] |= fsv[j+2]|fsv[j+3];
			fsv[j+2] |= fsv[j+3];
			
			p+=3;
		}
	}
	
	//�����`��
	for ( j = ys ; j < ye ; j += 1 ) {
		fsvb = fsv[j]>>xs;
		for ( i = xs ; i < xe ; i += 1 ) {
			if ( fsvb&1 ) q[x-1+i] = wkcolor;
			fsvb >>= 1;
		}
		q+=128;
	}
	
	return ret;
}

unsigned short sFontPut( int x, int y, unsigned short code )
{
	pceFontSetType(0);
	//�����`��
	if ( sFontStatus.spr&4 ) sFontPutFuchi( x, y, code );
	
	//�����`��
//	return sFontPutMoji( x, y, code );
	pceFontSetTxColor((sFontStatus.spr&2)?0:3);
	pceFontSetBkColor(FC_SPRITE);
	return pceFontPut(x, y, code);
}

char *sFontPutStrEx( char Type, const char *p )
{
	union {
		unsigned short s;
		struct {
			unsigned char w;
			unsigned char h;
		} b;
	} m;
	unsigned char c1;
	char *rpt = NULL;
	m.s = 0;

	pceFontSetType(0);
	while (1) {
		c1 = *p++;
		if (!c1) break;
		if ((c1 >= 0x81 && c1 <= 0x9f) || (c1 >= 0xe0 && c1 <= 0xfc)) {
			if (*p) {
				unsigned char c2 = *p++;
				if (Type) {
					pceFontSetTxColor((sFontStatus.spr&2)?0:3);
					pceFontSetBkColor(FC_SPRITE);
					m.s = pceFontPut(sFontStatus.x, sFontStatus.y, (c1 << 8) + c2);
				} else {
					m.s = sFontPutFuchi(sFontStatus.x, sFontStatus.y, (c1 << 8) + c2);
				}
			}
			else {
				break;
			}
		}
		else if (c1 == '\r') {
			if (*p == '\n') p+=1;
			goto CRLF;
		}
		else if (c1 == '\n') {
			goto CRLF;
		}
		else {
			if (Type) {
				pceFontSetTxColor((sFontStatus.spr&2)?0:3);
				pceFontSetBkColor(FC_SPRITE);
				m.s = pceFontPut(sFontStatus.x, sFontStatus.y, c1);
			} else {
				m.s = sFontPutFuchi(sFontStatus.x, sFontStatus.y, c1);
			}
		}
		sFontStatus.x += m.b.w;
		c1 = *p;
		if (sFontStatus.x + (((c1 >= 0x81 && c1 <= 0x9f) || (c1 >= 0xe0 && c1 <= 0xfc))? 9 : 4) > sFontStatus.xMax) {
CRLF:
			sFontStatus.x = sFontStatus.xMin;
			sFontStatus.y += 10;
			if (sFontStatus.y+9 > sFontStatus.yMax) {
				sFontStatus.y = sFontStatus.yMin;
				if (*p) rpt = (char *)p;
				break;
			}
		}
	}
	return rpt;
}

char *sFontPutStr(const char *p)
{
	char *ret;
	SFONT bkFont = sFontStatus;

	bkFont.x += ( sFontStatus.x==sFontStatus.xMin );
	bkFont.y += ( sFontStatus.y==sFontStatus.yMin );
	sFontStatus.xMin += 1;
	sFontStatus.xMax -= 1;
	sFontStatus.yMin += 1;
	sFontStatus.yMax -= 1;

	sFontStatus.x = bkFont.x;
	sFontStatus.y = bkFont.y;
	sFontPutStrEx(0, p);
	sFontStatus.x = bkFont.x;
	sFontStatus.y = bkFont.y;
	
	ret = sFontPutStrEx(1, p);
	bkFont.x = sFontStatus.x;
	bkFont.y = sFontStatus.y;
	sFontStatus = bkFont;
	
	return ret;
}

// �ȉ� nsawa����́w�ԊO��XMODEM�x ��蔲��

int sFontPrintf(const char* fmt, ...)
{
extern unsigned char _def_vbuff[]; /* ������W�J�ɗ��p */
	int result;
	va_list ap;

	va_start(ap, fmt);
	result = vsprintf(_def_vbuff, fmt, ap);
	va_end(ap);

	sFontPutStr(_def_vbuff);

	return result;
}
