
#include "libfpk_impl.h"
#include "piecezl.h"

/* zlib �W�J���[�`���T���v���ifrom P/ECE HAND BOOK Vol.2 ��F�̖��j*/
/* ���̃\�[�X�̌��́C�܂��ׂЂ낵 ����̏�L�R�[�h */
/* ���̃\�[�X���̂́C�d��Chu���ĂƂ灚�ۂ��� ����� myunpack �̃\�[�X����O�����p�i�� */
/* ������ P/ECE HAND BOOK �����ĂȂ����i�� */

// 0x13e000�`0x13ffff �܂ŁA�W�J�p�o�b�t�@
#define	EXTSIZE		0x0400
#define	WORKSADR	0x13d400
#define	WORKS 		((void *)WORKSADR)
#define	EXTBUFF		((BYTE *)(WORKSADR - EXTSIZE))
static BYTE *s_pZlibDstBuffer;
static DWORD s_dwDecodeLength = 0;

// BEGIN: yui: 2005.07.03
#define FETCHBUF_SIZE 0x400         // ���Ȃ�����H
static DWORD s_dwSourcePos = 0, s_dwSourceSize = 0, s_dwOffset = 0;
static BYTE s_byFetchBuffer[FETCHBUF_SIZE];
static HFPK s_hFpk;
// END: yui: 2005.07.03

static int dummy(zlibIO *zi)
{
	return 0xff;
}

static void DecodeProc1(zlibIO *zi)
{
	int n = zi->ptr - zi->ptr0;

	zi->ptr = zi->ptr0;
	memcpy(&s_pZlibDstBuffer[s_dwDecodeLength], zi->ptr0, n);
	s_dwDecodeLength += n;
}

// yui: 2005.07.03: �X�g���[�������ǂ݂��œǂ�
static int FetchBufProc(zlibIO* zi)
{
	int size = FETCHBUF_SIZE;
	if(size > s_dwSourceSize)
		size = s_dwSourceSize;
	fpkFileReadPos(s_hFpk, zi->ptr0, s_dwOffset + s_dwSourcePos, size);
	s_dwSourcePos += size;
	s_dwSourceSize -= size;
	zi->ptr = zi->ptr0;
	return *(zi->ptr++);
}


// pOutBuff: �W�J�����f�[�^������o�b�t�@
DWORD zlibDecode(HFPK hFpk, DWORD dwOffset, DWORD dwSourceSize, BYTE *pOutBuff)
{
	zlibIO Zin, Zout;

	s_dwDecodeLength = 0;
	s_pZlibDstBuffer = pOutBuff;
	s_dwSourceSize = dwSourceSize - FETCHBUF_SIZE;
	s_dwSourcePos = FETCHBUF_SIZE;
	s_dwOffset = dwOffset;
	s_hFpk = hFpk;

// BEGIN: yui: 2005.07.03
//	Zin.ptr = (BYTE *)pArcData;
//	Zin.ptre = (BYTE *)-1;
//	Zin.fn.fil = dummy;
// END: yui: 2005.07.03

	Zin.ptr = Zin.ptr0 = s_byFetchBuffer;
	Zin.ptre = s_byFetchBuffer + FETCHBUF_SIZE;
	Zin.fn.fil = FetchBufProc;
	fpkFileReadPos(s_hFpk, Zin.ptr0, s_dwOffset, FETCHBUF_SIZE);

	Zout.ptr0 = EXTBUFF;
	Zout.ptr = EXTBUFF;
	Zout.ptre = EXTBUFF + EXTSIZE;
	Zout.fn.fls = DecodeProc1;

	pceZlibExpand(&Zin, &Zout, WORKS);

	return s_dwDecodeLength;
}
