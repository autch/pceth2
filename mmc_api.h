//---------------------------------------------------------------------------
// P/ECE MMC����֘A���[�`��
// 2004/05/02 by �܂ǂ�
//
// H    P:http://www2.plala.or.jp/madoka/
// E-mail:madoka@olive.plala.or.jp
//
//�^�u��4�Ō��Ă�������
//---------------------------------------------------------------------------
#ifndef __MMC_CTRL_API__
#define __MMC_CTRL_API__
//---------------------------------------------------------------------------
//���� �萔��` ��������������������������������������������������������������
//---------------------------------------------------------------------------
//#define MMC_FILESIZE_MAX	(5242880)	// = (5 * 1024 * 1024)	�����MMC�h���C�o�ň�����ő�t�@�C���T�C�Y
//#define MMC_FILESIZE_MAX	(120586240)	// = (115 * 1024 * 1024) �����MMC�h���C�o�ň�����ő�t�@�C���T�C�Y 115MB	
#define MMC_FILESIZE_MAX	(15 * 1024 * 1024)

//---------------------------------------------------------------------------
//���� �\���̐錾 ������������������������������������������������������������
//---------------------------------------------------------------------------
//CID(Card IDentification number)�@�J�[�hID���W�X�^���\����
typedef struct tag_MMC_CID_INFO
{
	unsigned char   ucMID;		//ManufactureID(8bit)
	unsigned short  usOID;		//OEM/Application ID(16bit)
	unsigned char   ucPNM[6];	//Product name(48bit)
	unsigned char   ucPRV;		//Product revision(8bit)
	unsigned long   ulPSN;		//Product serial number(32bit)
	unsigned char   ucMDT;		//Manufacturing date(8bit)
	unsigned char   ucCRC;		//7-bit CRC checksum(7bit)
} MMC_CID_INFO;
#define MMC_CID_INFO_SIZE	(16)	//�{����CID���W�X�^���̃T�C�Y

//CSD(Card Specific Data)�@�J�[�h�������W�X�^���\����
//�r�b�g���ƂɐF�X����̂��ʓ|�Ȃ̂ŕK�v�ŏ�����
typedef struct tag_MMC_CSD_INFO
{
	unsigned short usReadBlockLength;	//�ǂݍ��݃u���b�N��(bytes)
	unsigned short usWriteBlockLength;	//�������݃u���b�N��(bytes)

	unsigned long ulCardSize;			//�J�[�h�e��(bytes)
	unsigned long ulTatalBlocks;		//�g�[�^���u���b�N(�Z�N�^)��
		
} MMC_CSD_INFO;
#define MMC_CSD_INFO_SIZE	(16)	//�{����CSD���W�X�^���̃T�C�Y

//---------------------------------------------------------------------------
//���� �֐��̃v���g�^�C�v�錾 ������������������������������������������������
//---------------------------------------------------------------------------

//MMC�̏�����
//MMC�n��API���g�p����O�ɕK��1�x�����Ăяo���Ă��������B
//MMC��̃t�@�C���������ۂ́A�ő�t�@�C���T�C�Y���w�肵�܂��B
//���̏������ł�MMC���̂̏�������API���Ŏg�p���郏�[�N�G���A���q�[�v����m�ۂ��܂��B
//MMC�̏������Ɏ��s�����ꍇ�܂��̓��[�N�G���A�̊m�ۂɎ��s�����ꍇ��0��Ԃ��܂��B
//����I�������ꍇ��1��Ԃ��܂��B
unsigned char mmcInit(unsigned long ulMaxFileSize);

//MMC���������̏������ʂ��擾
//MMC�Ή��J�[�l��Ver.1.27���烆�[�U�A�v���N���O�ɃJ�[�l��
//����MMC�����������ɍs���悤�ɕύX���܂����B
//pceFile�nAPI���t�b�N����悤�ɂ����̂ŁA���[�U��MMC�̑���
//���C�ɂ��邱�Ƃ͖����Ǝv���܂����A�������ɐ����������ǂ���
//��m�肽�����ׂ̈ɂ��̊֐���p�ӂ��܂����B
//�g�p����@����Ȃ����ł���΁A�폜���邩���m��܂���B
unsigned char mmcGetInitResult(void);

//MMC�̏I��
//MMC�n��API���s�v�ɂȂ�����K���Ăяo���Ă��������B
//�����ŏ��������Ɋm�ۂ������[�N�G���A��������܂��B
void mmcExit(void);

//CID���W�X�^�̓��e���擾
//�擾�Ɏ��s�����ꍇ��0���A���������ꍇ��1��Ԃ��܂��B
//�����ɂ�CID���W�X�^���̊i�[��A�h���X���w�肵�Ă��������B
unsigned char mmcGetCIDInfo(MMC_CID_INFO *pCIDInfo);

//CSD���W�X�^�̓��e���擾
//�擾�Ɏ��s�����ꍇ��0���A���������ꍇ��1��Ԃ��܂��B
//�����ɂ�CSID���W�X�^���̊i�[��A�h���X���w�肵�Ă��������B
unsigned char mmcGetCSDInfo(MMC_CSD_INFO *pCSDInfo);

//MMC��pceFileFindOpen
//����ɏI�������ꍇ��0��Ԃ��܂��B
//MMC���ڑ�����Ă��Ȃ����̃G���[�����������ꍇ��1��Ԃ��܂��B
int mmcFileFindOpen(FILEINFO *pfi);

//MMC��pceFileFindNext
//�߂�l	0:�t�@�C����������Ȃ�(pfi�̓��e�͖���)
//		1:�t�@�C������������(pfi�̓��e�͗L��)
int mmcFileFindNext(FILEINFO *pfi);

//MMC��pceFileFindClose
//�߂�l�͏��0�ł�(����I��)
int mmcFileFindClose(FILEINFO *pfi);

//MMC��pceFileOpen
//����I���̏ꍇ0��Ԃ��܂��B
//�t�@�C����������Ȃ��ꍇ��1��Ԃ��܂��B
//�܂��t�@�C���̃T�C�Y���傫������ꍇ��2��Ԃ��܂��B
//�t�@�C���̃f�[�^������N���X�^�Ɍ��ד������������ꍇ��3��Ԃ��܂��B
//FAT�`�F�C���e�[�u�����쐬����̂ɕK�v�ȃq�[�v�̈悪����Ȃ��ꍇ��4��
//�Ԃ��܂��B
//���̑��̃G���[��5��Ԃ��܂��B
int mmcFileOpen(FILEACC *pfa,const char *fname,int mode);

//MMC��pceFileReadSct
//MMC����P/ECE�ł�1�Z�N�^(4096bytes)����ǂݍ��݂܂��B
//sct�ɂ�P/ECE�ł̃Z�N�^�ԍ����w�肵�ĉ������B
//�߂�l�͓ǂݍ��񂾃o�C�g���ł��B
//�Z�N�^�ԍ����K�؂ł͂Ȃ��ꍇ��Apfar��NULL�̏ꍇ�A�܂�MMC���ڑ�
//����Ă��Ȃ��ꍇ�͎��s�Ƃ���0��Ԃ��܂��B
//
//P/ECE�ł̊g���@�\�ł���ptr=NULL�ł̌Ăяo���͈ꉞ�T�|�[�g
//���Ă��܂����A���Ǒ�փo�b�t�@�ɓǂݍ���ł���̂ŁA�����͈����ł��B
//�܂���֕��o�b�t�@��4096�o�C�g�𒴂���A�N�Z�X�͕ۏ؂���܂���B
int mmcFileReadSct(FILEACC *pfa,void *ptr,int sct,int len);

//MMC��pceFileReadSct�k����
//MMC����MMC�ł�1�Z�N�^(512bytes)����ǂݍ��݂܂��B
//sct�ɂ̓t�@�C���̐擪�����MMC�ł̃Z�N�^�����w�肵�ĉ������B
//�߂�l�͓ǂݍ��񂾃o�C�g���ł��B
//�N���X�^�ԍ����K�؂ł͂Ȃ��ꍇ��Apfar��NULL�̏ꍇ�A�܂�MMC���ڑ�
//����Ă��Ȃ��ꍇ�͎��s�Ƃ���0��Ԃ��܂��B
//
//P/ECE�ł̊g���@�\�ł���ptr=NULL�ł̌Ăяo���͈ꉞ�T�|�[�g
//���Ă��܂����A���Ǒ�փo�b�t�@�ɓǂݍ���ł���̂ŁA�����͈����ł��B
//�܂���֕��o�b�t�@��4096�o�C�g�𒴂���A�N�Z�X�͕ۏ؂���܂���B
int mmcFileReadMMCSct(FILEACC *pfa,void *ptr,int sct,int len);

//MMC��pceFileClose
//����I���̏ꍇ0��Ԃ��܂��B
int mmcFileClose(FILEACC *pfa);

//MMC���pex�t�@�C�������s
//�߂�l�͏��0�ł�
int mmcAppExecFile(const char *fname,	//�t�@�C����
				   int resv);			//�\��(�K��0�ɂ��ĉ�����)

//2004/11/02 nsawa���񂩂�̌�v�]�ɂ��O���Ɍ��J
//
//�Z�N�^���[�h(�჌�x��MMC����֐�)
//MMC����1�Z�N�^��(512�o�C�g��z��)�f�[�^��ǂݏo���܂�
//
//unsigned long ulSector	�ǂݏo���ΏۃZ�N�^(�����Z�N�^�ԍ�)
//unsigned char *pBuff		�o�b�t�@(�K��512�o�C�g�ȏ゠�邱��)
//
//����ɓǂݏo�����ꍇ��1���A�G���[�����������ꍇ��0��Ԃ��܂�
unsigned char mmcReadSector(unsigned long ulSector,unsigned char *pucBuff);

#if 0  //2005/03/12 Vanished by Madoka
//{{2004/12/13 Add by Naoyuki Sawa
extern int mbr_protect;
unsigned char mmcWriteSector(unsigned long ulSector,const unsigned char *pucBuff);
//}}2004/12/13 Add by Naoyuki Sawa
#endif //2005/03/12 Vanished by Madoka

//#define MMC_RECV_TIMEOUT		0xFFFF		//��M�^�C���A�E�g�l
//��2004/12/13 Change by N.SAWA �������������A������ƒ��߂ɂ��܂����B
#define MMC_RECV_TIMEOUT		0xFFFFF		//��M�^�C���A�E�g�l
//#define MMC_POLLING_TIMEOUT		1000		//��M�|�[�����O���̃^�C���A�E�g��
//��2004/12/13 Change by N.SAWA �������������A������ƒ��߂ɂ��܂����B
#define MMC_POLLING_TIMEOUT		10000		//��M�|�[�����O���̃^�C���A�E�g��

//2005/03/12 Add by Madoka
#define APPNF_INITMMC 7     //pceAppNotify�Œʒm�����J�[�l����MMC�������₢���킹
                            //�̒ʒm�^�C�v
                            //
                            //pceAppNotify�̕ԓ��ł̏��u
                            //APPNR_ACCEPT  or APPNR_IGNORE �c MMC�����������܂�
                            //APPNR_SUSPEND or APPNR_REJECT �c MMC�����������܂���
                            //
//---------------------------------------------------------------------------
#endif	//#ifndef __MMC_CTRL_API__
//---------------------------------------------------------------------------
