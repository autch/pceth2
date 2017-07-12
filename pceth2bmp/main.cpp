/*
 *	pceth2bmp
 *	ToHeart2���甲���o�������l��8bit�^24bit�^���l��32bitBMP��16�K��PGD�ϊ��p�̃O���[�X�P�[��BMP�ɕϊ�
 *	�T�C�Y�w���A���������v���O�����ɂ��܂�
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/02/16	����
 *	2005/02/17	grayscale�֐��ɂ����ă��l��臒l��0x80�ɕύX
 *				24bitBMP�ɂ��Ή��i32bitBMP�̃v���O�C����Susie�ŃT���l�C���m�F�ł��Ȃ��̂Łj
 *				��2005/02/25	���ڐ؂�o����悤�ɂȂ����̂ŕs�v(^^;
 *	2005/02/20	�����̃��C���h�J�[�h�Ή��isetargv.obj�������N�j
 *	2005/02/25	�g�b�v�_�E���`���̏ꍇmalloc�ŃG���[�ɂȂ��Ă����̂��C��
 *				8bitBMP�ɂ��Ή��i�k�����ɂ��Ă͍l�����Ă��Ȃ��j
 *	2005/04/23	640pixel�ȊO�i�ꕔ�����������G�j�̓g���~���O�ŗ����G�̏d�S�������ɗ���悤�ɏC��
 *	2005/04/24	BITMAPFILEHEADER��bfReserved1�����ė����G�̏d�S��␳�i���͊ԈႢ�ł����j
 *	2005/05/10	�`�b�v�L�����Ȃǂ̏k������1/2��
 *	2005/05/21	��L�`�b�v�L�����ȂǈȊO�ɂ��āA�g���~���O�̃A���C�������g��8*5��
 *				�ild���C�u������8alignment�������ꂽ�̂ŁApgx�d�l�̍ŏ��T�C�Y�j
 *
 *	MEMO : �����G�̘b
 *	���l��臒l�������Ɣ��̖т������Ă��܂����A�Ⴂ�Ɨ֊s�������o�Ă��܂����A����B
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

BYTE *bmp8to32(BYTE *, BITMAPINFOHEADER *, RGBQUAD *);
BYTE *bmp24to32(BYTE *, BITMAPINFOHEADER *);
BYTE *trim0(BYTE *, BITMAPFILEHEADER *, BITMAPINFOHEADER *);
BYTE *trim1(BYTE *, BITMAPINFOHEADER *);
BYTE *resize(BYTE *, BITMAPINFOHEADER *, const int);
BYTE *grayscale(BYTE *, BITMAPINFOHEADER *);

int main(int argc, char *argv[])
{
	FILE	*fp = NULL;
	BITMAPFILEHEADER	bf;
	BITMAPINFOHEADER	bi;
	RGBQUAD	rgb[256];
	long	line_bytes;
	BYTE	*bmp = NULL;

	if(argc == 1) {	// usage
		fprintf(stderr, "ToHeart2 24�^32bitBMP��16�K���{�����F128�~88BMP�R���o�[�^\n");
		fprintf(stderr, "pceth2bmp filename ..\n");
		return 1;
	}

	for(int i = 1; i < argc; i++) {
		if((fp = fopen(argv[i], "rb")) == NULL)	goto ERR;
		// BITMAPFILEHEADER
		if(fread(&bf, 1, sizeof(BITMAPFILEHEADER), fp) != sizeof(BITMAPFILEHEADER)) goto ERR;
		if(strncmp((char*)&bf.bfType, "BM", 2)) goto ERR;
		// BITMAPINFOHEADER
		if(fread(&bi, 1, sizeof(BITMAPINFOHEADER), fp) != sizeof(BITMAPINFOHEADER)) goto ERR;
		if(bi.biSize != sizeof(BITMAPINFOHEADER)) goto ERR;
		switch(bi.biBitCount) {
			case 8:
				line_bytes = bi.biWidth;
				if(!bi.biClrUsed)	bi.biClrUsed = 256;
				fread(rgb, 1, sizeof(RGBQUAD) * bi.biClrUsed, fp);	// �J���[�p���b�g��ǂݍ���
				break;
			case 24:
				line_bytes = bi.biWidth * 3;
				while(line_bytes % 4)	line_bytes++;
				break;
			case 32:
				line_bytes = bi.biWidth * 4;
				break;
			default:
				goto ERR;
		}
		// BITMAP�{�́i�{�g���A�b�v�œǂݍ��ށj
		if((bmp = (BYTE*)malloc(line_bytes * abs(bi.biHeight))) == NULL)	goto ERR;
		if(bi.biHeight < 0) {	// �g�b�v�_�E��
			bi.biHeight = -bi.biHeight;
			for(int y = bi.biHeight - 1; y >= 0; y--) {
				fread(bmp + line_bytes * y, 1, line_bytes, fp);
			}
		} else {				// �{�g���A�b�v
			fread(bmp, 1, line_bytes * bi.biHeight, fp);
		}
		fclose(fp);

		switch(bi.biBitCount) {	// 32bit�ɕϊ�
			case 8:
				if((bmp = bmp8to32(bmp, &bi, rgb)) == NULL)	goto ERR;
				break;
			case 24:
				if((bmp = bmp24to32(bmp, &bi)) == NULL)	goto ERR;
				break;
		}
		if (strstr(argv[i], "BUTTON") != NULL || \
			strstr(argv[i], "CHIP") != NULL || \
			strstr(argv[i], "THUM") != NULL || \
			strstr(argv[i], "TH2_") != NULL || \
			strstr(argv[i], "T.bmp") != NULL) {
			if((bmp = resize(bmp, &bi, 2)) == NULL)	goto ERR;
		} else {
			if((bmp = trim0(bmp, &bf, &bi)) == NULL)	goto ERR;
			if((bmp = trim1(bmp, &bi)) == NULL)	goto ERR;
			if((bmp = resize(bmp, &bi, 5)) == NULL)	goto ERR;
		}
		if((bmp = grayscale(bmp, &bi)) == NULL)	goto ERR;
		bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
		bf.bfSize    = bf.bfOffBits + bi.biWidth * bi.biHeight;
		
		if((fp = fopen(argv[i], "wb")) == NULL)	goto ERR;
		fwrite(&bf, 1, sizeof(BITMAPFILEHEADER), fp);
		fwrite(&bi, 1, sizeof(BITMAPINFOHEADER), fp);

		// �J���[�p���b�g�쐬
		for(int c = 0; c < 256; c++) {
			rgb[c].rgbReserved = 0;
			if(c < 16) {
				rgb[c].rgbRed = rgb[c].rgbGreen = rgb[c].rgbBlue = 255 - 17 * c;
			} else {	// �����F������
				rgb[c].rgbRed = rgb[c].rgbBlue = 0;
				rgb[c].rgbGreen = 255;
			}
		}
		fwrite(rgb, 1, sizeof(RGBQUAD) * 256, fp);
		fwrite(bmp, 1, bi.biWidth * bi.biHeight, fp);
		printf("%s - �������܂����B\n", argv[i]);
		goto FREE;
ERR:
		fprintf(stderr, "%s - ���s���܂����B\n", argv[i]);
FREE:
		if(fp  != NULL)	fclose(fp);
		if(bmp != NULL)	free(bmp);
	}

	return 0;
}

BYTE *bmp8to32(BYTE *bmp, BITMAPINFOHEADER *bi, RGBQUAD *rgb)
{
	BYTE *new_bmp;
	long w = bi->biWidth, h = bi->biHeight;
	if((new_bmp = (BYTE*)malloc(w * h * 4)) == NULL)	return NULL;

	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			new_bmp[(x + w * y) * 4 + 3] = rgb[bmp[x + w * y]].rgbReserved;
			new_bmp[(x + w * y) * 4 + 2] = rgb[bmp[x + w * y]].rgbRed;
			new_bmp[(x + w * y) * 4 + 1] = rgb[bmp[x + w * y]].rgbGreen;
			new_bmp[(x + w * y) * 4]     = rgb[bmp[x + w * y]].rgbBlue;
		}
	}

	free(bmp);
	bi->biBitCount = 32;
	return new_bmp;
}

BYTE *bmp24to32(BYTE *bmp, BITMAPINFOHEADER *bi)
{
	BYTE *new_bmp;
	long w = bi->biWidth, h = bi->biHeight, line_bytes = w * 3;
	while(line_bytes % 4)	line_bytes++;
	if((new_bmp = (BYTE*)malloc(w * h * 4)) == NULL)	return NULL;

	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			BYTE a = 255;
			BYTE r = bmp[(x * 3 + line_bytes * y) + 2];
			BYTE g = bmp[(x * 3 + line_bytes * y) + 1];
			BYTE b = bmp[(x * 3 + line_bytes * y)];
			if(r == 0 && g == 0 && b == 0)	a = 0;
			new_bmp[(x + w * y) * 4 + 3] = a;
			new_bmp[(x + w * y) * 4 + 2] = r;
			new_bmp[(x + w * y) * 4 + 1] = g;
			new_bmp[(x + w * y) * 4]     = b;
		}
	}

	free(bmp);
	bi->biBitCount = 32;
	return new_bmp;
}

// ���E�̓����������l�߂Ă���(8*5)pixel�A���C�������g
BYTE *trim1(BYTE *bmp, BITMAPINFOHEADER *bi)
{
	BYTE *new_bmp;
	long w = bi->biWidth, h = bi->biHeight;

	long x, x1 = w, x2 = 0;
	for (int y = 0; y < bi->biHeight; y++) {
		for (x = 0; !bmp[(x + w * y) * 4 + 3] && x < w; x++);
		if (x1 > x) { x1 = x; }
		for (x = w; !bmp[(x - 1 + w * y) * 4 + 3] && x > 0; x--);
		if (x2 < x) { x2 = x; }
	}
	if (x1 > w - x2) {
		x1 = w - x2;
	} else {
		x2 = w - x1;
	}

	while ((x2 - x1) % 40) {
		x1--;
		x2++;
	}

	if((new_bmp = (BYTE*)calloc(1, (x2 - x1) * h * 4)) == NULL)	return NULL;
	for(int y = 0; y < h; y++) {
		memcpy(new_bmp + (x2 - x1) * y * 4, bmp + (x1 + w * y) * 4, (x2 - x1) * 4);
	}
	free(bmp);
	bi->biWidth  = x2 - x1;
	bi->biHeight = h;
	return new_bmp;
}

// 640�~440��
BYTE *trim0(BYTE *bmp,  BITMAPFILEHEADER *bf, BITMAPINFOHEADER *bi)
{
	BYTE *new_bmp;
	long w = 640, h = 440, h2 = 448;

	if((new_bmp = (BYTE*)calloc(1, w * h * 4)) == NULL)	return NULL;

	for(int sy = 0, dy = h2 - (bf->bfReserved2 + bi->biHeight) - 4; sy < bi->biHeight; sy++, dy++) {
		if (dy >= 0 && dy < h) {
			memcpy(new_bmp + (bf->bfReserved1 + w * dy) * 4, bmp + bi->biWidth * sy * 4, bi->biWidth * 4);
		}
	}
	free(bmp);
	bi->biWidth  = w;
	bi->biHeight = h;
	return new_bmp;
}

BYTE *resize(BYTE *bmp, BITMAPINFOHEADER *bi, const int rate)
{
	BYTE *new_bmp;
	long w = bi->biWidth / rate, h = bi->biHeight / rate;
	if((new_bmp = (BYTE*)malloc(w * h * 4)) == NULL)	return NULL;
	
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			int a = 0, r = 0, g = 0, b = 0;
			for(int y1 = y * rate; y1 < y * rate + rate; y1++) {
				for(int x1 = x * rate; x1 < x * rate + rate; x1++) {
					a += bmp[(x1 + bi->biWidth * y1) * 4 + 3];
					r += bmp[(x1 + bi->biWidth * y1) * 4 + 2];
					g += bmp[(x1 + bi->biWidth * y1) * 4 + 1];
					b += bmp[(x1 + bi->biWidth * y1) * 4];
				}
			}
			a /= rate * rate;	r /= rate * rate;	g /= rate * rate;	b /= rate * rate;
			new_bmp[(x + w * y) * 4 + 3] = (BYTE)a;
			new_bmp[(x + w * y) * 4 + 2] = (BYTE)r;
			new_bmp[(x + w * y) * 4 + 1] = (BYTE)g;
			new_bmp[(x + w * y) * 4]     = (BYTE)b;
		}
	}
	free(bmp);
	bi->biWidth  = w;
	bi->biHeight = h;
	return new_bmp;
}

BYTE *grayscale(BYTE *bmp, BITMAPINFOHEADER *bi)
{
	BYTE *new_bmp;
	long w = bi->biWidth, h = bi->biHeight;
	if((new_bmp = (BYTE*)malloc(w * h)) == NULL)	return NULL;

	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			int a = bmp[(x + w * y) * 4 + 3];
			int r = bmp[(x + w * y) * 4 + 2];
			int g = bmp[(x + w * y) * 4 + 1];
			int b = bmp[(x + w * y) * 4];
			if(a & 0x80) {
				BYTE gray = (BYTE)((r * 3 + g * 5 + b * 2) / 10);
				new_bmp[x + w * y] = (gray >> 4) ^ 0x0F;	//�Ȃ�not�����肭�����񂩂����̂�
			} else {	// ���ߐF
				new_bmp[x + w * y] = 16;
			}
		}
	}
	free(bmp);
	bi->biBitCount = 8;
	bi->biClrUsed  = 0;
	bi->biCompression = BI_RGB;
	return new_bmp;
}
