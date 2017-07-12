/*
 *	th2bm2cmv
 *	ToHeart2���甲���o����8�^32bitBM2�t�@�C����BMP�t�@�C���ɕϊ�
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/02/20	����
 *				�����̃��C���h�J�[�h�Ή��isetargv.obj�������N�j
 *	2005/02/25	�֐��v���g�^�C�v�錾���폜�Ausage���C���i���̃v���O��������̃R�s�y���΂�邪�ȁj
 *				�g���q��".BM2"�ɐ����i�ԈႦ��".bmp"���ĕϊ����Ă��܂��Ă������߁j
 *				8bit�摜�̃r�b�g�}�b�v�o�͕���bit�����ւ���悤�C��
 *
 *	MEMO : BM2�`���ɂ���
 *	��{�I��Windows DIB�B
 *	�㉺���]�B
 *	8bit�̓p���b�g�́A32bit�̓r�b�g�}�b�v��R��B������ւ���Ă���B
 *	�\��r�b�g�����l�Ɏg�p�i0x00�`0x80�j
 *	8bit�̓r�b�g�}�b�v�̑�4bit�Ƒ�5bit������ւ���Ă���B
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[])
{
	FILE	*fpin = NULL, *fpout = NULL;
	BITMAPFILEHEADER	bf;
	BITMAPINFOHEADER	bi;
	char	path[_MAX_PATH];
	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];
	BYTE	buf, a, r, g, b;

	if(argc == 1) {	// usage
		fprintf(stderr, "ToHeart2 BM2�t�@�C����8�^32bitBMP�R���o�[�^\n");
		fprintf(stderr, "th2bm2cmv filename ..\n");
		return 1;
	}

	for(int i = 1; i < argc; i++) {
		_splitpath(argv[i], drive, dir, fname, ext);
		if(strcmp(ext, ".BM2"))	goto ERR;
		if((fpin = fopen(argv[i], "rb")) == NULL)	goto ERR;
		// BITMAPFILEHEADER
		if(fread(&bf, 1, sizeof(BITMAPFILEHEADER), fpin) != sizeof(BITMAPFILEHEADER))	goto ERR;
		if(strncmp((char*)&bf.bfType, "BM", 2))	goto ERR;
		// BITMAPINFOHEADER
		if(fread(&bi, 1, sizeof(BITMAPINFOHEADER), fpin) != sizeof(BITMAPINFOHEADER))	goto ERR;
//		if(bi.biSize != sizeof(BITMAPINFOHEADER))	goto ERR;
		if(bi.biBitCount != 32 && bi.biBitCount != 8)	goto ERR; // 8�^32bit
		fseek(fpin, 0x40, SEEK_SET);

		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biHeight= -bi.biHeight;	// �㉺���]
		bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		if(bi.biBitCount == 8)	bf.bfOffBits += sizeof(RGBQUAD) * 256;
		bf.bfSize = bf.bfOffBits + bi.biWidth * abs(bi.biHeight) * bi.biBitCount / 8;
//		bf.bfReserved1 = bf.bfReserved2 = 0;	// �`��I�t�Z�b�g���߂�d�v���l�Ȃ̂Ń��Z�b�g���Ȃ�
		_makepath(path, drive, dir, fname, ".bmp");
		if((fpout = fopen(path, "wb")) == NULL)	goto ERR;
		fwrite(&bf, 1, sizeof(BITMAPFILEHEADER), fpout);
		fwrite(&bi, 1, sizeof(BITMAPINFOHEADER), fpout);
		
		if(bi.biBitCount == 32) {	// 32bit
			for(int j = 0; j < bi.biWidth * abs(bi.biHeight); j++) {
				b = fgetc(fpin);	g = fgetc(fpin);	r = fgetc(fpin);	a = fgetc(fpin);
				if(a & 0x80) {	// ���l�␳
					a = 0xFF;
				} else {
					a <<= 1;
				}
				fputc(r, fpout);	fputc(g, fpout);	fputc(b, fpout);	fputc(a, fpout);	// RB����ւ�
			}
		} else {					// 8bit
			for(int j = 0; j < 256; j++) {
				b = fgetc(fpin);	g = fgetc(fpin);	r = fgetc(fpin);	a = fgetc(fpin);
				if(a & 0x80) {	// ���l�␳
					a = 0xFF;
				} else {
					a <<= 1;
				}
				fputc(r, fpout);	fputc(g, fpout);	fputc(b, fpout);	fputc(a, fpout);	// RB����ւ�
			}
			for(int j = 0; j < bi.biWidth * abs(bi.biHeight); j++) {
				buf = fgetc(fpin);
				if((buf & 0x10) ^ ((buf & 0x08) << 1))	buf ^= 0x18;	// ��4bit�Ƒ�5bit�����ւ�
				fputc(buf, fpout);
			}
		}
		printf("%s - �������܂����B\n", argv[i]);
		goto FREE;
ERR:
		fprintf(stderr, "%s - ���s���܂����B\n", argv[i]);
FREE:
		if(fpout != NULL)	fclose(fpout);
		if(fpin  != NULL)	fclose(fpin);
	}

	return 0;
}
