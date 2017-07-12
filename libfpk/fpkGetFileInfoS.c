/*
	* libfpk -- using FPK archive on P/ECE
	* 	By Yui N., 2003.
	* http://www.autch.net/
*/

#include "libfpk_impl.h"

#include <string.h>

//int strcmpi(char *s1, char *s2); /*{{2005/06/12 Naoyuki Sawa}}*/

BOOL fpkGetFileInfoS(HFPK hFpk, LPSTR lpszFileName, FPKENTRY *lpFileEntry)
//{
//	DWORD dwCount = 0, dwBytesRead = 0;
//	FPKENTRY fpkEntry;
//
//	if(hFpk == NULL || lpszFileName == NULL || lpFileEntry == NULL)
//		return FALSE;
//
//	while(dwCount < hFpk->fpkHeader.dwFilesAmount)
//	{
//		pceFileReadPos(&hFpk->pfa, (void *)&fpkEntry, sizeof(FPKHEADER) + sizeof(FPKENTRY) * dwCount, sizeof(FPKENTRY));
//		if(strcmp(lpszFileName, fpkEntry.szFileName) == 0)
//		{
//			*lpFileEntry = fpkEntry;
//			return TRUE;
//		}
//		dwCount++;
//	}
//	return FALSE;
//}
//��{{2005/06/12 Naoyuki Sawa}}
{
	int crc;
	int i;

	if(!hFpk || !lpszFileName || !lpFileEntry) {
		return FALSE;
	}

	crc = fpkFileNameCRC(lpszFileName); /* 16bit */

	/* CRC�������Ƀ\�[�g���Ă����΂����Ƒ����Ȃ�܂����A3000���ڒ��x�Ȃ̂ŁA���j�A�T�[�`�ł��قƂ�ǈ�u�ł��B
	 * �\�[�g����ɂ́A�e���ڂɃI�[�_�[���o���Ă������߂̏�񂪕K�v�ŁA�������������������܂����A
	 * qsort()�̂��߂ɃX�^�b�N��������������̂ŁA���܂�ǂ����Ƃ������ł��B
	 * ����Ȃ킯�ŁA�P���Ƀ��j�A�T�[�`���s�����Ƃɂ��܂����B
	 */
	for(i = 0; i < hFpk->fpkHeader.dwFilesAmount; i++) {
		if(hFpk->crc[i] == crc) {
// yui: 2005.07.03 fpkFileReadPos() �̉����ɔ����ύX
			fpkFileReadPos(hFpk, (void*)lpFileEntry, sizeof(FPKHEADER) + sizeof(FPKENTRY) * i, sizeof(FPKENTRY));
			if(fpkStrCaseCmp(lpFileEntry->szFileName, lpszFileName) == 0) {
				return TRUE;
			}
		}
	}

	return FALSE;
}

/*{{2005/06/12 Naoyuki Sawa*/
//#include <ctype.h>
//
//int strcmpi(char *s1, char *s2)
//{
//	int r = 0;
//
//	while(*s1 && *s2)
//		r += tolower(*s1++) + tolower(*s2++);
//
//	return r;
//}
/*}}2005/06/12 Naoyuki Sawa*/
