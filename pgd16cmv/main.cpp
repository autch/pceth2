/*
 *	pgd16cmv
 *	4�^8bit�����kBMP��16�K���g��PIECE_BMP�R���o�[�^
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/02/16	����
 *	2005/02/20	�����̃��C���h�J�[�h�Ή��isetargv.obj�������N�j
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct tagPBMP_FILEHEADER{
	DWORD	head;		//	HEADER   'PBEX'
	DWORD	fsize;		//	�t�@�C���� �iBYTE�P�ʁj
	BYTE	bpp;		//	bit�[�x  �i4�j
	BYTE	mask;		//	�}�X�N��bit�[�x  �i0/1�j
	short	w;			//	X��		8�s�N�Z���P�ʌ���
	short	h;			//	Y����		
	DWORD	buf_size;	//	BMP�T�C�Y	�iBYTE�P�ʁj
}PBMP_FILEHEADER;

int main(int argc, char *argv[])
{
	FILE	*fp = NULL;
	BITMAPFILEHEADER	bf;
	BITMAPINFOHEADER	bi;
	PBMP_FILEHEADER		pb;
	BYTE	*bmp = NULL, *buf = NULL, *mask = NULL;
	char	output_flag = 't';
	char	path[_MAX_PATH];
	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];

	if(argc == 1) {	// usage
		fprintf(stderr, "4�^8bit�����kBMP��16�K���g��PIECE_BMP�R���o�[�^\n");
		fprintf(stderr, "pgd16cmv [option] filename ..\n");
		fprintf(stderr, "-b �o�C�i���o��(.pgx)\n");
		fprintf(stderr, "-t �e�L�X�g�o��(.c)�f�t�H���g\n");
		return 1;
	}

	for(int i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "-t")) {	// �Ȍ�̃t�@�C���̓e�L�X�g�o��
			output_flag = 't';
			continue;
		}
		if(!strcmp(argv[i], "-b")) {	// �Ȍ�̃t�@�C���̓o�C�i���o��
			output_flag = 'b';
			continue;
		}

		_splitpath(argv[i], drive, dir, fname, ext);
		if((fp = fopen(argv[i], "rb")) == NULL)	goto ERR;
		// BITMAPFILEHEADER
		if(fread(&bf, 1, sizeof(BITMAPFILEHEADER), fp) != sizeof(BITMAPFILEHEADER)) goto ERR;
		if(strncmp((char*)&bf.bfType, "BM", 2)) goto ERR;
		// BITMAPINFOHEADER
		if(fread(&bi, 1, sizeof(BITMAPINFOHEADER), fp) != sizeof(BITMAPINFOHEADER)) goto ERR;
		if(bi.biSize != sizeof(BITMAPINFOHEADER)) goto ERR;
		if(bi.biWidth % 8)	goto ERR;	// ����8�s�N�Z���P��
		if(bi.biBitCount != 8 && bi.biBitCount != 4)	goto ERR; // 4/8bit
		if(bi.biCompression != BI_RGB)	goto ERR;	// �����k
		if(bi.biClrUsed == 0)	bi.biClrUsed = 1 << bi.biBitCount;
		// RGBQUAD[bi.biClrUsed]�i�ǂݔ�΂��j
		if(fseek(fp, sizeof(RGBQUAD) * bi.biClrUsed, SEEK_CUR))	goto ERR;
		// BITMAP�{��
		if((bmp = (BYTE*)malloc(bi.biWidth * bi.biHeight * bi.biBitCount / 8)) == NULL)	goto ERR;
		if(bi.biHeight < 0) {	// �g�b�v�_�E��
			fread(bmp, 1, bi.biWidth * bi.biHeight * bi.biBitCount / 8, fp);
			bi.biHeight = -bi.biHeight;
		} else {				// �{�g���A�b�v
			for(int y = bi.biHeight - 1; y >= 0; y--) {
				fread(bmp + bi.biWidth * y * bi.biBitCount / 8, 1, bi.biWidth * bi.biBitCount / 8, fp);
			}
		}
		fclose(fp);

		// PBMP_FILEHEADER
		strncpy((char*)&pb.head, "XEBP", sizeof(DWORD));
		pb.bpp	= 4;
		pb.mask	= 0;
		pb.w	= (short)bi.biWidth;
		pb.h	= (short)bi.biHeight;
		// PIECE_BMP�{��
		if((buf  = (BYTE*)malloc(pb.w * pb.h / 2)) == NULL)	goto ERR;
		if((mask = (BYTE*)malloc(pb.w * pb.h / 8)) == NULL)	goto ERR;
		if(bi.biBitCount == 8) {	// 8bit���}�X�N�L/��16�K��
			BYTE *b = buf, *m = mask;
			for(int j = 0; j < pb.w * pb.h; j += 8) {	// �}�X�N
				for(int k = 0; k < 8; k++) {
					*m <<= 1;
					if(bmp[j+k] & 0xF0) {
						bmp[j+k] = 0;
						*m |= 1;
						pb.mask = 1;
					}
				}
				m++;
			}
			for(int j = 0; j < pb.w * pb.h; j += 2) {	// ��f
				*b++ = bmp[j] << 4 | bmp[j+1];
			}
		} else {					// 4bit���}�X�N��16�K��
			memcpy(buf, bmp, pb.w * pb.h / 2);
		}
		// PBMP_FILEHEADER
		pb.buf_size = (pb.bpp + pb.mask) * pb.w * pb.h / 8;
		pb.fsize = sizeof(PBMP_FILEHEADER) + pb.buf_size;
		free(bmp);
		bmp = NULL;

		if(output_flag == 't') {	// �e�L�X�g�o��
			_makepath(path, drive, dir, fname, ".c");
			if((fp = fopen(path, "wt")) == NULL)	goto ERR;
			fprintf(fp, "unsigned char %s[] = {\n", fname);
			BYTE *p = (BYTE*)&pb;
			for(int j = 0; j < sizeof(PBMP_FILEHEADER); j++)	fprintf(fp, "0x%02x, ",*p++);
			fprintf(fp, "\n");
			p = buf;
			for(int y = 0; y < pb.h; y++) {
				for(int x = 0; x < pb.w; x += 2) {
					fprintf(fp, "0x%02x, ", *p++);
				}
				fprintf(fp, "\n");
			}
			if(pb.mask) {
				p = mask;
				for(int y = 0; y < pb.h; y++) {
					for(int x = 0; x < pb.w; x += 8) {
						fprintf(fp, "0x%02x, ", *p++);
					}
					fprintf(fp, "\n");
				}
			}
			fprintf(fp, "\n};\n");
		} else {					// �o�C�i���o��
			_makepath(path, drive, dir, fname, ".pgx");
			if((fp = fopen(path, "wb")) == NULL)	goto ERR;
			fwrite(&pb, 1, sizeof(PBMP_FILEHEADER), fp);
			fwrite(buf, 1, pb.w * pb.h / 2, fp);
			if(pb.mask)	fwrite(mask, 1, pb.w * pb.h / 8, fp);
		}
		printf("%s - �������܂����B\n", path);
		goto FREE;

ERR:
		fprintf(stderr, "%s - ���s���܂����B\n", argv[i]);
FREE:
		if(fp	!= NULL)	fclose(fp);
		if(bmp	!= NULL)	free(bmp);
		if(buf	!= NULL)	free(buf);
		if(mask	!= NULL)	free(mask);
	}

	return 0;
}