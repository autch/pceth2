#include <s1c33cpu.h>
#include <string.h>
#include <stdio.h>
#include <piece.h>
#include "mmc_api.h"

#define PDWAIT	(*(volatile unsigned char*)0x1000000)	/* �p���[�_�E���̂��߂̃E�F�C�g�̈� */
#define P0D	(*(volatile unsigned char*)0x00402D1)	/* P0���o�͌��p�|�[�g�f�[�^���W�X�^ */

/* �N���b�NLo���M��̃E�F�C�g(1��K�v�A0��ł̓G���[) */
#define WAIT1 do {	\
	*pdwait = 0;	\
} while(0)

/* �N���b�NHi���M��̃E�F�C�g(5��K�v�A4��ł̓G���[) */
#define WAIT5 do {	\
	*pdwait = 0;	\
	*pdwait = 0;	\
	*pdwait = 0;	\
	*pdwait = 0;	\
	*pdwait = 0;	\
} while(0)


//---------------------------------------------------------------------------

int
mmcWaitStartBit(void)
{
	volatile unsigned char* const pdwait = &PDWAIT;
	volatile unsigned char* const p0d = &P0D;
	//
	const int lo0 = *p0d & ~0x60;
	const int lo1 = lo0 | 0x20;
	//const int hi0 = lo0 | 0x40;
	const int hi1 = lo0 | 0x60;
	//
	int i;

	/* �X�^�[�g�r�b�g(0)������܂ő҂B */
	i = MMC_RECV_TIMEOUT;
	for(;;) {

		/* �N���b�N��Low�ɁB(P06��Low��) */
		*p0d = lo1;
		WAIT1; /* �E�F�C�g1��ȏ�K�v */

		/* �N���b�N��High�ɁB(P06��High��) */
		*p0d = hi1;
		WAIT5; /* �E�F�C�g5��ȏ�K�v */

		/* �f�[�^���擾(P04����f�[�^���擾)�B */
		if(!(*p0d & 0x10)) {
			break; /* �X�^�[�g�r�b�g(0)�����o���� */
		}

		/* �^�C���A�E�g��0��Ԃ��B */
		if(!--i) {
			return 0;
		}
	}

	return 1;
}

//---------------------------------------------------------------------------

int
mmcWaitBusy(void)
{
	volatile unsigned char* const pdwait = &PDWAIT;
	volatile unsigned char* const p0d = &P0D;
	//
	const int lo0 = *p0d & ~0x60;
	const int lo1 = lo0 | 0x20;
	//const int hi0 = lo0 | 0x40;
	const int hi1 = lo0 | 0x60;
	//
	int i;

	/* �X�^�[�g�r�b�g(0)������܂ő҂B */
	i = MMC_RECV_TIMEOUT;
	for(;;) {

		/* �N���b�N��Low�ɁB(P06��Low��) */
		*p0d = lo1;
		WAIT1; /* �E�F�C�g1��ȏ�K�v */

		/* �N���b�N��High�ɁB(P06��High��) */
		*p0d = hi1;
		WAIT5; /* �E�F�C�g5��ȏ�K�v */

		/* �f�[�^���擾(P04����f�[�^���擾)�B */
		if(*p0d & 0x10) {
			break; /* �r�W�[����(1)�����o���� */

		}

		/* �^�C���A�E�g��0��Ԃ��B */
		if(!--i) {
			return 0;
		}
	}

	return 1;
}

//---------------------------------------------------------------------------

unsigned char
mmcByteRecv(void)
{
	volatile unsigned char* const pdwait = &PDWAIT;
	volatile unsigned char* const p0d = &P0D;
	//
	const int lo0 = *p0d & ~0x60;
	const int lo1 = lo0 | 0x20;
	//const int hi0 = lo0 | 0x40;
	const int hi1 = lo0 | 0x60;
	//
	int v = 0;

	*p0d = lo1; WAIT1; *p0d = hi1; WAIT5; if(*p0d & 0x10) v |= 0x80;
	*p0d = lo1; WAIT1; *p0d = hi1; WAIT5; if(*p0d & 0x10) v |= 0x40;
	*p0d = lo1; WAIT1; *p0d = hi1; WAIT5; if(*p0d & 0x10) v |= 0x20;
	*p0d = lo1; WAIT1; *p0d = hi1; WAIT5; if(*p0d & 0x10) v |= 0x10;
	*p0d = lo1; WAIT1; *p0d = hi1; WAIT5; if(*p0d & 0x10) v |= 0x08;
	*p0d = lo1; WAIT1; *p0d = hi1; WAIT5; if(*p0d & 0x10) v |= 0x04;
	*p0d = lo1; WAIT1; *p0d = hi1; WAIT5; if(*p0d & 0x10) v |= 0x02;
	*p0d = lo1; WAIT1; *p0d = hi1; WAIT5; if(*p0d & 0x10) v |= 0x01;

	return v;
}

//---------------------------------------------------------------------------

void
mmcByteSend(int data)
{
	volatile unsigned char* const pdwait = &PDWAIT;
	volatile unsigned char* const p0d = &P0D;
	//
	const int lo0 = *p0d & ~0x60;
	const int lo1 = lo0 | 0x20;
	const int hi0 = lo0 | 0x40;
	const int hi1 = lo0 | 0x60;

	/* �f�[�^�A���]���ȊO�ł̃o�C�g���M����_�~�[�N���b�N���M���́A�E�F�C�g���K�v�݂����ł��B */
	if(data & 0x80) { *p0d = lo1; WAIT1; *p0d = hi1; WAIT5; } else { *p0d = lo0; WAIT1; *p0d = hi0; WAIT5; }
	if(data & 0x40) { *p0d = lo1; WAIT1; *p0d = hi1; WAIT5; } else { *p0d = lo0; WAIT1; *p0d = hi0; WAIT5; }
	if(data & 0x20) { *p0d = lo1; WAIT1; *p0d = hi1; WAIT5; } else { *p0d = lo0; WAIT1; *p0d = hi0; WAIT5; }
	if(data & 0x10) { *p0d = lo1; WAIT1; *p0d = hi1; WAIT5; } else { *p0d = lo0; WAIT1; *p0d = hi0; WAIT5; }
	if(data & 0x08) { *p0d = lo1; WAIT1; *p0d = hi1; WAIT5; } else { *p0d = lo0; WAIT1; *p0d = hi0; WAIT5; }
	if(data & 0x04) { *p0d = lo1; WAIT1; *p0d = hi1; WAIT5; } else { *p0d = lo0; WAIT1; *p0d = hi0; WAIT5; }
	if(data & 0x02) { *p0d = lo1; WAIT1; *p0d = hi1; WAIT5; } else { *p0d = lo0; WAIT1; *p0d = hi0; WAIT5; }
	if(data & 0x01) { *p0d = lo1; WAIT1; *p0d = hi1; WAIT5; } else { *p0d = lo0; WAIT1; *p0d = hi0; WAIT5; }
}

//---------------------------------------------------------------------------

int
mmcRecvData(unsigned char* buf, int len)
{
	volatile unsigned char* const pdwait = &PDWAIT;
	volatile unsigned char* const p0d = &P0D;
	//
	const int lo0 = *p0d & ~0x60;
	const int lo1 = lo0 | 0x20;
	//const int hi0 = lo0 | 0x40;
	const int hi1 = lo0 | 0x60;
	//
	int v;
	int i;

	/* 0�o�C�g��M�v���͕s���ȃ��N�G�X�g�ł��B */
	if(len <= 0) {
		return 0;
	}

	/* �X�^�[�g�r�b�g(0)������܂ő҂B */
	i = MMC_RECV_TIMEOUT;
	for(;;) {

		/* �N���b�N��Low�ɁB(P06��Low��) */
		*p0d = lo1;
		WAIT1; /* �E�F�C�g1��ȏ�K�v */

		/* �N���b�N��High�ɁB(P06��High��) */
		*p0d = hi1;
		WAIT5; /* �E�F�C�g5��ȏ�K�v */

		/* �f�[�^���擾(P04����f�[�^���擾)�B */
		if(!(*p0d & 0x10)) {
			break; /* �X�^�[�g�r�b�g(0)�����o���� */
		}

		/* �^�C���A�E�g��0��Ԃ��B */
		if(!--i) {
			return 0;
		}
	}

	/* R1 or R2���X�|���X��M���H */
	if(len <= 2) {
		/* 1�r�b�g��M�ς݂ɁB */
		v = 0;
		goto R1_R2;
	}

	/* �����̃f�[�^����M�B */
	do {
		/* 1�o�C�g=8�r�b�g�̃f�[�^����M���܂��B
		 * �Ȃ������������̓N���b�NLow�AHi�̌�̃E�F�C�g�����ł��������l����M�ł��܂��B
		 * ��������f�[�^�]�����n�܂�����AMMC�����̕��G�ȏ����������Ȃ邩��ł��傤���H
		 */
		v = 0;
		*p0d = lo1; *p0d = hi1; if(*p0d & 0x10) v |= 0x80;
R1_R2:		*p0d = lo1; *p0d = hi1; if(*p0d & 0x10) v |= 0x40;
		*p0d = lo1; *p0d = hi1; if(*p0d & 0x10) v |= 0x20;
		*p0d = lo1; *p0d = hi1; if(*p0d & 0x10) v |= 0x10;
		*p0d = lo1; *p0d = hi1; if(*p0d & 0x10) v |= 0x08;
		*p0d = lo1; *p0d = hi1; if(*p0d & 0x10) v |= 0x04;
		*p0d = lo1; *p0d = hi1; if(*p0d & 0x10) v |= 0x02;
		*p0d = lo1; *p0d = hi1; if(*p0d & 0x10) v |= 0x01;
		*buf++ = v;

	} while(--len);

	return 1;
}

//---------------------------------------------------------------------------

int
mmcSendData(const unsigned char *paSendBuff,int iSendLen)
{
	//volatile unsigned char* const pdwait = &PDWAIT;
	volatile unsigned char* const p0d = &P0D;
	//
	const int lo0 = *p0d & ~0x60;
	const int lo1 = lo0 | 0x20;
	const int hi0 = lo0 | 0x40;
	const int hi1 = lo0 | 0x60;
	//
	int data;

	if(iSendLen <= 0) {
		return 0;
	}

	do {
		data = *paSendBuff++;

		/* �f�[�^�A���]�����̓E�F�C�g�����ő��v�݂����ł��B
		 * ���̑���A�Ƃ��ǂ�Busy�N���A���x���Ȃ�݂����ł��B
		 * ���Ԃ񂻂̂Ƃ��A�t���b�V���������̏����������s���Ă���̂��Ǝv���܂��B
		 * �^�C���A�E�g�͏������߂Ɏ���Ă��������B
		 */
		if(data & 0x80) { *p0d = lo1; *p0d = hi1; } else { *p0d = lo0; *p0d = hi0; }
		if(data & 0x40) { *p0d = lo1; *p0d = hi1; } else { *p0d = lo0; *p0d = hi0; }
		if(data & 0x20) { *p0d = lo1; *p0d = hi1; } else { *p0d = lo0; *p0d = hi0; }
		if(data & 0x10) { *p0d = lo1; *p0d = hi1; } else { *p0d = lo0; *p0d = hi0; }
		if(data & 0x08) { *p0d = lo1; *p0d = hi1; } else { *p0d = lo0; *p0d = hi0; }
		if(data & 0x04) { *p0d = lo1; *p0d = hi1; } else { *p0d = lo0; *p0d = hi0; }
		if(data & 0x02) { *p0d = lo1; *p0d = hi1; } else { *p0d = lo0; *p0d = hi0; }
		if(data & 0x01) { *p0d = lo1; *p0d = hi1; } else { *p0d = lo0; *p0d = hi0; }

	} while(--iSendLen);

	return 1;
}
//---------------------------------------------------------------------------
void mmcSendDataCore(unsigned char *paData,int iDataLen)
{

	//�f�[�^�̑��M�R�A
	
	//�_�~�[�N���b�N�𑗂�Ȃ���MMC���ŐF�X������
	//���Ă���Ȃ��Ȃ�̂Œ���
	int i;
	
	//8�N���b�N���̃_�~�[���M
	mmcByteSend(0xFF);
	
	//�w��̃o�C�g���f�[�^�𑗐M
	for(i = 0;i < iDataLen;++i)
	{
		//�P�o�C�g���M
		mmcByteSend(*paData++);
	}

	//8�N���b�N���̃_�~�[���M
	mmcByteSend(0xFF);

}
//---------------------------------------------------------------------------
unsigned short mmcSendCommandAndRecvResponse(unsigned char ucCMD,unsigned long ulArg)
{

	//�R�}���h���M�����X�|���X��M
	//CMD13����R2���X�|���X(2bytes)
	unsigned short usRes = 0xFFFF;		//���X�|���X
	unsigned char ucWork;				//�ꎞ�L��
	unsigned char ucaCommand[6];		//�R�}���h�f�[�^
	//int i;

	//�R�}���h�f�[�^�̍쐬

	//�R�}���h���Z�b�g
	ucaCommand[0] = (0x40 | ucCMD);
	
	//�������Z�b�g
	//for(i = 4;i > 0;--i)
	//{
	//	ucaCommand[i] = (unsigned char)(ulArg & 0xFF);
	//	ulArg >>= 8;
	//}
	//2005/02/09 Change by Madoka �����ł������������邽�߁A�x�^�ŏ����܂�
	{	
		ucaCommand[4] = (unsigned char)(ulArg & 0xFF);
		ulArg >>= 8;
		ucaCommand[3] = (unsigned char)(ulArg & 0xFF);
		ulArg >>= 8;
		ucaCommand[2] = (unsigned char)(ulArg & 0xFF);
		ulArg >>= 8;
		ucaCommand[1] = (unsigned char)(ulArg       );
	}	

	//CRC���Z�b�g
	ucaCommand[5] = 0xFF;	//�_�~�[

	//�R�}���h���M
	mmcSendDataCore(ucaCommand,6);

	//���X�|���X����M
	if(mmcRecvData(&ucWork,1) == 1)
	{
		//���X�|���X���擾
		usRes = ucWork;

		//�������̂�CMD13���H
		if(ucCMD == 13)
		{
			//�������̂���ʂɂ��炷
			usRes <<= 8;

			//�c���1�o�C�g����M
			if(mmcRecvData(&ucWork,1) == 1)
			{
				
				usRes |= (unsigned short)ucWork;

			}
			else usRes = 0xFFFF;	//���s
		}
	}

	return usRes;

}
//---------------------------------------------------------------------------
