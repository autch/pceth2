/*
  * libfpk -- using FPK archive on P/ECE
  *   By Yui N., 2003 - 2005.
  * http://www.autch.net/
*/

/*
 * Merged by Yui N. 2005.03.21
 * pva �W�J�R�[�h��g�ݍ��݂܂����D
 > * Modified by �d��Chu at 2004/11/08
 > * pva�`���W�J�ɑΉ����܂����B
 > * �W�J�o�b�t�@�̖�����'\0'��ǉ����܂����iP/VNScripter�d�l�j�B
 * �f�[�^�̉t�����m�ۂ��邽�߁Cpva �`���ȊO�Ńo�b�t�@������ \0 ��t������̂�
 * �����ł���Ă��������i��
 */

#include "libfpk_impl.h"

//BYTE *fpkExtractToBuffer(HFPK hFpk, FPKENTRY *fpkEntry)
//{
//  if(hFpk == NULL || fpkEntry == NULL)
//    return NULL;
//
//  switch((BYTE)fpkEntry->szFileName[15])
//  {
//    case FPK_LZSS_COMPRESSION:
//    //case FPK_ZLIB_COMPRESSION: /*{{2005/06/23 Naoyuki Sawa}}*/
//    {
//      BYTE *pBuffer = pceHeapAlloc(fpkEntry->dwSize);
//      if(pBuffer == NULL)
//        return NULL;
//
//      //pceFileReadPos(&hFpk->pfa, pBuffer, fpkEntry->dwOffset, fpkEntry->dwSize);
//      //��{{2005/06/12 Naoyuki Sawa}}
//      pceFileReadPos(hFpk, pBuffer, fpkEntry->dwOffset, fpkEntry->dwSize);
//
//      {
//        // fpk ���A���Z�N�^��ɗL��Ƃ͌���Ȃ��̂ŁC�������񒆊ԃo�b�t�@�ɃR�s�[
//        DWORD dwOriginalSize = *((DWORD *)pBuffer);
//        BYTE *pOutput = pceHeapAlloc(dwOriginalSize);
//        if(pOutput == NULL)
//        {
//          pceHeapFree(pBuffer);
//          return NULL;
//        }
//        //if((BYTE)fpkEntry->szFileName[15] == FPK_LZSS_COMPRESSION)  /*{{2005/06/23 Naoyuki Sawa}}*/
//          hitodeLZSSDecode(pOutput, pBuffer + 4, dwOriginalSize - 4);
//        //else                                                        /*{{2005/06/23 Naoyuki Sawa}}*/
//        //  zlibDecode(pBuffer + 4, pOutput);                         /*{{2005/06/23 Naoyuki Sawa}}*/
//        pceHeapFree(pBuffer);
//        return pOutput;
//      }
//    }
//    case FPK_NO_COMPRESSION:
//    default:
//    {
//      BYTE *pBuffer = pceHeapAlloc(fpkEntry->dwSize);
//      if(pBuffer == NULL)
//        return NULL;
//
//      //pceFileReadPos(&hFpk->pfa, pBuffer, fpkEntry->dwOffset, fpkEntry->dwSize);
//      //��{{2005/06/12 Naoyuki Sawa}}
//      pceFileReadPos(hFpk, pBuffer, fpkEntry->dwOffset, fpkEntry->dwSize);
//
//      //{{2005/06/23 Naoyuki Sawa
//      //if(*((DWORD *)pBuffer) == PVNSPACK_HEADER)  // pvnspack format
//      //{
//      //  DWORD dwOriginalSize = *((DWORD *)(pBuffer + 28));
//      //  BYTE *pOutput = pceHeapAlloc(dwOriginalSize + 1);
//      //  if(pOutput == NULL)
//      //  {
//      //    pceHeapFree(pBuffer);
//      //    return NULL;
//      //  }
//      //  zlibDecode(pBuffer + 36, pOutput);
//      //  pceHeapFree(pBuffer);
//      //  *(pOutput + dwOriginalSize) = '\0';
//      //  return pOutput;
//      //}
//      //else
//      //}}2005/06/23 Naoyuki Sawa
//        return pBuffer;
//    }
//  }
//}
//��{{2005/06/24 Naoyuki Sawa}}
// yui: 2005.07.03 fpkFileReadPos() �̉����ɔ����ύX
// 2005/08/20 �d��Chu �m�ۍς݃o�b�t�@�ɓW�J�ł���悤�ύX
BYTE* fpkExtractToBuffer(HFPK hFpk, FPKENTRY* fpkEntry, BYTE* pDst)
{
	BYTE* pBuffer = NULL;
	//
	DWORD dwOriginalSize;

	if(hFpk && fpkEntry)
	{
		switch((BYTE)fpkEntry->szFileName[15])
		{
			case FPK_NO_COMPRESSION:
				pBuffer = pceHeapAlloc(fpkEntry->dwSize);
				if(!pBuffer) return NULL;

				fpkFileReadPos(hFpk, pBuffer, fpkEntry->dwOffset, fpkEntry->dwSize);
#ifndef LIBFPK_NO_ZLIB
				if(*((DWORD *)pBuffer) == PVNSPACK_HEADER)	// pvnspack format
				{
					DWORD dwOriginalSize = *((DWORD *)(pBuffer + 28));
					BYTE *pOutput = pDst;
					if (!pOutput)
						pceHeapAlloc(dwOriginalSize + 1);
					pceHeapFree(pBuffer);
					if(!pOutput)
						return NULL;
					zlibDecode(hFpk, 36, fpkEntry->dwSize - 36, pOutput);
					*(pOutput + dwOriginalSize) = '\0';
					return pOutput;
				}
#endif // !LIBFPK_NO_ZLIB
				if (pDst)
				{
					memcpy(pDst, pBuffer, fpkEntry->dwSize);
					pceHeapFree(pBuffer);
					pBuffer = pDst;
				}
				break;
#ifndef LIBFPK_NO_LZSS
			case FPK_LZSS_COMPRESSION:
#endif
#ifndef LIBFPK_NO_LZF
			case FPK_LZF_COMPRESSION:
#endif
#ifndef LIBFPK_NO_ZLIB
			case FPK_ZLIB_COMPRESSION:
#endif
				fpkFileReadPos(hFpk, (unsigned char*)&dwOriginalSize, fpkEntry->dwOffset, 4);
				if (pDst)
					pBuffer = pDst;
				else
					pBuffer = pceHeapAlloc(dwOriginalSize);
				if(pBuffer)
				{
// yui: 2005.07.05
#ifndef LIBFPK_NO_LZSS
					if((BYTE)fpkEntry->szFileName[15] == FPK_LZSS_COMPRESSION)
						hitodeLZSSDecode(hFpk, fpkEntry->dwOffset + 4, fpkEntry->dwSize - 4, pBuffer);
#endif
#ifndef LIBFPK_NO_LZF
					if((BYTE)fpkEntry->szFileName[15] == FPK_LZF_COMPRESSION)
						lzfDecompress(hFpk, fpkEntry->dwOffset + 4, fpkEntry->dwSize - 4, pBuffer, dwOriginalSize);
#endif
#ifndef LIBFPK_NO_ZLIB
					if((BYTE)fpkEntry->szFileName[15] == FPK_ZLIB_COMPRESSION)
						zlibDecode(hFpk, fpkEntry->dwOffset + 4, fpkEntry->dwSize - 4, pBuffer);
#endif
				}
				break;
		}
	}
	return pBuffer;
}
