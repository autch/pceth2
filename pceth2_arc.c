/*
 *	pceth2 - �A�[�J�C�u�֘A
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/07/23	main.c���番��
 *	2005/08/20	���炩���ߊm�ۂ��Ă������o�b�t�@�ɓW�J�ł���悤�ύX
 */

//	2005/07/23
//	�A�[�J�C�u�Ƃ̐܏Ղ͑S�Ă��̃\�[�X�ɔC���܂��B

#include "libfpk/libfpk.h"

HFPK	hFpk;	// �n���h��

/*
 *	�n���h���̏�����
 *
 *	*fName	�A�[�J�C�u�t�@�C����
 *
 *	return	TRUE / FALSE
 */
BOOL fpk_InitHandle(char *fName)
{
	if ((hFpk = fpkOpenArchive(fName)) == NULL) {
		return FALSE;	// ���s
	}

	return TRUE;	// ����
}

/*
 *	par�`���̃n���h������G���g�����J��
 *
 *	*fName	�G���g���̃t�@�C����
 *	*len	�G���g���̃T�C�Y��Ԃ��|�C���^�iNULL�Ȃ�Ԃ��Ȃ��j
 *	*pDst	�o�͐�iNULL�Ȃ�V�����q�[�v���m�ۂ��ĕԂ��j
 *
 *	return	�G���g���̃f�[�^�ipceHeapAlloc()�Ŋm�ہj
 */
BYTE *fpk_getEntryData(char *fName, DWORD *len, BYTE *pDst)
{
	BYTE		*ret = NULL;
	FPKENTRY	fpkEntry;

	if (fpkGetFileInfoS(hFpk, fName, &fpkEntry)) {
		if (len != NULL) {
			if (fpkEntry.szFileName[15] & 0x80) {	// ���k�G���g�����H
				//pceFileReadPos(&hFpk->pfa, (BYTE *)len, fpkEntry.dwOffset, 4);
				//��{{2005/06/12 Naoyuki Sawa}}
				// yui: 2005.07.03 pceFileReadPos() �������
				fpkFileReadPos(hFpk, (BYTE *)len, fpkEntry.dwOffset, 4);
			} else {
				*len = fpkEntry.dwSize;
			}
		}
		ret = fpkExtractToBuffer(hFpk, &fpkEntry, pDst);
	}

	return ret;
}

BOOL fpk_getFileInfoS(char *fName, FPKENTRY *fpkEntry)
{
	return fpkGetFileInfoS(hFpk, fName, fpkEntry);
}

DWORD fpk_getEntryDataPos(const FPKENTRY *fpkEntry, BYTE *pDst, DWORD offset, DWORD size)
{
	DWORD	ret = 0;

	if (offset < fpkEntry->dwSize) {
		ret = (offset + size <= fpkEntry->dwSize)? size : (fpkEntry->dwSize - offset);
		fpkFileReadPos(hFpk, pDst, fpkEntry->dwOffset + offset, ret);
	}
	return ret;
}

/*
 *	par�`���̃n���h������G���g���̈ꕔ�����J��
 *
 *	*fName	�G���g���̃t�@�C����
 *	*pDst	�o�͐�
 *	offset
 *	size
 *
 *	return �ǂݍ��񂾒���
 */
DWORD fpk_getEntryDataPosEx(char *fName, BYTE *pDst, DWORD offset, DWORD size)
{
	FPKENTRY	fpkEntry;

	if (fpkGetFileInfoS(hFpk, fName, &fpkEntry)) {
		return fpk_getEntryDataPos(&fpkEntry, pDst, offset, size);
	}
	return 0;
}

/*
 *	par�`���̃n���h�����������
 */
void fpk_ReleaseHandle(void)
{
	fpkCloseArchive(hFpk);
}
