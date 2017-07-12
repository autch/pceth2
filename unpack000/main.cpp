/*
 *	unpack000
 *	ToHeart2��TH2DATA.000�^TH2DATA.001����f�[�^�؂�o��
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/02/20	����
 *	2005/02/26	�R�}���h���C���I�v�V������-bm2,-bin��ǉ�
 *	2005/04/17	unpack000.ini��ToHeart2�̃t�H���_��ۑ�
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tagPACK000 {
	DWORD	header;		// "PACK"
	WORD	famount;
	WORD	reserved;	// 01 00 ���k�t���O���H
} PACK000;

typedef struct tagPACK000_FILEINFO {
	char	fname[24];
	DWORD	offset;
	DWORD	size;
} PACK000_FILEINFO;

void unpack000(const char *fName, FILE *fp000, FILE *fp001, int famount);
int unpack001(PACK000_FILEINFO *finfo, FILE *fp001);

int main(int argc, char *argv[])
{
	FILE	*fp000 = NULL, *fp001 = NULL, *fpout = NULL, *fpini = NULL, *fplst = NULL;
	char	th2dir[_MAX_PATH];
	char	path[_MAX_PATH];
	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	PACK000				pinfo;
	PACK000_FILEINFO	finfo;

	if(argc == 1) {	// usage
		fprintf(stderr, "ToHeart2 TH2DATA.000�^TH2DATA.001 �W�J\n");
		fprintf(stderr, "unpack000 { option | filename .. }\n");
		fprintf(stderr, "-all �S�t�@�C���o��\n");
		fprintf(stderr, "-bm2 �SBM2�t�@�C���o��\n");
		fprintf(stderr, "-bin �SBIN�t�@�C���o��\n");
		return 1;
	}

	if((fpini = fopen("unpack000.ini", "r")) != NULL) {
		fscanf(fpini, "%s", th2dir);
		fclose(fpini);
	} else {
		printf("ToHeart2�̃f�[�^������t�H���_���w�肵�ĉ������B�y��zD:\\�^C:\\ToHeart2\n");
		scanf("%s", th2dir);
	}

	if(*(th2dir + strlen(th2dir) - 1) != '\\')	strcat(th2dir, "\\");
	_splitpath(th2dir, drive, dir, NULL, NULL);
	_makepath(path, drive, dir, "TH2DATA", ".000");
	if((fp000 = fopen(path, "rb")) == NULL) goto ERR;
	fread(&pinfo, 1, sizeof(PACK000), fp000);
	if(strncmp((char*)&pinfo, "PACK", 4))	goto ERR;
	_makepath(path, drive, dir, "TH2DATA", ".001");
	if((fp001 = fopen(path, "rb")) == NULL) goto ERR;

	if((fpini = fopen("unpack000.ini", "w")) != NULL) {
		fprintf(fpini, "%s", th2dir);
		fclose(fpini);
	}

	if(!strcmp(argv[1], "-all")) {			// �S�t�@�C��
		for(int j = 0; j < pinfo.famount; j++) {
			fread(&finfo, 1, sizeof(PACK000_FILEINFO), fp000);
			if(unpack001(&finfo, fp001)) {
				printf("%s - �������܂����B\n", finfo.fname);
			} else {
				fprintf(stderr, "%s - ���s���܂����B\n", finfo.fname);
			}
		}
	} else if(!strcmp(argv[1], "-bm2")) {	// BM2�t�@�C��
		for(int j = 0; j < pinfo.famount; j++) {
			fread(&finfo, 1, sizeof(PACK000_FILEINFO), fp000);
			if(strstr(finfo.fname, ".BM2")) {
				if(unpack001(&finfo, fp001)) {
					printf("%s - �������܂����B\n", finfo.fname);
				} else {
					fprintf(stderr, "%s - ���s���܂����B\n", finfo.fname);
				}
			}
		}
	} else if(!strcmp(argv[1], "-bin")) {	// BIN�t�@�C��
		for(int j = 0; j < pinfo.famount; j++) {
			fread(&finfo, 1, sizeof(PACK000_FILEINFO), fp000);
			if(strstr(finfo.fname, ".BIN")) {
				if(unpack001(&finfo, fp001)) {
					printf("%s - �������܂����B\n", finfo.fname);
				} else {
					fprintf(stderr, "%s - ���s���܂����B\n", finfo.fname);
				}
			}
		}
	} else if(!strcmp(argv[1], "-list")) {	// ���X�g����
		char fName[32];
		for(int i = 2; i < argc; i++) {
			if((fplst = fopen(argv[i], "rt")) == NULL)	goto ERR_LST;
			while(1) {
				fscanf(fplst, "%s", fName);
				if(!strcmp(fName, "0"))	break;
				unpack000(fName, fp000, fp001, pinfo.famount);
			}
			fclose(fplst);
			continue;
ERR_LST:
			fprintf(stderr, "%s - ���s���܂����B\n", argv[i]);
		}
	} else {								// �ʃt�@�C��
		for(int i = 1; i < argc; i++) {
			unpack000(argv[i], fp000, fp001, pinfo.famount);
		}
	}

	fclose(fp001);
	fclose(fp000);
	return 0;

ERR:
	fprintf(stderr, "%s - ���s���܂����B\n", path);
	return 1;
}

void unpack000(const char *fName, FILE *fp000, FILE *fp001, int famount)
{
	PACK000_FILEINFO finfo;

	fseek(fp000, sizeof(PACK000), SEEK_SET);
	int i = 0;
	for(; i < famount; i++) {
		fread(&finfo, 1, sizeof(PACK000_FILEINFO), fp000);
		if(!strcmp(fName, finfo.fname))	break;
	}
	if(i >= famount)	goto ERR;
	if(unpack001(&finfo, fp001)) {
		printf("%s - �������܂����B\n", fName);
		return;
	}
ERR:
	fprintf(stderr, "%s - ���s���܂����B\n", fName);
}

int unpack001(PACK000_FILEINFO *finfo, FILE *fp001)
{
	BYTE buf;

	FILE *fpout = NULL;
	if((fpout = fopen(finfo->fname, "wb")) == NULL)	return 0;

	fseek(fp001, finfo->offset, SEEK_SET);
	for(int i = 0; i < (long)finfo->size; i++) {
		buf = fgetc(fp001);
		fputc(buf, fpout);
	}
	fclose(fpout);
	return 1;
}
