//---------------------------------------------------------------------------
// P/ECE MMC����֘A���[�`��
// 2004/05/02 by �܂ǂ� �쐬�J�n
// 2004/05/16 by �܂ǂ� �ǂݍ��ݎ��̃Z�N�^��N���X�^�̌v�Z��
//                      �����Ԉ���Ă����̂��C���B
// 2004/08/29 by �܂ǂ� mmcFileReadSct��mmcFileReadSct(&pfa, NULL, sct, 0);
//                      �Ƃ����ꍇ�ɁA�����̑�փo�b�t�@�ɉ����ǂݍ��܂��
//                      ���Ȃ������s����C���B�܂��AmmcFileReadMMCSct�ł�
//                      ���l�ɏC���B�����āAmmcFileReadMMCSct�ł͑�փo�b�t�@
//                      �A�N�Z�X�ۏ؃T�C�Y��512�o�C�g�ɕύX�B
// 2004/08/30 by �܂ǂ� SD�J�[�h�Ə������Ɏ��Ԃ�������MMC�ɑΉ��B
//                      ���񋟂��Ă���������nsawa����(http://piece.no-ip.org/)
//                      ���肪�Ƃ��������܂�m(-_-)m
// 2004/11/06 by �܂ǂ� �X�[�p�[�t���b�s�[�`���ɑΉ����܂����B
//                      ���񋟂��Ă���������nsawa����(http://piece.no-ip.org/)
//                      ���肪�Ƃ��������܂�m(-_-)m
// 2004/11/15 by N.SAWA MS-DOS�̃V���[�g�t�@�C�����Ƃ��ċ�����镶�����g���Ă���
//                      �G���g�����������ʂɊ܂߂鏈����ǉ����܂���
//                      �f�B���N�g���E�G���g���܂��̓{�����[�����x���̑���������
//                      �G���g�����������珜�O���鏈����ǉ����܂����B
//                      �A�v���P�[�V�����ɒ��ڑg�ݍ���ŃR���p�C������ꍇ��
//                      �K�v�̖���������#ifdef __PCEKN__�}�N���ɂď��O���܂����B
// 2004/12/12 by N.SAWA mmcInit()��pceFile�nAPI���t�b�N���A�{��FlashROM��MMC��
//                      ��ʂȂ�������悤�ɂ��܂����B
//                      �܂��AmmcExit()���Ƀt�b�N���������܂��B
// 2004/12/12 by N.SAWA mmcFileReadSct()�ɂāA�t�@�C���I�[�𒴂���ǂݍ��݃o�C�g
//                      �����w�肳�ꂽ�ꍇ�́A�ǂݍ��݃o�C�g����؂�l�߂�悤��
//                      �ύX���܂����BpceFileReadSct()�Ƃ̌݊��̂��߂ł��B
// 2004/12/12 by N.SAWA mmcFileReadSct()�ɂāA�t�@�C���I�[�𒴂���ǂݍ��݃o�C�g
//                      �����w�肳�ꂽ�ꍇ�́A�ǂݍ��݃o�C�g����؂�l�߂�悤��
//                      �ύX���܂����BpceFileReadSct()�Ƃ̌݊��̂��߂ł��B
// 2004/12/12 by N.SAWA �������̂��߁AmmcByteSend()��mmcRecvData()��mmc_fram.c��
//                      �ړ����܂����B
// 2004/12/12 by N.SAWA mmcByteRecv()�͎g���Ă��Ȃ��̂ō폜���܂����B
//                      SEND_CLOCK_WAIT���s�v�ɂȂ����̂ō폜���܂����B
// 2004/12/12 by N.SAWA �������̂��߁AmmcFileReadSct()��mmcFileReadMMCSct()�ł�
//                      �ڑ��m�F�͏Ȃ��܂����B
// 2004/12/12 by N.SAWA mmcFileReadSct()�͊��荞�ݓ����痘�p���܂��B���荞�ݒ�
//                      �̓X�^�b�N��FRAM2�G���A�ɐ؂�ւ��邽�߃X�^�b�N�̈悪��
//                      �Ȃ��AucaBuff[512]�ɂ���ăX�^�b�N������̂�h�����߁A
//                      static�ɕύX���܂����B
//                      ���̑��̊֐��ɂ����傫�߂̃��[�J���z�񂪂���܂����A
//                      ���荞�ݓ����痘�p���Ȃ��̂ŁA���̂܂܂ɂ��Ă���܂��B
//                      2005/03/20 �ǋL by �܂ǂ� 
//                      ��L�̏C���������悤�Ƃ����̂ł����A�J�[�l������512�o�C�g
//                      ��static�ϐ�����邱�Ƃ́A�������c�ʂ̊֌W�Ō����炴���
//                      ���Ȃ��Ȃ�܂����B
//                      �ł��̂ŁA����RAM��ɃX�^�b�N��؂�ւ��ė��p����ꍇ�́A
//                      �����ӂ��������B
// 2004/12/12 by N.SAWA ��փo�b�t�@�����蓖�Ă̏�ԂŁA���荞�ݒ��ɏ���ǂݍ���
//                      ���s�����ꍇ�A���荞�ݓ���pceHeapAlloc()���g���Ă��܂���
//                      �ƂɂȂ�댯�ł��̂ŁA��փo�b�t�@�͖������ōŏ��Ɋm�ۂ�
//                      �Ă��܂����Ƃɂ��܂����B
// 2004/12/13 by N.SAWA mmcWriteSector()��ǉ����܂����B
//                      ����ɔ����AmmcByteRecv()�𕜌����܂����B
// 2005/02/09 by �܂ǂ� PMF�v���C���[�𐻍삷��ۂɁA�ǂݍ��ݑ��x���Ԃɍ���Ȃ��̂�
//                      �������̌�������ɍ������B
//                      �����āAmmcSendDataCore��mmcSendCommandAndRecvResponse��
//                      ����RAM��Ɉړ��B���J�[�l�����ł͍���RAM��ɒu���܂���B
// 2005/03/12 by �܂ǂ� mmcFileReadSct���ŃO���[�o���ϐ������[�J���Ɉڂ��Ďg�p��
//                      �����ɍ�������}���Ă݂܂����B
//                      �܂��Ansawa����̏������݊֘A�֐���#if 0�ŉB���܂����B
// 2005/03/12 by �܂ǂ� Ver.1.27����J�[�l������MMC�̏����������݂�悤�ɂ�����
//                      �ŁA���̎��s���ʂ��擾����
//                      unsigned char mmcGetInitResult(void) ��ǉ��B
// 2005/03/20 by �܂ǂ� 512MB�ȏ�1GB�ȓ��̃������J�[�h�ɑΉ����܂����B
// 2005/03/20 by �܂ǂ� �������̂��߁AmmcFileFindNext()�ł̐ڑ��m�F�͏Ȃ��܂����B
// 2005/04/02 by �܂ǂ� �V�X�e�����j���[�Őݒ肷��J�[�l����MMC�������L���t���O
//                      ��ǉ����܂����B
// 2005/04/05 by �܂ǂ� mmcFileReadSct��ptr=NULL,len=0���܂�����ɓ����Ȃ��Ȃ���
//                      ���܂��Ă����s����C�����܂����B
//                      ���̏C���ɔ��Ȃ��AmmcFileReadMMCSct�ɐ؂�l�ߏ������ǉ��B
// 2005/04/24 by �܂ǂ� �t�@�C���������Ɉ�����ő�T�C�Y�҂�����̃t�@�C�����͂�
//                      �����������������̂��C���B(����:nsawa����)
// 2005/05/04 by �܂ǂ� MMC���C�u�������J�[�l���ł͂Ȃ��A�v�����ŗ��p����ꍇ�́A
//                      ���[�N�G���A���q�[�v����m�ۂ��Ȃ��ŁA�O���[�o���z��Ƃ�
//                      �Ē�`�ł���悤�ɕύX�B�]���ǂ���q�[�v�����邱�Ƃ�
//                      �\�ł��B���[�N�G���A���O���[�o���z��Ŏ��ꍇ�́A
//                      Makefile��__MMC_WORK_GLOBAL__���`���Ă��������B
// 2005/05/04 by �܂ǂ� pceFile�nAPI�̃t�b�N��__PCEFILE_API_HOOK__�̒�`��������
//                      �t�b�N����E���Ȃ��𐧌䂷�邱�Ƃɂ��܂����B
// 2005/05/07 by �܂ǂ� ������ő�t�@�C���T�C�Y���J�[�h�̗e�ʂ𒴂���ꍇ�́A�ő�
//                      �t�@�C���T�C�Y���J�[�h�̗e�ʂɐ؂�l�߂�悤�ɏC�����܂����B
//                      ����ɂ��A64MB�̃J�[�h��50MB�ȏ�̃t�@�C���������Ȃ�����
//                      �s����C������܂��B
//
//
// H    P:http://www2.plala.or.jp/madoka/
// E-mail:madoka@olive.plala.or.jp
//
//�^�u��4�Ō��Ă�������
//---------------------------------------------------------------------------
//���� �t�@�C���C���N���[�h ��������������������������������������������������
//---------------------------------------------------------------------------
#include <s1c33cpu.h>
#include <string.h>
#include <stdio.h>
#include <piece.h>
#include "mmc_api.h"
#ifdef __PCEKN__		// 2004/11/15 Add by N.SAWA
#include "pcekn.h"
#endif //#ifdef PCEKN	// 2004/11/15 Add by N.SAWA

//---------------------------------------------------------------------------
//���� ���W�X�^�A�N�Z�X��` ��������������������������������������������������
//---------------------------------------------------------------------------
//�e���W�X�^�ւ̃A�h���X

//���o�͌��p�|�[�g�֘A
//CPU�f�[�^�V�[�gP.370,P.372�Q��
#define P0CF	(*(volatile unsigned char*)0x402D0)		//P0�@�\�I�����W�X�^
#define P0D		(*(volatile unsigned char*)0x402D1)		//P0���o�͌��p�|�[�g�f�[�^���W�X�^
#define P0IOC	(*(volatile unsigned char*)0x402D2)		//P0I/O���䃌�W�X�^
#define P1CF	(*(volatile unsigned char*)0x402D4)		//P1�@�\�I�����W�X�^
#define P1D		(*(volatile unsigned char*)0x402D5)		//P1���o�͌��p�|�[�g�f�[�^���W�X�^
#define P1IOC	(*(volatile unsigned char*)0x402D6)		//P1I/O���䃌�W�X�^
#define PCFEX	(*(volatile unsigned char*)0x402DF)		//�|�[�g�@�\�g�����W�X�^

//�}�N����`
#define CARD_ENABLE		{ P1D &= ~(1 << 4); }			//�J�[�h��Active�ɂ��܂�
#define CARD_DISABLE	{ P1D |=  (1 << 4); }			//�J�[�h��NonActive�ɂ��܂�	
//---------------------------------------------------------------------------
//���� �t�@�C�����萔��` ����������������������������������������������������
//---------------------------------------------------------------------------
//#define MMC_RECV_TIMEOUT			0xFFFF	//��M�^�C���A�E�g�l
//#define MMC_POLLING_TIMEOUT		1000	//��M�|�[�����O���̃^�C���A�E�g��
//											//2004/08/30 SD�J�[�h����сA�������Ɏ��Ԃ�
//											//������MMC�J�[�h�ɑΉ�
//											//(����:nsawa����)
//��2004/12/13 Move to mmc_api.h by N.SAWA

#define VALIDFILEACC 0x9ce6					//"file.c"���甲��

//#define SEND_CLOCK_WAIT 10				//��������N���b�N�p�̃E�G�C�g(48MHz���[�h���Ɏg�p)
//											//�����������������Ă��A�ȂɊ���Ă̓f�R�[�h������
//											//�ǂݍ��݂��Ԃɍ���Ȃ��A�܂��́A���߂���ʐM������
//											//�f�[�^�������铙�̌����œr���Ńt���[���G���[�ɂȂ�
//											//�Ȃ��r���ŏI��ꍇ������܂��B
//											//���̂Ƃ���A����ȏ�̒����͓�����Ȃ̂ŁA��������
//											//���̂��炢�̒����Ŏ~�߂Ă������Ƃɂ��܂��B
//											//
//											//���Ȃ݂ɂ���MMC��MP3�v���C���[�ł�32kHz 48kbps��
//											//�t�@�C�������������Ԃɍ����܂���B
//											//���Ƃ�����32kbps�̃t�@�C���͉����������̂ŁA�I�X�X��
//											//�ł��܂���(^^;
//��2004/12/12 Delete by N.SAWA

//2005/05/04 Added by Madoka >>>��������

//���[�N�G���A���q�[�v������Ȃ��Ȃ������ɁA
//�O���[�o���ϐ��Ƃ��Ē�`���邽�߂ɗ��p����
//�萔����
#define PIECE_SECTOR_SIZE		(4096)		//P/ECE�ł̃Z�N�^�T�C�Y
#define MMC_MIN_CLUSTER_SIZE	(512)		//MMC�ł̍ŏ��N���X�^�T�C�Y
#define MMC_FATCHAIN_MAX		(((MMC_FILESIZE_MAX / MMC_MIN_CLUSTER_SIZE) + 1) * sizeof(unsigned short))	//�ő�T�C�Y�t�@�C����FAT�`�F�C���e�[�u���T�C�Y
#define MMC_WORKAREA_MAX		(MMC_FATCHAIN_MAX + PIECE_SECTOR_SIZE)	//MMC���C�u�����ŗ��p���郏�[�N�G���A�̍ő�T�C�Y

//2005/05/04 Added by Madoka <<<�����܂�

//---------------------------------------------------------------------------
//���� �t�@�C�����\���̒�` ����������������������������������������������������
//---------------------------------------------------------------------------
//MMC�̏��\����
typedef struct tag_MMC_INFO
{
	unsigned char bInit;		//�������ς݃t���O
	
	MMC_CID_INFO tCIDInfo;		//CID���
	MMC_CSD_INFO tCSDInfo;		//CSD���

} MMC_INFO;

//MBR�̃p�[�e�B�V�����G���g���\����
typedef struct tag_PARTITIONENTRY_AT
{
    unsigned char  ucStatus;				//�̈���(0x00:�X���[�v�A0x80:�A�N�e�B�u)
    unsigned char  ucStartHead;				//�̈�̊J�n�w�b�h
    unsigned short usStartCylinderSecter;	//�̈�̊J�n�V�����_�E�Z�N�^
    unsigned char  ucType;					//�̈�̎��
    unsigned char  ucEndHead;				//�̈�̏I���w�b�h
    unsigned short usEndCylinderSecter;		//�̈�̏I���V�����_�E�Z�N�^
    unsigned long  ulOffsetSectors;			//MBR�̐擪����̈�̐擪�܂ł̃Z�N�^��
    unsigned long  ulSizeSector;			//�̈�̃Z�N�^��

} PARTITIONENTRY_AT;

//�f�B���N�g���G���g���\����
typedef struct tag_DIR_ENTRY
{
	char			sFileName[8];		//�t�@�C����(�폜�t�@�C����1�����ڂ�0xE5�A���g�p�G���g����1�����ڂ�0x00)
    char			sExtension[3];		//�g���q(3�����ɖ����Ȃ��ꍇ�̎c��̕�����0x20�A�t�@�C���������l)
    unsigned char	ucAttribute;		//����(R:0x01�AH:0x02�AS:0x04�AA:0x20�ADir:0x10�AVol:0x08)
    unsigned char	ucaReserved[10];	//�\��(VFAT�ŗ��p�A���p���Ȃ��ꍇ��0�t�B��)
    unsigned short	usTime;				//�쐬����
    unsigned short	usDate;				//�쐬��
    unsigned short  usCluster;			//�t�@�C���܂��̓f�B���N�g���̐擪�N���X�^
    unsigned long	ulFileSize;			//�t�@�C���T�C�Y(Dir�AVol�͏��0�A����ȊO�̃t�@�C����0�̂Ƃ��A�N���X�^��0)

} DIR_ENTRY;

//�t�@�C���V�X�e�����\����
typedef struct tag_FILESYSTEM_INFO
{	
	unsigned char   ucSectorsParCluster;		//1�N���X�^������̃Z�N�^��
	unsigned char   ucSectorsParClusterShift;	//1�N���X�^������̃Z�N�^�����v�Z�Ŏg�p���鎞�̃V�t�g��
	unsigned char   ucClustorSizeShift;			//1�N���X�^������̃o�C�g�����v�Z�Ɏg�p���鎞�̃V�t�g��
	unsigned short  usFATOffsetSecters;			//�_���Z�N�^0(������u�[�g�Z�N�^)����FAT�̈�܂ł̃Z�N�^��(���̓u�[�g���R�[�h�̃T�C�Y�ł���)
	unsigned short	usRootDirEntryMax;			//���[�g�f�B���N�g���G���g���̍ő吔(FAT12/16�p)
	unsigned short  usFATSize;					//FAT�̃Z�N�^��(FAT12/16�p)
	unsigned long   ulVolumeSerialID;			//�{�����[���V���A��ID(�t�H�[�}�b�g���Ƀ����_���Ɋ��蓖�Ă���)

	unsigned long	ulFATAddress;				//FAT�̕����Z�N�^�ԍ�
	unsigned long	ulRootDirAddress;			//���[�g�f�B���N�g���̕����Z�N�^�ԍ�
	unsigned long	ulFirstClusterAddress;		//�擪�N���X�^(�N���X�^�ԍ�2)�̕����Z�N�^�ԍ�
	unsigned short	*pusaFATchain;				//FAT�`�F�C���e�[�u��(�ő�5MB���܂�) ��:���̎d�l�ɂ��5MB�𒴂���t�@�C���͈����܂���  
	DIR_ENTRY		*ptaDirTable;				//�f�B���N�g���G���g���e�[�u��(1�Z�N�^��)
	unsigned char	*pucaReadSubBuff;			//mmcFileReadSct�p�̑�փo�b�t�@(4096�o�C�g��)

} FILESYSTEM_INFO;

//������Ɨp�t�@�C���������\����
typedef struct tag_MMC_FILEFIND_INFO
{
	unsigned short usDirIndex;			//�������̃f�B���N�g���G���g���C���f�b�N�X
	unsigned char  ucDummy[14];			//�T�C�Y���킹�̃_�~�[

} MMC_FILEFIND_INFO;

//---------------------------------------------------------------------------
//���� �t�@�C�����O���[�o���ϐ���` ������������������������������������������������
//---------------------------------------------------------------------------
static MMC_INFO			g_tMMCInfo;
static FILESYSTEM_INFO	g_tFS_Info;
static unsigned long g_ulMaxFileSize;
static unsigned char g_ucInitResult;	//2005/03/12 Add by Madoka ���������̖߂�l�ێ��ϐ�

//2005/05/04 Added by Madoka
//���[�N�G���A���O���[�o���z��ɂƂ�̂́A
//�A�v�����ł̂݋����܂�
#if (!defined(__PCEKN__)) && defined(__MMC_WORK_GLOBAL__)	
static unsigned char g_ucMMCWorkArea[MMC_WORKAREA_MAX];		//MMC���C�u�����ŗ��p���郏�[�N�G���A	
#endif //(!defined(__PCEKN__)) && defined(__MMC_WORK_GLOBAL__)	

//---------------------------------------------------------------------------
//���� �t�@�C�����݂̂̊֐��v���g�^�C�v�錾 ����������������������������������������
//---------------------------------------------------------------------------

//�~���b�P�ʂ̎w��̎��ԑ҂�(�ėp)
void mmcWait(unsigned long ulWaitTime);

//�e��I/O�̏�����
void mmcInitIO(void);

//�Œ胏�[�N�G���A�̊m��
//MMC�Ƃ̒ʐM�Ɏg�p����Œ蒷�̃��[�N�G���A��
//�q�[�v�̈悩��m�ۂ��܂��B
//�m�ۂɎ��s�����ꍇ0��Ԃ��܂��B
unsigned char mmcAllocFixedWorkArea(void);

//MMC�������̃R�A����
//�����������Ɏ��s�����ꍇ0��Ԃ��܂��B
unsigned char mmcInitCore(void);

//MMC��SPI���[�h������
//�������Ɏ��s�����ꍇ��0��Ԃ��܂��B����I����1��Ԃ��܂��B
unsigned char mmcInitSPIMode(void);

//1�o�C�g���M
//inline void mmcByteSend(unsigned char ucData);
//��2004/12/12 Change by N.SAWA
void mmcByteSend(int data);

//�f�[�^�̑��M�R�A
void mmcSendDataCore(unsigned char *paData,int iDataLen);

//�R�}���h���M�����X�|���X��M
//CMD13����R2���X�|���X(2bytes)
unsigned short mmcSendCommandAndRecvResponse(unsigned char ucCMD,
											 unsigned long ulArg);

//1�o�C�g��M
//inline unsigned char mmcByteRecv(void);
//��2004/12/13 Change by N.SAWA inline�͂�߂܂����B
unsigned char mmcByteRecv(void);

//�f�[�^��M
//�X�^�[�g�r�b�g�ȍ~�̎w��̃o�C�g������M���܂�
//�^�C���A�E�g�����������ꍇ0��Ԃ��܂��B����I����1��Ԃ��܂��B
//unsigned char mmcRecvData(unsigned char *paRecvBuff,int iRecvLen);
//��2004/12/13 Change by N.SAWA
int mmcRecvData(unsigned char *paRecvBuff,int iRecvLen);

//�ڑ��X�`�F�b�N
//����ɐڑ�����Ă���ꍇ��0��Ԃ��B
//�G���[�ɂȂ����ꍇ��1��Ԃ��܂��B
unsigned char mmcCheckConnection(void);

//�}�X�^�[�u�[�g���R�[�h����u�[�g�Z�N�^�̃I�t�Z�b�g���擾
//���s�����ꍇ��0xFFFFFFFF��Ԃ�
unsigned long mmcGetBootSecterOffset(void);

//�u�[�g�Z�N�^����t�@�C���V�X�e�������擾
//�����ɂ�mmcGetBootSecterOffset�Ŏ擾����MBR����u�[�g�Z�N�^
//�܂ł̃I�t�Z�b�g����͂��܂��B
//����ɓǂݏo�����ꍇ��1���A���s�����ꍇ��0��Ԃ��܂��B
unsigned char mmcGetFileSystemInfo(unsigned long ulBootSecterOffset);

//�f�B���N�g���G���g�������̃R�A
DIR_ENTRY* mmcSearchCore(MMC_FILEFIND_INFO* pInfo);

//FAT�f�B���N�g���G���g���̃t�@�C��������P/ECE�̃t�@�C�����ɃR�s�[
void mmcCopyFatFileNameToPieceFileName(char *pPieceFile,char *pFatFile,char *pFatExt);

//�t�@�C������
DIR_ENTRY* mmcSearchFile(const char *fname);

//FAT�`�F�C���e�[�u���̍쐬
//�w��̃t�@�C���̐擪�N���X�^�ƃT�C�Y���w�肵�܂��B
//FAT�̃`�F�C���̓r���Ō��׃N���X�^�������������ꍇ��1��Ԃ��܂��B
//����I���̏ꍇ��0��Ԃ��܂��B
int mmcCreateFATChainTable(const unsigned short usFirstCluster,
						   const unsigned long ulFileSize);

int mmcWaitStartBit(void); // 2004/12/13 Add by N.SAWA
int mmcWaitBusy(void);     // 2004/12/13 Add by N.SAWA

int mmcSendData(const unsigned char *paSendBuff,int iSendLen);

//---------------------------------------------------------------------------
//���� �֐��C���v�������g ����������������������������������������������������
//---------------------------------------------------------------------------
unsigned char mmcInit(unsigned long ulMaxFileSize)
{

	//MMC�̏�����
	//MMC�n��API���g�p����O�ɕK��1�x�����Ăяo���Ă��������B
	//API���Ŏg�p���郏�[�N�G���A���q�[�v����m�ۂ��܂��B
	//MMC�̏������Ɏ��s�����ꍇ�܂��̓��[�N�G���A�̊m�ۂɎ��s�����ꍇ��0��Ԃ��܂��B
	//����I�������ꍇ��1��Ԃ��܂��B

	//�g�p����I/O�̏��������s���܂�
	mmcInitIO();

	//�����t�@�C���̍ő�T�C�Y��ݒ�
	if(ulMaxFileSize > MMC_FILESIZE_MAX)
		g_ulMaxFileSize = MMC_FILESIZE_MAX;
	else
		g_ulMaxFileSize = ulMaxFileSize;
		
	//MMC���̏�����
	//return mmcInitCore();
	//��2004/12/12 Change by N.SAWA
	{
		static void hook_mmc();
		
		//2005/03/12 Changed by Madoka
		//Old::
		//int retval = mmcInitCore();
		//if(retval == 1) {
		//	hook_mmc();
		//}
		//
		//return retval;
		g_ucInitResult = mmcInitCore();

//2005/05/04 Changed by Madoka
//pceFile�nAPI�̃t�b�N��__PCEFILE_API_HOOK__����`����Ă���
//�Ƃ��̂ݎ��s
#ifdef __PCEFILE_API_HOOK__
		if(g_ucInitResult == 1) {
			hook_mmc();
		}
#endif //__PCEFILE_API_HOOK__
		
		return g_ucInitResult;
	}
}
//---------------------------------------------------------------------------
//��2005/03/12 Add by Madoka
unsigned char mmcGetInitResult(void)
{
	return g_ucInitResult;
}
//---------------------------------------------------------------------------
unsigned char mmcInitCore(void)
{

	//MMC�������̃R�A����
	//�����������Ɏ��s�����ꍇ0��Ԃ��܂�

	unsigned long ulOffsetSector;	//MBR����u�[�g�Z�N�^�܂ł̃I�t�Z�b�g�Z�N�^��
	
	//�������ς݃t���O�N���A
	g_tMMCInfo.bInit = 0;

	//MMC��SPI���[�h�ŏ��������܂�
	if(mmcInitSPIMode() == 0)
		return 0;	//���������s

	//CID���̎擾
	if(mmcGetCIDInfo(&g_tMMCInfo.tCIDInfo) == 0)
		return 0;	//�擾���s

	//CSD���̎擾
	if(mmcGetCSDInfo(&g_tMMCInfo.tCSDInfo) == 0)
		return 0;	//�擾���s

	//MBR����u�[�g�Z�N�^�܂ł̃I�t�Z�b�g���擾
	ulOffsetSector = mmcGetBootSecterOffset();

	//�����Ǝ擾�ł������H
	if(ulOffsetSector == 0xFFFFFFFF)
		return 0;

	//2004/11/06 Add by Madoka
	//�X�[�p�[�t���b�s�[�`���ɑΉ�(����:nsawa����(http://piece.no-ip.org/))
	//���肪�Ƃ��������܂��B
	//
	//�u�[�g�Z�N�^����t�@�C���V�X�e���̏����擾
    if((ulOffsetSector == 0xFFFFFFFF) ||
       (mmcGetFileSystemInfo(ulOffsetSector) == 0))
    {
        //�X�[�p�[�t���b�s�[�`���Ɖ��肵�ă��g���C
        //(�X�[�p�[�t���b�s�[�`���ɂ�MBR���������߁A�I�t�Z�b�g0��)
		ulOffsetSector = 0;
        if(mmcGetFileSystemInfo(ulOffsetSector) == 0)
            return 0;
    }

	//�Œ蒷�̃��[�N�G���A���q�[�v�̈悩��擾
	if(mmcAllocFixedWorkArea() == 0)
		return 0;

	//2004/12/12 Add by N.SAWA
	//��փo�b�t�@�����蓖�Ă̏�ԂŁA���荞�ݒ��ɏ���ǂݍ��݂��s�����ꍇ�A
	//���荞�ݓ���pceHeapAlloc()���g���Ă��܂����ƂɂȂ�댯�ł��̂ŁA
	//��փo�b�t�@�͖������ōŏ��Ɋm�ۂ��Ă��܂����Ƃɂ��܂����B
	if(g_tFS_Info.pucaReadSubBuff == NULL)	//2005/03/20 Add by Madoka ��փo�b�t�@�̃T�C�Y�͕ς��Ȃ��̂œ�d�m�ۖh�~
	{
//2005/05/04 Changed by Madoka	���[�N�G���A�̓O���[�o���z���	
#if (!defined(__PCEKN__)) && defined(__MMC_WORK_GLOBAL__)	
		g_tFS_Info.pucaReadSubBuff = (unsigned char*)(g_ucMMCWorkArea + MMC_FATCHAIN_MAX);
#else
		g_tFS_Info.pucaReadSubBuff = (unsigned char*)pceHeapAlloc(PIECE_SECTOR_SIZE);
		if(g_tFS_Info.pucaReadSubBuff == NULL)
			return 0;	
#endif //(!defined(__PCEKN__)) && defined(__MMC_WORK_GLOBAL__)			
	}
	
	//�������I��
	g_tMMCInfo.bInit = 1;	//�������ς݃t���OON

	//����I��
	return 1;

}
//---------------------------------------------------------------------------
void mmcExit(void)
{

	//MMC�̏I��
	//MMC�n��API���s�v�ɂȂ�����K���Ăяo���Ă��������B
	//�����ŏ��������Ɋm�ۂ������[�N�G���A��������܂��B

//2005/05/04 Changed by Madoka
//pceFile�nAPI�̃t�b�N��__PCEFILE_API_HOOK__����`�����
//���鎞�̂ݎ��s
#ifdef __PCEFILE_API_HOOK__	
	//{{2004/12/12 Add by N.SAWA
	{
		static void unhook_mmc();
		unhook_mmc();
	}
	//}}2004/12/12 Add by N.SAWA
#endif //__PCEFILE_API_HOOK__

	//�m�ۂ����q�[�v�����
	if(g_tFS_Info.ptaDirTable != NULL)
	{
		pceHeapFree(g_tFS_Info.ptaDirTable);
		g_tFS_Info.ptaDirTable = NULL;
		g_tFS_Info.pusaFATchain = NULL;		
	}
		
	if(g_tFS_Info.pucaReadSubBuff != NULL)
	{
		pceHeapFree(g_tFS_Info.pucaReadSubBuff);
		g_tFS_Info.pucaReadSubBuff = NULL;			
	}

}
//---------------------------------------------------------------------------
void mmcWait(unsigned long ulWaitTime)
{

	//�~���b�P�ʂ̎w��̎��ԑ҂�(�ėp)

	unsigned long ulStartTime = pceTimerGetCount();
	
	//�w��̎��Ԃ��o�܂Ń��[�v
	while((pceTimerGetCount() - ulStartTime) < ulWaitTime);

}
//---------------------------------------------------------------------------
void mmcInitIO(void)
{

	//�e��I/O�̏�����

	//�|�[�g�@�\�g�����W�X�^��P04,P05,P06,P14��
	//�ėp�|�[�g�Ƃ��Ďg�p�ł���悤�ɂ���
	//(CPU�f�[�^�V�[�gP.372 �|�[�g�@�\�g�����W�X�^���Q��)
	PCFEX &= 0x8E;

	//�ȉ��̏ڍׂ�CPU�f�[�^�V�[�gP.370���Q�Ƃ��Ă�������

	//CS(Chip Select)�ɐڑ�����Ă���P14�|�[�g�̐ݒ�
	P1CF  &= 0xEF;		//P14���g�p�\��
	P1IOC |= (1 << 4);	//P14���o�̓|�[�g�Ƃ��Đݒ�

/* �ʂɐݒ肷��ꍇ
	//SCLK(Serial CLocK)�ɐڑ�����Ă���P06�|�[�g�̐ݒ�
	P0CF &= 0xBF;		//P06���g�p�\��
	P0IOC |= (1 << 6);	//P06���o�̓|�[�g�Ƃ��Đݒ�
	
	//DI(Data In)�ɐڑ�����Ă���P05�|�[�g�̐ݒ�
	P0CF &= 0xDF;		//P05���g�p�\��
	P0IOC |= (1 << 5);	//P05���o�̓|�[�g�Ƃ��Đݒ�
	
	//DO(Data Out)�ɐڑ�����Ă���P04�|�[�g�̐ݒ�
	P0CF &= 0xEF;		//P04���g�p�\��
	P0IOC &= ~(1 << 4);	//P04����̓|�[�g�Ƃ��Đݒ�
*/

	//P04,P05,P06����x�ɐݒ�
	P0CF &= 0x8F;		//P04,P05,P06���g�p�\��
	P0IOC |= 0x60;		//P05,p06�͏o�̓|�[�g�Ƃ��Đݒ�
	P0IOC &= ~(1 << 4);	//P04�͓��̓|�[�g�Ƃ��Đݒ�
	
}
//---------------------------------------------------------------------------
unsigned char mmcAllocFixedWorkArea(void)
{

	//�Œ胏�[�N�G���A�̊m��
	
	//�Œ胏�[�N�G���A�̊m��
	//MMC�Ƃ̒ʐM�Ɏg�p����Œ蒷�̃��[�N�G���A��
	//�q�[�v�̈悩��m�ۂ��܂��B
	//�m�ۂɎ��s�����ꍇ0��Ԃ��܂��B
	unsigned char  ucRet = 1;
	unsigned short usClusterNum;
	unsigned long  ulHeapSize = sizeof(DIR_ENTRY) * 16;	//�K�v�ȃq�[�v�T�C�Y

	//MMC�𐧌䂷��̂ɕK�v�ȃ��[�N�G���A����C�Ɋm��
	//���̌�A�K�v�ȕ����Ƀ|�C���^�𕪂���

	//2005/05/07 Added by Madoka
	//�����ő�t�@�C���T�C�Y���J�[�h�̗e�ʂ𒴂��Ă���ꍇ��
	//�ő�t�@�C���T�C�Y���J�[�h�e�ʂ̃T�C�Y�ɐ؂�l�߂�
	if(g_ulMaxFileSize > g_tMMCInfo.tCSDInfo.ulCardSize)
		g_ulMaxFileSize = g_tMMCInfo.tCSDInfo.ulCardSize;

	//�����ő�t�@�C���T�C�Y����K�v�ȃN���X�^�����Z�o
	//�K�v�ȃe�[�u���̃T�C�Y���Z�o(�N���X�^��)
	usClusterNum = g_ulMaxFileSize / (1 << g_tFS_Info.ucClustorSizeShift) + 1;
		
	//�m�ۂ���q�[�v�T�C�Y������
	ulHeapSize += (sizeof(unsigned short) * usClusterNum);

	//���łɎ擾����Ă��Ȃ����H
	if(g_tFS_Info.ptaDirTable != NULL)
	{
		//�Ƃ肠����������Ă���
		pceHeapFree(g_tFS_Info.ptaDirTable);
		g_tFS_Info.ptaDirTable = NULL;
		g_tFS_Info.pusaFATchain = NULL;
	}

#if (!defined(__PCEKN__)) && defined(__MMC_WORK_GLOBAL__)	
	//���[�N�G���A���擾
	g_tFS_Info.ptaDirTable = (DIR_ENTRY*)g_ucMMCWorkArea;
#else	
	//�q�[�v���m��
	g_tFS_Info.ptaDirTable = (DIR_ENTRY*)pceHeapAlloc(ulHeapSize);
#endif //(!defined(__PCEKN__)) && defined(__MMC_WORK_GLOBAL__)	

	//�擾�ł������H
	if(g_tFS_Info.ptaDirTable == NULL)
		ucRet = 0;

	//�|�C���^�𕪂���
	
	//FAT�`�F�C���e�[�u���̃��[�N�G���A
	g_tFS_Info.pusaFATchain = (unsigned short*)((unsigned char*)g_tFS_Info.ptaDirTable 
												+ sizeof(DIR_ENTRY) * 16); 
	
	return ucRet;

}
//---------------------------------------------------------------------------
//void mmcByteSend(unsigned char ucData)
//��2004/12/12 Change by N.SAWA
//void mmcByteSend(int data) ��2004/12/12 Move to mmc_fram.c by N.SAWA
//---------------------------------------------------------------------------
//void mmcSendDataCore(unsigned char *paData,int iDataLen) ��2005/02/09 Move to mmc_fram.c by Madoka
//---------------------------------------------------------------------------
//unsigned short mmcSendCommandAndRecvResponse(unsigned char ucCMD,unsigned long ulArg)
// ��2005/02/09 Move to mmc_fram.c by Madoka
//---------------------------------------------------------------------------
//unsigned char mmcByteRecv(void) ��2004/12/13 Move to mmc_fram.c by N.SAWA
//---------------------------------------------------------------------------
//unsigned char  mmcRecvData(unsigned char *paRecvBuff,int iRecvLen) ��2004/12/12 Move to mmc_fram.c by N.SAWA
//---------------------------------------------------------------------------
unsigned char mmcInitSPIMode(void)
{

	//MMC��SPI���[�h������
	//�������Ɏ��s�����ꍇ��0��Ԃ��܂��B����I����1��Ԃ��܂��B

	unsigned char CMD0[6] = { 0x40,0x00,0x00,0x00,0x00,0x95 };	//CMD0�̃f�[�^�t�H�[�}�b�g
	unsigned char CMD1[6] = { 0x41,0x00,0x00,0x00,0x00,0xFF }; 	//CMD1�̃f�[�^�t�H�[�}�b�g
	unsigned char ucRecvBuffR1 = 0xFF;							//R1���X�|���X(8bit)�̎�M�o�b�t�@
	int i;

	//MMC�̃f�[�^�V�[�g(P.99)�L�ڂ�SPI���[�h��������
	//�菇�ɂ����ď��������s���܂�
	//MMC�̃f�[�^�V�[�g�͈ȉ��̏ꏊ�ɂ���܂�
	//http://www.renesas.com/avs/resource/japan/jpn/pdf/flashcard/j603002_mmc.pdf
	//���̑��ɂ��ȉ��̏ꏊ�ɃJ�[�h���̂̃f�[�^�V�[�g�����J����Ă��܂��B
	//�Q�l�ɂ��ĉ������B
	//http://www.renesas.com/avs/resource/japan/jpn/pdf/flashcard/j203658_hb28e016mm2.pdf
	//
	//���Ȃ݂ɍ���̗p����SanDisk��MMC�f�[�^�V�[�g�͂�����(�p��)
	//http://media-server.amazon.com/media/mole/MANUAL000007788.pdf
	
	//�f�o�b�O�p
	//#define INIT_MMC	"MMC_INIT"
	//SendUsbCom(INIT_MMC,strlen(INIT_MMC));
	//mmcWait(USBCOM_WAIT);	

	//(1)CS = High�Ƃ��āA�J�[�h��NonActive�ɂ��܂��B
	//   CS�ɂ�P14�|�[�g��ڑ����Ă���̂ŁAP14��High�ɂ��܂��B
    CARD_DISABLE


	//(2)MMC�C�j�V�����C�Y�p�̃_�~�[�N���b�N��74�N���b�N�ȏ㔭�s���܂��B
	for(i = 0;i < 10;++i)
		mmcByteSend(0xFF);	//8�N���b�N���̃_�~�[���M


	//(3)CS = Low�Ƃ��ăJ�[�h��Active�ɂ��ACMD0(GO_IDLE_STATE)�𑗐M���܂��B

	//CS = Low�Ƃ��ăJ�[�h��Active�ɂ��܂��B
	//CS�ɂ�P14�|�[�g��ڑ����Ă���̂ŁAP14��Low�ɂ��܂��B
    CARD_ENABLE

	//CMD0���M
	mmcSendDataCore(CMD0,sizeof(CMD0));

	
	//(4)���̎��_��SPI���[�h�ɐ؂�ւ��܂��B������R1���X�|���X(8bit)��҂��܂��B
	//   �����Ŏ�M�^�C���A�E�g��0x01�ȊO����M�����ꍇ�̓G���[�Ƃ��܂��B
	if((mmcRecvData(&ucRecvBuffR1,1) == 0) ||
	   (ucRecvBuffR1 != 0x01))
	{
#if 0
		#define MMC_CMD0_ERR	"MMC_CMD0_ERR"
		SendUsbCom(MMC_CMD0_ERR,strlen(MMC_CMD0_ERR));	
#endif
		//�G���[�Ȃ̂Ŗ߂�
		CARD_DISABLE
		return 0;
	}

	//�f�o�b�O�p
	//#define MMC_CMD0_OK	"MMC_CMD0_OK"
	//SendUsbCom(MMC_CMD0_OK,strlen(MMC_CMD0_OK));	
	//mmcWait(USBCOM_WAIT);
	
	//(5)CMD1(SEND_OP_CMD)�𑗐M���AMMC�����R1���X�|���X(8bit)��҂��܂��B
	//   R1���X�|���X��0x01�̏ꍇCMD1���đ��M���AR1���X�|���X��0x00�ɂȂ�
	//   �܂Ń|�[�����O���܂��B
	//   �܂��AR1���X�|���X��0x00 or 0x01�ȊO�̏ꍇ���M�^�C���A�E�g�ɂȂ�
	//   ���ꍇ�̓G���[�Ƃ��܂��B

	//CMD1���M
	mmcSendDataCore(CMD1,sizeof(CMD1));

	//��M�|�[�����O
	for(i = 0;i < MMC_POLLING_TIMEOUT;++i)
	{
	
		//��M�^�C���A�E�g�ɂȂ������H
		if(mmcRecvData(&ucRecvBuffR1,1) == 0)
		{
			//������
			i = MMC_POLLING_TIMEOUT;
			break;
		}

		//��M�l���`�F�b�N
		if(ucRecvBuffR1 == 0x00)
		{
			//�����������Ŕ�����
			break;
		}
		else if(ucRecvBuffR1 == 0x01)
		{
			//�r�W�[
		
			//�f�o�b�O�p
			//#define MMC_CMD1_BUSY	"MMC_CMD1_BUSY"
			//SendUsbCom(MMC_CMD1_BUSY,strlen(MMC_CMD1_BUSY));	
			//mmcWait(USBCOM_WAIT);

			//CMD1�đ�
			mmcSendDataCore(CMD1,sizeof(CMD1));
		}
		else
		{
			//�G���[�Ŕ�����
			i = MMC_POLLING_TIMEOUT;
			break;
		}

	}

	//�G���[���H
	if(i == MMC_POLLING_TIMEOUT)
	{
#if 0
		#define MMC_CMD1_ERR	"MMC_CMD1_ERR"
		SendUsbCom(MMC_CMD1_ERR,strlen(MMC_CMD1_ERR));	
#endif
		//�G���[�Ȃ̂Ŗ߂�
		CARD_DISABLE
		return 0;
	}

	//�f�o�b�O�p
	//#define MMC_CMD1_OK	"MMC_CMD1_OK"
	//SendUsbCom(MMC_CMD1_OK,strlen(MMC_CMD1_OK));	
	//mmcWait(USBCOM_WAIT);
	
	//�p���ς񂾂̂ŁACS = High�Ƃ��āA�J�[�h��NonActive�ɂ��܂��B
	//CS�ɂ�P14�|�[�g��ڑ����Ă���̂ŁAP14��High�ɂ��܂��B
   	CARD_DISABLE

	return 1;

}
//---------------------------------------------------------------------------
unsigned char mmcGetCIDInfo(MMC_CID_INFO *pCIDInfo)
{

	//CID���W�X�^�̓��e���擾
	//�擾�Ɏ��s�����ꍇ��0���A���������ꍇ��1��Ԃ��܂��B
	//�����ɂ�CID���W�X�^���̊i�[��A�h���X���w�肵�Ă��������B
	unsigned char ucaRecvCID[MMC_CID_INFO_SIZE];	//CID����M�p�o�b�t�@
	unsigned char ucRet = 0;

	//�J�[�h��Active��
	CARD_ENABLE

	//CMD10���M����R1���X�|���X����M
	if(mmcSendCommandAndRecvResponse(10,0) == 0x00)
	{
	
		//���X�|���X��ɑ��M�����CID�f�[�^����M
		if(mmcRecvData(ucaRecvCID,MMC_CID_INFO_SIZE) == 1)
		{
	
			//��M�����f�[�^�𕪉����č\���̂ɕۑ�

			//MID(ManufactureID)�̎擾
			pCIDInfo->ucMID = ucaRecvCID[0];
		
			//OID(OEM/Application ID)�̎擾
			pCIDInfo->usOID = (unsigned short)((ucaRecvCID[1] << 8) | ucaRecvCID[2]);

			//PNM(Product name)�̎擾
			pCIDInfo->ucPNM[0] = ucaRecvCID[3];
			pCIDInfo->ucPNM[1] = ucaRecvCID[4];
			pCIDInfo->ucPNM[2] = ucaRecvCID[5];
			pCIDInfo->ucPNM[3] = ucaRecvCID[6];
			pCIDInfo->ucPNM[4] = ucaRecvCID[7];
			pCIDInfo->ucPNM[5] = ucaRecvCID[8];

			//PRV(Product revision)�̎擾
			pCIDInfo->ucPRV = ucaRecvCID[9];

			//PSN(Product serial number)�̎擾
			pCIDInfo->ulPSN = (unsigned long)((ucaRecvCID[10] << 24) |
											  (ucaRecvCID[11] << 16) |
											  (ucaRecvCID[12] <<  8) |
											   ucaRecvCID[13]);
			
			//MDT(Manufacturing date)�̎擾
			pCIDInfo->ucMDT = ucaRecvCID[14];

			//CRC(7-bit CRC checksum)�̎擾
			pCIDInfo->ucCRC = ucaRecvCID[15] >> 1;	//LSB1bit�͊֌W�Ȃ��̂ō폜
	
			//�f�o�b�O�p
			//	SendUsbCom((unsigned char*)pCIDInfo,sizeof(MMC_CID_INFO));	
			//	mmcWait(USBCOM_WAIT);
			
			//��M�ɐ���
			ucRet = 1;

			//CID�f�[�^�̌�ɕt���ė���
            //16bit����CRC���_�~�[�N���b�N��
            //�����Ė�������

            //16�N���b�N���̃_�~�[���M
            mmcByteSend(0xFF);
            mmcByteSend(0xFF);

		}

	}

	//�J�[�h��NonActive��
	CARD_DISABLE

	return ucRet;

}
//---------------------------------------------------------------------------
unsigned char mmcGetCSDInfo(MMC_CSD_INFO *pCSDInfo)
{

	//CSD���W�X�^�̓��e���擾
	//�擾�Ɏ��s�����ꍇ��0���A���������ꍇ��1��Ԃ��܂��B
	//�����ɂ�CSD���W�X�^���̊i�[��A�h���X���w�肵�Ă��������B
	unsigned char ucaRecvCSD[MMC_CSD_INFO_SIZE];	//CSD����M�p�o�b�t�@
	unsigned char ucRet = 0;

	//�J�[�h��Active��
	CARD_ENABLE

	//CMD9���M����R1���X�|���X����M
	if(mmcSendCommandAndRecvResponse(9,0) == 0x00)
	{

		//���X�|���X��ɑ��M�����CSD�f�[�^����M
		if(mmcRecvData(ucaRecvCSD,MMC_CSD_INFO_SIZE) == 1)
		{
	
			//��M�����f�[�^�𕪉����č\���̂ɕۑ�
			//
			//�{����16�o�C�g(128bit)���S���\���̂ŕۑ����Ă����̂�
			//�ǂ��̂��낤�Ǝv�����ǁA����CSD�Ɋւ��Ă͊e��f�[�^��
			//�r�b�g�P�ʂŋ�؂��ĂāA�\���̂Ƀr�b�g�t�B�[���h�Ƃ�
			//�g���ĕۑ����悤�Ƃ��Ă��A���̓��e���m�F���邽�߂�USB
			//��VC�̃A�v���ƒʐM��������P/ECE��GCC��VC�Ńr�b�g�t�B�[
			//���h�̎d�l���Ⴄ�̂��A��肭�l���n���Ȃ������肵�đ��
			//�������̂ŁA����͕K�v�ŏ����̓��e�݂̂�ۑ����邱�Ƃ�
			//���܂��B

			unsigned short C_SIZE;
			unsigned char C_SIZE_MULT;
			unsigned long MULT;
			unsigned long BLOCKNR;
            
			//�ǂݍ��݃u���b�N�����擾
			//SanDisk��64MB�ł�512�ł���
			pCSDInfo->usReadBlockLength = (unsigned short)(1 << (ucaRecvCSD[5] & 0x0F));

			//�������݃u���b�N�����擾
			//SanDisk��64MB�ł�512�ł���
			pCSDInfo->usWriteBlockLength = (unsigned short)(1 << (((ucaRecvCSD[12] & 0x03) << 2) |
																  ((ucaRecvCSD[13] & 0xC0) >> 6)));
		
			//�� MMC�͓ǂݏ����ɑ΂���u���b�N�����f�t�H
			//   ���g��512�o�C�g�Ƃ����̂��W���̂悤�Ȃ�
			//   �ŁA���񐻍삵���f�[�^�̓ǂݏ���API�ł�
			//   �u���b�N��512�o�C�g�Ƃ��ď������s���Ă�
			//   �܂��B
			//   �����A���g����MMC�̃u���b�N����512�o�C
			//   �g�ȊO�̏ꍇ�́ACMD16�Ńu���b�N����512
			//   �o�C�g�ɐݒ肷�邩�AAPI�̒��g��K�؂�
			//   ���̂ɏ��������Ă��������B


			//�J�[�h�e�ʂ��v�Z
			//
			//�J�[�h�e�ʂ�C_SIZE,C_SIZE_MULT�����READ_BLK_LEN
			//����ȉ��̗l�Ɍv�Z����܂��B
			//
			//memory_capacity = BLOCKNR * BLOCK_LEN
			//
			//BLOCKNR   = (C_SIZE+1) * MULT
			//MULT      = 2^(C_SIZE_MULT+2)
			//BLOCK_LEN = 2^READ_BLK_LEN
			//
			//(���̂���BLOCK_LEN�͂��ł�usReadBlockLength�Ƃ���
			// �v�Z�ς݂Ȃ̂ŁA���̒l�𗘗p���܂�)
			//
			
			//CSIZE���擾
			//SanDisk��64MB�ł�3919�ł���
			C_SIZE = (unsigned short)(((ucaRecvCSD[6] & 0x03) << 10) |
									   (ucaRecvCSD[7] << 2) | 
									  ((ucaRecvCSD[8] & 0xC0) >> 6));
	
			//C_SIZE_MULT���擾
			//SanDisk��64MB�ł�3�ł���
			C_SIZE_MULT = (unsigned char)(((ucaRecvCSD[ 9] & 0x03) << 1) | 
										  ((ucaRecvCSD[10] & 0x80) >> 7));

			//MULT���v�Z
			MULT = 1 << (C_SIZE_MULT+2);

			//BLOCKNR�̌v�Z�ƕۑ�
			//SanDisk��64MB�ł�125440�ɂȂ�܂���
			BLOCKNR = (C_SIZE+1) * MULT;
			pCSDInfo->ulTatalBlocks = BLOCKNR;	

			//�J�[�h�e�ʂ��v�Z
			//SanDisk��64MB�ł�64225280�ɂȂ�܂���
			pCSDInfo->ulCardSize = (unsigned long)(BLOCKNR * pCSDInfo->usReadBlockLength);

			//�f�o�b�O�p
			//	SendUsbCom((unsigned char*)pCSDInfo,sizeof(MMC_CSD_INFO));	
			//	mmcWait(USBCOM_WAIT);
				
			//��M�ɐ���
			ucRet = 1;

			//CSD�f�[�^�̌�ɕt���ė���
            //16bit����CRC���_�~�[�N���b�N��
            //�����Ė�������

            //16�N���b�N���̃_�~�[���M
            mmcByteSend(0xFF);
            mmcByteSend(0xFF);
			
		}

	}

	//�J�[�h��NonActive��
	CARD_DISABLE

	return ucRet;

}
//---------------------------------------------------------------------------
unsigned char mmcCheckConnection(void)
{

	//�ڑ��`�F�b�N
	//����ɐڑ�����Ă���ꍇ��0��Ԃ��B
	//�G���[�ɂȂ����ꍇ��1��Ԃ��܂��B
	
	unsigned char ucRet = 0;
	unsigned short usRes;

	//�X�e�[�^�X�₢���킹

	//�J�[�h��Active��
	CARD_ENABLE

	//CMD13���M����R2���X�|���X����M
	usRes = mmcSendCommandAndRecvResponse(13,0);

	//�J�[�h��NonActive��
	CARD_DISABLE

	//���X�|���X�`�F�b�N
	if(usRes != 0)
	{
		//�����������������Ă݂�
		if(mmcInitSPIMode() == 0)
			ucRet = 1;	//�G���[
	}
	
	return ucRet;

}
//---------------------------------------------------------------------------
unsigned char mmcReadSector(unsigned long ulSector,unsigned char *pucBuff)
{

	//�Z�N�^���[�h
	//MMC����1�Z�N�^��(512�o�C�g��z��)�f�[�^��ǂݏo���܂�
	//
	//unsigned long ulSector	�ǂݏo���ΏۃZ�N�^(�����Z�N�^�ԍ�)
	//unsigned char *pBuff		�o�b�t�@(�K��512�o�C�g�ȏ゠�邱��)
	//
	//����ɓǂݏo�����ꍇ��1���A�G���[�����������ꍇ��0��Ԃ��܂�
	unsigned long ulAddress = ulSector * g_tMMCInfo.tCSDInfo.usReadBlockLength;	//�A�h���X
	unsigned char ucRet = 0;

	//�J�[�h��Active��
	CARD_ENABLE

	//CMD17���M����R1���X�|���X����M
	if(mmcSendCommandAndRecvResponse(17,ulAddress) == 0x00)
	{

		//���X�|���X��ɑ��M�����Z�N�^�f�[�^����M
		if(mmcRecvData(pucBuff,g_tMMCInfo.tCSDInfo.usReadBlockLength) == 1)
		{
	
			//�Z�N�^�f�[�^�̍Ō�ɕt���ė���
			//16bit����CRC���_�~�[�N���b�N��
			//�����Ė�������
			
			//16�N���b�N���̃_�~�[���M
			mmcByteSend(0xFF);
			mmcByteSend(0xFF);

			//�f�o�b�O�p
			//	SendUsbCom((unsigned char*)pBuff,g_tMMCInfo.tCSDInfo.usReadBlockLength);	
			//	mmcWait(USBCOM_WAIT);

			//����I��
			ucRet = 1;

		}
		
	}
	
	//�J�[�h��NonActive��
	CARD_DISABLE

	return ucRet;

}
//---------------------------------------------------------------------------
unsigned long mmcGetBootSecterOffset(void)
{

	//�}�X�^�[�u�[�g���R�[�h����u�[�g�Z�N�^�̃I�t�Z�b�g���擾

	unsigned char ucaMBR[512];					//�}�X�^�[�u�[�g���R�[�h�̃f�[�^
	PARTITIONENTRY_AT tPartition[4];			//�p�[�e�B�V�����G���g���e�[�u��
	unsigned long ulOffsetSector = 0xFFFFFFFF;	//MBR����u�[�g�Z�N�^�܂ł̃I�t�Z�b�g�Z�N�^��
	int i;

	//�ȉ��̑����
	//http://hp.vector.co.jp/authors/VA013937/editdisk/tech.html
	//�̏����Q�l�ɂ��Ă��܂��B

	//MBR(Master Boot Record)�̓ǂݍ���
	if(mmcReadSector(0,ucaMBR) == 1)
	{
	
		//FAT����(0xAA55)���`�F�b�N
		if((ucaMBR[510] == 0x55) && (ucaMBR[511] == 0xAA))
		{

			//�p�[�e�B�V�����e�[�u���̓ǂݍ���
			memcpy(tPartition,&ucaMBR[446],sizeof(PARTITIONENTRY_AT) << 2);

			//�A�N�e�B�u�ȃp�[�e�B�V������T��
			for(i = 0;i < 4;++i)
			{
				//��Ԃ��`�F�b�N
				if(tPartition[i].ucStatus == 0x80)
					break;
			}

			//�����������H
			if(i < 4)
			{
				
				//MBR����u�[�g�Z�N�^�܂ł̃I�t�Z�b�g�Z�N�^�����擾
				ulOffsetSector = tPartition[i].ulOffsetSectors;
	
				//�f�o�b�O�p
				//SendUsbCom((unsigned char*)&ulOffsetSector,4);	
				//mmcWait(USBCOM_WAIT);
				
			}	
			else
			{
				//������Ȃ������ꍇ�͈�ԍŏ��̃p�[�e�B�V������
				//�A�N�e�B�u�ƌ��Ȃ��܂��B
				//��SD�J�[�h�p�̑Ώ�
				//(����:nsawa����)
				//
				//�����ASD�J�[�h�֘A�f�o�C�X�̊J���Ń��C�Z���X���ǂ�
				//�����Ƃ�����ꂽ��A���̕������R�����g�������SD
				//�J�[�h�ł͓����Ȃ��Ǝv���܂��B
				//

				//MBR����u�[�g�Z�N�^�܂ł̃I�t�Z�b�g�Z�N�^�����擾
				ulOffsetSector = tPartition[0].ulOffsetSectors;
			}

		}
        	
	}

	return ulOffsetSector;

}
//---------------------------------------------------------------------------
unsigned char mmcGetFileSystemInfo(unsigned long ulBootSecterOffset)
{

	//�u�[�g�Z�N�^����t�@�C���V�X�e�������擾
	//�����ɂ�mmcGetBootSecterOffset�Ŏ擾����MBR����u�[�g�Z�N�^
	//�܂ł̃I�t�Z�b�g����͂��܂��B
	//����ɓǂݏo�����ꍇ��1���A���s�����ꍇ��0��Ԃ��܂��B
	
	unsigned char ucaBoot[512];
	unsigned long ulClustorSize;
	unsigned char ucRet = 0;
    	
	//�ȉ��̑����
	//http://triaez.kaisei.org/~s-zouda/pc/fat32.html
	//���Q�l�ɂ��Ă��܂��B

	//�u�[�g�Z�N�^�̓ǂݍ���
	if(mmcReadSector(ulBootSecterOffset,ucaBoot) == 1)
	{
	
		//FAT����(0xAA55)���`�F�b�N
		if((ucaBoot[510] == 0x55) && (ucaBoot[511] == 0xAA))
		{

			//1�N���X�^������̃Z�N�^�����擾
			g_tFS_Info.ucSectorsParCluster = ucaBoot[0x0D];

			//1�N���X�^������̃Z�N�^������A�v�Z�Ŏg�p�����
			//�̃V�t�g�����Z�o���ĕۑ�
			for(g_tFS_Info.ucSectorsParClusterShift = 0;
				g_tFS_Info.ucSectorsParClusterShift < 16;
				++g_tFS_Info.ucSectorsParClusterShift)
			{
                if((1 << g_tFS_Info.ucSectorsParClusterShift)
				   == g_tFS_Info.ucSectorsParCluster) 
					break;
			}			
			
			//1�N���X�^������̃Z�N�^������A1�N���X�^������̃o�C�g����
			//�v�Z�Ŏg�p����ۂ̃V�t�g�����Z�o���ĕۑ�
			ulClustorSize = (g_tMMCInfo.tCSDInfo.usReadBlockLength *
							 g_tFS_Info.ucSectorsParCluster);
			for(g_tFS_Info.ucClustorSizeShift = 0;
				g_tFS_Info.ucClustorSizeShift < 16;
				++g_tFS_Info.ucClustorSizeShift)
			{
                if((1 << g_tFS_Info.ucClustorSizeShift) == ulClustorSize) 
					break;
			}
			
			//�_���Z�N�^0(������u�[�g�Z�N�^)����FAT�̈�܂ł̃Z�N�^�����擾
			memcpy(&g_tFS_Info.usFATOffsetSecters,&ucaBoot[0x0E],2);

			//���[�g�f�B���N�g���G���g���̍ő吔���擾
			memcpy(&g_tFS_Info.usRootDirEntryMax,&ucaBoot[0x11],2);

			//FAT�̃Z�N�^�����擾
			memcpy(&g_tFS_Info.usFATSize,&ucaBoot[0x16],2);

			//�{�����[�����x���V���A��ID���擾
			memcpy(&g_tFS_Info.ulVolumeSerialID,&ucaBoot[0x27],4);

			//FAT�̈ʒu���Z�o
			g_tFS_Info.ulFATAddress
		     = ulBootSecterOffset + g_tFS_Info.usFATOffsetSecters;

			//���[�g�f�B���N�g���̈ʒu���Z�o
			g_tFS_Info.ulRootDirAddress
			 = g_tFS_Info.ulFATAddress + 2 * g_tFS_Info.usFATSize;

			//�擪�N���X�^(�N���X�^�ԍ�2)�̈ʒu���Z�o
			g_tFS_Info.ulFirstClusterAddress 
			 = g_tFS_Info.ulRootDirAddress 
			   + (g_tFS_Info.usRootDirEntryMax >> 4);	// = g_tFS_Info.usRootDirEntryMax * 32 / 512;
														// = (g_tFS_Info.usRootDirEntryMax << 5) >> 9 	
		
			//�ǂݍ��ݏI��
			ucRet = 1;

		}
        	
	}	

	return ucRet;

}
//---------------------------------------------------------------------------
static int mmcCheck_fnameForDirEntry( const char *p )
{

	//�f�B���N�g���G���g���p�t�@�C�����`�F�b�N�֐�
	//P/ECE�J�[�l���\�[�X"file.c"��check_fname
	//���Q�l�ɂ��܂����B
	//
	//P/ECE�͉p�������Ɛ������������܂��񂪁A
	//FAT16�̃f�B���N�g���G���g���ɂ͑啶����
	//�o�^�����̂ŁA�����ł͑啶��OK�Ƃ���
	//�`�F�b�N���܂��B
	//
	//�܂�8�����ɖ����Ȃ��t�@�C�����ł́A��
	//�Ŗ��߂���̂ŁA�f�B���N�g���G���g��
	//�ł̃t�@�C�����ł̂݋󔒂������܂��B
	//
	//�f�B���N�g���G���g���̃t�@�C������8����
	//�Œ�Ȃ̂ŁA8�����ȏ�̓`�F�b�N���܂���B
    //
	//�܂��A1�����ł��������̍��������t�@�C������
	//�����O�t�@�C�����Ƃ��Ĉ�����̂ŁA�����ł�
	//�����ł��܂���B
	//�����Ń`�F�b�N�ł���̂͑S���啶�����S��������
	//�̃t�@�C������8.3�`���̂��̂����ł��B���ӁB
	//
	int i;
	char a;

	for(i = 0;i < 8;++i)
	{
		//1�����擾
		a = *p++;
		
		//P/ECE�ŋ�����镶�����H
		if(!(// ���t�@�C�����ɋ�����镶���Z�b�g+�� [0-9A-Z_ ]
			 ( a >= '0' && a <= '9' ) ||
			 ( a >= 'A' && a <= 'Z' ) ||
			 ( a == '_' ) ||
			 ( a == ' ' ) 
			 //{{2004/11/15 Add by N.SAWA
			 //  MS-DOS�̃V���[�g�t�@�C�����Ƃ��ċ�����镶����ǉ����܂����B(�J�i�A����������)
			 //  MS-DOS�̃V���[�g�t�@�C�����Ƃ��ċ�����镶���́A���̂Ƃ���ł��B
			 //    A�`Z 0�`9 $ & # { } ~ % ' ( ) - @ ^ ` ! _ �J�i ����
			 //  �y�Q�l�z�A�X�L�[�o�ŋǁw�W��MS-DOS�n���h�u�b�N�x��1�� p.25 Table2.1 �t�@�C����
			              ||
			 ( a == '$' ) ||
			 ( a == '&' ) ||
			 ( a == '#' ) ||
			 ( a == '{' ) || ( a == '}' ) ||
			 ( a == '~' ) ||
			 ( a == '%' ) ||
			 ( a == '\'' ) ||
			 ( a == '(' ) || ( a == ')' ) ||
			 ( a == '-' ) ||
			 ( a == '@' ) ||
			 ( a == '^' ) ||
			 ( a == '`' ) ||
			 ( a == '!' )
			 //}}2004/11/15 Add by N.SAWA
		    )
		   ) break;	//������Ă��Ȃ��������Ɣ�����
	}

	//8�����ȓ��ɋ�����Ȃ����������������H
	if(i < 8) return 2;

	//����ȊO�͐���ȃt�@�C�����Ƃ���
	return 0;

}
//---------------------------------------------------------------------------
DIR_ENTRY* mmcSearchCore(MMC_FILEFIND_INFO* pInfo)
{

	//�f�B���N�g���G���g�������̃R�A
	DIR_ENTRY *pDir = NULL;
	unsigned char ucFirst;

	//�f�B���N�g���G���g�����擾���郏�[�N�G���A�͊m�ۂ���Ă��邩�H
	if(g_tFS_Info.ptaDirTable == NULL)
		return NULL;	//���s
	
	//�L���ȃf�B���N�g���G���g����������܂Ń��[�v
	while(1)
	{
		//�Ō�܂Ō����������H
		if(pInfo->usDirIndex >= g_tFS_Info.usRootDirEntryMax)
			break;

		//�f�o�b�O�p
		//SendUsbCom("�f�B���N�g���G���g���`�F�b�N  �J�n",34);	
		//mmcWait(USBCOM_WAIT);

		//�V���Ƀf�B���N�g���G���g����ǂݍ��ޕK�v�����邩�H
		if((pInfo->usDirIndex % 16) == 0)
		{

			//�f�o�b�O�p
			//SendUsbCom("�f�B���N�g���G���g���ǂݍ���",28);	
			//mmcWait(USBCOM_WAIT);

			//���̃Z�N�^��ǂݍ���
			if(mmcReadSector(g_tFS_Info.ulRootDirAddress + (pInfo->usDirIndex >> 4),	// >> 4 = / 16
							 (unsigned char*)g_tFS_Info.ptaDirTable) == 0)
				break;		//�Z�N�^�̓ǂݍ��݂Ɏ��s			
		}
	
		//�f�B���N�g���G���g���̃|�C���^���擾
		pDir = &g_tFS_Info.ptaDirTable[pInfo->usDirIndex % 16];

		//�f�o�b�O�p
		//SendUsbCom((unsigned char *)pDir,sizeof(DIR_ENTRY));	
		//mmcWait(USBCOM_WAIT);
		
		//�t�@�C������1�����ڂ��擾
		ucFirst = (unsigned char)pDir->sFileName[0];

		//���̃f�B���N�g���G���g���֐i�߂Ă���
		pInfo->usDirIndex++;

		//�L���ȃf�B���N�g���G���g��(�t�@�C����\��)���H
		if((ucFirst != 0x00) && (ucFirst != 0xE5) && 
		   (ucFirst != 0x2E) && (ucFirst != 0x05))
		{

			//�t�@�C���T�C�Y��������ő�T�C�Y�𒴂��Ă��Ȃ����H
			//�܂��̓t�@�C������P/ECE�ŗ��p�ł�����̂��H
            if((pDir->ulFileSize > g_ulMaxFileSize) ||		//2004/04/24 Changed by Madoka >= �� >
			   //{{2004/11/15 Add by N.SAWA
			   //  �f�B���N�g���E�G���g���܂��̓{�����[�����x���̑����������Ă��Ȃ����H
			   //  VFAT�̃����O�t�@�C�����X���b�g�́A�Ǐo����p�t�@�C���A�s�������A�V�X�e���t�@�C���A�{�����[�����x����
			   //  �����������Ă���̂ŁA�{�����[�����x���̑��������G���g�������O���邱�Ƃɂ���ăX�L�b�v�ł��܂��B
			   //  �y�Q�l�z�wJF: Linux Kernel 2.4 Documentation: vfat.txt�x
			   //          http://www.linux.or.jp/JF/JFdocs/kernel-docs-2.4/filesystems/vfat.txt.html
			   (pDir->ucAttribute & (0x10 | 0x08)) ||
			   //}}2004/11/15 Add by N.SAWA
			   (mmcCheck_fnameForDirEntry(pDir->sFileName)))
			{
			
#if 0	//���͎g��Ȃ��̂Ŗ���������

				//�f�o�b�O�p
				//SendUsbCom("�t�@�C���G���[",14);	
				//mmcWait(USBCOM_WAIT);

				if(pDir->ulFileSize >= g_ulMaxFileSize)
				{
					//�f�o�b�O�p
					//SendUsbCom("�t�@�C���T�C�Y���傫��",22);	
					//mmcWait(USBCOM_WAIT);

					//SendUsbCom((unsigned char *)&pDir->ulFileSize,4);	
					//mmcWait(USBCOM_WAIT);
				}

				if(mmcCheck_fnameForDirEntry(pDir->sFileName))
				{
					//�f�o�b�O�p
					//SendUsbCom("�t�@�C��������",14);	
					//mmcWait(USBCOM_WAIT);

                    //{
					//	char sss;
					//	sss = '0' + (unsigned char)mmcCheck_fnameForDirEntry(pDir->sFileName);
					//	SendUsbCom(&sss,1);	
					//	mmcWait(USBCOM_WAIT);
					//}

					//SendUsbCom((unsigned char *)pDir->sFileName,sizeof(pDir->sFileName));	
					//mmcWait(USBCOM_WAIT);				
				}

#endif	//���͎g��Ȃ��̂Ŗ���������

				//�f�B���N�g���G���g���̃|�C���^�𖳌���
				pDir = NULL;

				//���ɐi�߂�
				continue;

			}
			
			//���������̂Ń��[�v�𔲂���
			break;
 
		}
		
		//�f�B���N�g���G���g���̃|�C���^��������
		pDir = NULL;

		//�f�o�b�O�p
		//SendUsbCom("���߂ł���",10);	
		//mmcWait(USBCOM_WAIT);

	}
	
	return pDir;

}
//---------------------------------------------------------------------------
void mmcCopyFatFileNameToPieceFileName(char *pPieceFile,char *pFatFile,char *pFatExt)
{

	//FAT�f�B���N�g���G���g���̃t�@�C��������P/ECE�̃t�@�C�����ɃR�s�[

	int i;
	
	//�܂��̓t�@�C�������R�s�[
	//P/ECE�̓t�@�C�����ɋ󔒂��g���Ȃ��̂ŁA
	//�󔒂��o�Ă������_�ŏI���ɂ���
	for(i = 0;i < 8;++i)
	{
		//�󔒂��H
		if(*pFatFile == ' ')
			break;
		
		//1�����R�s�[
		*pPieceFile = *pFatFile;

		//�������ϊ�
		if((*pPieceFile >= 'A') && (*pPieceFile <= 'Z'))
			*pPieceFile += 0x20;

		//���̕�����
		pPieceFile++;
		pFatFile++;
	}
	
	//'.'��ǉ�
	*pPieceFile = '.';
	pPieceFile++;
		
	//�g���q��ǉ�
	for(i = 0;i < 3;++i)
	{
		//�󔒂��H
		if(*pFatExt == ' ')
			break;
		
		//1�����R�s�[
		*pPieceFile = *pFatExt;

		//�������ϊ�
		if((*pPieceFile >= 'A') && (*pPieceFile <= 'Z'))
			*pPieceFile += 0x20;

		//���̕�����
		pPieceFile++;
		pFatExt++;
	}
	
	//�Ō�ɂ͏I�[�������Z�b�g
	*pPieceFile = '\0';

}
//---------------------------------------------------------------------------
int mmcFileFindOpen(FILEINFO *pfi)
{

	//MMC��pceFileFindOpen
	//����ɏI�������ꍇ��0��Ԃ��܂��B
	//MMC���ڑ�����Ă��Ȃ����̃G���[�����������ꍇ��1��Ԃ��܂��B

	MMC_FILEFIND_INFO *pWork;
    
	//MMC�̐ڑ��m�F
	if(mmcCheckConnection() == 1)
		return 1;	//MMC�ڑ��G���[
 	
	//�t�@�C�������̏��������܂�

	//������f�B���N�g���G���g������������
	pWork = (MMC_FILEFIND_INFO*)pfi->works;

	pWork->usDirIndex = 0;
	
	return 0;

}
//---------------------------------------------------------------------------
int mmcFileFindNext(FILEINFO *pfi)
{

	//MMC��pceFileFindNext
	//�߂�l	0:�t�@�C����������Ȃ�(pfi�̓��e�͖���)
	//		1:�t�@�C������������(pfi�̓��e�͗L��)
	
	DIR_ENTRY *pDir;

	//MMC�̐ڑ��m�F
	//if(mmcCheckConnection() == 1)
	//	return 0;	//MMC�ڑ��G���[
 	//��2005/03/20 Delete by Madoka �������̂��ߐڑ��m�F�͏Ȃ��܂����B
 	
	//�f�B���N�g���G���g���̌���
	pDir = mmcSearchCore((MMC_FILEFIND_INFO*)pfi->works);

	//�L���ȃf�B���N�g���G���g���������������H
	if(pDir == NULL) return 0;	//������܂���ł���
	
	//���������f�B���N�g���G���g������K�v�ȏ����擾
	mmcCopyFatFileNameToPieceFileName(pfi->filename,
								      pDir->sFileName,pDir->sExtension);
	pfi->length = pDir->ulFileSize;
	//pfi->attr = pDir->ucAttribute;	//���͗\�񂳂�Ă���̂�
	pfi->attr   = 0;					//0�����Ă����܂�
	pfi->adrs   = 0;
		
	return 1;

}
//---------------------------------------------------------------------------
int mmcFileFindClose(FILEINFO *pfi)
{

	//MMC��pceFileFindClose
	//�߂�l�͏��0�ł�(����I��)

	//P/ECE�łł��܂��������Ă��Ȃ��悤�Ȃ̂ŁA
	//MMC�ł����������ɕԂ��܂�
	
	return 0;

}
//---------------------------------------------------------------------------
DIR_ENTRY* mmcSearchFile(const char *fname)
{

	//�t�@�C������
	DIR_ENTRY *pDir = NULL;
	FILEINFO tFileInfo;
	MMC_FILEFIND_INFO *pWork = (MMC_FILEFIND_INFO*)tFileInfo.works;
	
	//�f�B���N�g���G���g���̍ŏ����猟��
	pWork->usDirIndex = 0;

	while(1)
	{

		//�L���ȃf�B���N�g���G���g�����擾
		pDir = mmcSearchCore(pWork);
		
		//������Ȃ������̂ł����
		if(pDir == NULL) break;

		//�t�@�C������P/ECE�p�ɕϊ�
		mmcCopyFatFileNameToPieceFileName(tFileInfo.filename,
										  pDir->sFileName,pDir->sExtension);
	
		//�t�@�C������r
		if(strcmp(tFileInfo.filename,fname) == 0) break;
	
	}

	return pDir;
}
//---------------------------------------------------------------------------
int mmcFileOpen(FILEACC *pfa,const char *fname,int mode)
{

	//MMC��pceFileOpen
	//����I���̏ꍇ0��Ԃ��܂��B
	//�t�@�C����������Ȃ��ꍇ��1��Ԃ��܂��B
	//�܂��t�@�C���̃T�C�Y���傫������ꍇ��2��Ԃ��܂��B
	//�t�@�C���̃f�[�^������N���X�^�Ɍ��ד������������ꍇ��3��Ԃ��܂��B
	//FAT�`�F�C���e�[�u�����쐬����̂ɕK�v�ȃq�[�v�̈悪����Ȃ��ꍇ��4��
	//�Ԃ��܂��B
	//���̑��̃G���[��5��Ԃ��܂��B

	DIR_ENTRY *pDir = NULL;

	//MMC�̐ڑ��m�F
	if(mmcCheckConnection() == 1)
		return 5;	//MMC�ڑ��G���[
 	
	//�t�@�C�����Ō������A�f�B���N�g���G���g�������擾
	pDir = mmcSearchFile(fname);
	
	//�ꉞ�`�F�b�N
	if(pfa == NULL) return 5;

	//������
	pfa->valid = 0;

	if(pDir == NULL) return 1;	// no file

	//�t�@�C���T�C�Y�͑傫�����Ȃ����H
	if(pDir->ulFileSize > g_ulMaxFileSize) return 2;	//�T�C�Y�I�[�o�[

	//FILEACC�\���̂ɒl���Ƃ肠�����Z�b�g
	pfa->fsize = pDir->ulFileSize;		//�t�@�C���̃T�C�Y
	pfa->chain = 0;						//MMC�łł͎g��Ȃ��̂�0
	pfa->valid = VALIDFILEACC;			//�Ƃ肠�����������Ă���

	
	//FAT���Q�Ƃ���FAT�`�F�C���e�[�u�����쐬���܂��B
	//������1�����Ȃ�FAT�`�F�C���e�[�u���ɒl���Z�b�g����̂ŁA
	//mmcFileOpen�͕����t�@�C���𓯎��ɊJ���܂���B���ӁB
	switch(mmcCreateFATChainTable(pDir->usCluster,pDir->ulFileSize))
	{
		case 0:	//����I��
			break;

		case 1:	//FAT�`�F�C���e�[�u���̍쐬�Ɏ��s(�q�[�v�̈悪����Ȃ�)
			return 4;

		case 2:	//�N���X�^�Ɍ��ד�����������
			return 3;	
	}

	//����I��
	return 0;

}
//---------------------------------------------------------------------------
int mmcCreateFATChainTable(const unsigned short usFirstCluster,
						   const unsigned long ulFileSize)
{

	//FAT�`�F�C���e�[�u���̍쐬
	//�w��̃t�@�C���̐擪�N���X�^���w�肵�܂��B
	//�K�v�ȃT�C�Y��FAT�`�F�C���e�[�u�����쐬�ł��Ȃ������ꍇ��1��Ԃ��܂��B
	//FAT�̃`�F�C���̓r���Ō��׃N���X�^�������������ꍇ��2��Ԃ��܂��B
	//����I���̏ꍇ��0��Ԃ��܂��B

	unsigned short usNextCluster = usFirstCluster;	
	unsigned char  ucFATBuff[512];
	unsigned long  ulFATSecter;
	unsigned long  ulLastFATSecter = 0;
	int iFATOffset;
	int iChainIndex = 0;
	int iRet = 0;

	//�ŏ��̃N���X�^�ԍ���ۑ�
	g_tFS_Info.pusaFATchain[iChainIndex] = usNextCluster;
	iChainIndex++;

	//�N���X�^�`�F�C�����I��܂Ń��[�v
	while(usNextCluster != 0xFFFF)
	{
		
		//�Y���N���X�^�ɑΉ�����FAT��񂪂��镨���Z�N�^�ԍ����Z�o
		//MMC�Ŏg����FAT��FAT16�ŁAFAT�ɂ̓t�@�C���̃f�[�^��
		//�����Ă���擪�N���X�^�ɑ����N���X�^�ԍ����e�N���X�^��
		//�Ή�����FAT�ɏ�����Ă���B1FAT������̏��ʂ�2�o�C�g�B
		//
		//����āA�~���������Z�N�^�ԍ���
		// FAT�擪�̕����Z�N�^�ԍ� +
		// (usNextCluster * 2 / 512) 
		// 
		ulFATSecter = g_tFS_Info.ulFATAddress 
					   + (usNextCluster >> 8);	// (a * 2 / 512) = ((a << 1) >> 9)
												// ((a << 1) >> 9) = a >> 8 	
														
		//�O��ǂݍ��񂾔͈͂ƈႤ�Ƃ���ɂ��邩�H
		if(ulLastFATSecter != ulFATSecter)
		{
			//�Y���N���X�^�t�߂�FAT����1�Z�N�^���ǂݍ���
			mmcReadSector(ulFATSecter,ucFATBuff);

			//����ǂݍ��񂾕����Z�N�^�ԍ���ۑ�
			ulLastFATSecter = ulFATSecter;
		}

		//�擾����FAT����512bytes / 2bytes = 256�N���X�^�P�ʂȂ̂ŁA
		//�Y���N���X�^�ɑΉ�����FAT�ւ̃I�t�Z�b�g���Z�o
		iFATOffset = (int)(usNextCluster % 256);

		//�Y���N���X�^�ɑΉ�����FAT�����擾
		usNextCluster = *((unsigned short*)ucFATBuff + iFATOffset);

		//���̃N���X�^������͈͓����H
		if(((usNextCluster < 0x0002) || (usNextCluster > 0xFFF6)) &&
		   (usNextCluster != 0xFFFF))	
		{
			//���׃N���X�^����������܂���
			iRet = 1;

			//�`�F�b�N�I��
			break;
		}

		//�N���X�^�`�F�C����ۑ�
		g_tFS_Info.pusaFATchain[iChainIndex++] = usNextCluster;

	}

	return iRet;

}
//---------------------------------------------------------------------------
int mmcFileReadSct(FILEACC *pfa,void *ptr,int sct,int len)
{

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
	
	int iReadBytes = 0;
	unsigned char *pReadBuff;				//�������ݐ�o�b�t�@�ւ̃|�C���^
#ifndef __PCEKN__		// 2005/04/02 Add by Madoka
	//��2004/12/12 Naoyuki Sawa �X�^�b�N���}���̂��߁ASTATIC�ɕύX
	static unsigned char ucaBuff[512];		//�Z�N�^�ǂݍ��ݗp���[�N	
#else
	unsigned char ucaBuff[512];			//�Z�N�^�ǂݍ��ݗp���[�N
	//��2004/12/12 Naoyuki Sawa �X�^�b�N���}���̂��߁ASTATIC�ɕύX
	//static unsigned char ucaBuff[512];		//�Z�N�^�ǂݍ��ݗp���[�N
	//2005/03/20 �ǋL by �܂ǂ�
	//512�o�C�g��static�ϐ��͌��݂̃J�[�l���̈�ɂ͓���Ȃ����߁A
	//static����������܂����B�\�[�X�擪�̃R�����g�ʂ�A����RAM��
	//�ɃX�^�b�N��؂�ւ��ė��p����ꍇ�́AmmcFileReadSct�Ăяo��
	//�ʒu�ɒ��ӂ��Ă��������B
#endif //__PCEKN__
	unsigned long ulReadSector;				//�ǂݍ��ݕ����Z�N�^�ԍ�
	//��4�� �O���[�o���ϐ��̒l�����[�J���Ɉڂ��ׂɒ�` 2005/03/12 Add by Madoka
	unsigned long ulFirstClusterAddress;	//�N���X�^2�̕����Z�N�^�ԍ�
	unsigned char ucSectorsParCluster;		//1�N���X�^������̃Z�N�^��
	unsigned char ucSectorsParClusterShift;	//1�N���X�^������̃Z�N�^���ɂȂ鍶�V�t�g��
	unsigned short *pusaFATchain;			//FAT�`�F�C���e�[�u���̃|�C���^
	int iFATChainIndex;
	int iReadedSector = 0;					//512MB�ȏ�̃J�[�h�p�̃_�~�[�ǂݍ��ݍς݃Z�N�^�� 2005/03/16 Add by Madoka
	int i;

	////MMC�̐ڑ��m�F
	//if(mmcCheckConnection() == 1)
	//	return 0;	//MMC�ڑ��G���[
 	//��2004/12/12 Delete by N.SAWA �������̂��߁A�ǂݍ��ݎ��̐ڑ��m�F�͏Ȃ��܂����B
 	
	//�t�@�C�����̗L�����`�F�b�N
	if(pfa == NULL) return 0;	//�G���[

	////�Z�N�^�ԍ��̗L�������`�F�b�N
	////�w��Z�N�^�ԍ��̈ʒu���t�@�C�����ɂ��邩�ǂ����𒲂ׂ�
	//if(sct > (((pfa->fsize + 4095) >> 12) - 1)) return 0;	//�Z�N�^�ԍ��G���[ ( >> 12 = / 4096)
	//��2004/12/12 Delete by N.SAWA �ȉ��̐؂�l�ߏ����Ŕ���ł���̂ŁA�����s�v�ł��B

	//{{2004/12/12 Add by N.SAWA
	//�ǂݍ��ރo�C�g�����t�@�C���I�[�𒴂��Ă�����A�t�@�C���I�[�܂łɐ؂�l�߂܂��B
	//pceFileReadSct()�݊��̐U�镑���Ƃ��邽�߂ɂ́A���̏������K�{�ł��B
	//if(len > pfa->fsize - sct * 4096)
	if(len > pfa->fsize - (sct << 12))	//2005/02/08 Change by Madoka	�����ł������������邽�߂ɃV�t�g�ɕύX
	{
		//len = pfa->fsize - sct * 4096;
		len = pfa->fsize - (sct << 12);	//2005/02/08 Change by Madoka	�����ł������������邽�߂ɃV�t�g�ɕύX
	}
	//��2005/04/05 Changed by Madoka 
	//ptr=NULL,len=0�̏ꍇ��������悤�ɏC��
	if((len <= 0) && (ptr != NULL)) return 0; //0�o�C�g�ǂݍ��ݗv���A�܂��̓Z�N�^�ԍ����I�[�𒴂��Ă���
	//}}2004/12/12 Add by N.SAWA
	
	//�������ݐ�o�b�t�@�̃|�C���^��ݒ�

	//�ۑ��p�o�b�t�@�̃|�C���^��NULL����Ȃ����H
	if(ptr != NULL)
	{
		//���̂܂܎g��
		pReadBuff = ptr;
	}
	else
	{
		//�g���@�\���ꉞ�T�|�[�g

		//��փo�b�t�@���m�F
		//if(g_tFS_Info.pucaReadSubBuff == NULL)
		//{
		//	//�m�ۂ���Ă��Ȃ��̂Ŋm�ۂ����݂�
		//	g_tFS_Info.pucaReadSubBuff = (unsigned char*)pceHeapAlloc(4096);
		//	
		//	//�m�ۂł������H
		//	if(g_tFS_Info.pucaReadSubBuff == NULL)
		//	{
		//		//����API�͎��s���܂����B
		//		pfa->aptr = NULL;
		//		return 0;
		//	}
		//}
		//��2004/12/12 Delete by N.SAWA ��փo�b�t�@�͏��������Ɋm�ۍς݂ł�

		//��փo�b�t�@�̃|�C���^���Z�b�g
		pReadBuff = g_tFS_Info.pucaReadSubBuff;
		pfa->aptr = pReadBuff;

		//2004/08/29 Add by Madoka
		if(len == 0) len = PIECE_SECTOR_SIZE;	//�T�C�Y�w�肪�����ꍇ�͍ő�T�C�Y�ɂ���
	}
	
	//�Y������ꏊ��FAT�`�F�C���e�[�u���C���f�b�N�X���Z�o
	//
	//�~���������Z�N�^�ԍ��́AFAT�`�F�C���e�[�u���̃N���X�^�ԍ�����
	//�Z�o�ł��܂��B
	//
	//�܂��A���ɓǂݍ���P/ECE�ł̃Z�N�^�ԍ�����ǂݍ��ݍς݂̃o�C�g
	//�����Z�o���A�����1�N���X�^������̃o�C�g���Ŋ���ƁAFAT�`�F�C��
	//�e�[�u���̃C���f�b�N�X���킩��̂ŁA���̃C���f�b�N�X�̈ʒu��
	//�����ꂽ�N���X�^�ԍ����畨���Z�N�^�ԍ����Z�o���܂��B
	//
	//
	//�� << 12 = * 4096
	//�� ���V�t�g�l����g_tFS_Info.ucClustorSizeShift��
	//�� �����Ă���̂�g_tFS_Info.ucClustorSizeShift��
	//�� �E�ɃV�t�g����K�v�����邩��ł��B
	//�� ���Ȃ݂� >> g_tFS_Info.ucClustorSizeShift = / sizeof(Clustor) 
	//�� (���������̕��@��1�N���X�^�̃T�C�Y��4096bytes�ȉ��̏ꍇ�Ɍ���܂��B���ӁB)
	//Old::
	//iFATChainIndex = sct << (12 - g_tFS_Info.ucClustorSizeShift);
    //2005/03/20 Changed by Madoka 512MB�ȏ�1GB�ȓ��̃������J�[�h�ɑΉ�
	if(g_tFS_Info.ucClustorSizeShift > 12)
	{
		int iShift = (g_tFS_Info.ucClustorSizeShift - 12);

		iFATChainIndex = sct >> iShift;
		iReadedSector  = (sct & ((1 << iShift) - 1)) << 3;

		//���̌v�Z�́A1�N���X�^��4096�o�C�g�ȏ�̃J�[�h�̏ꍇ�ɁA
		//�N���X�^���ł̃I�t�Z�b�g�Z�N�^�����o�����߂̂��̂ŁA
		//sct���E�V�t�g����Ė����Ȃ镔���̒l��4096 / 512 = 8
		//�������āA512�o�C�g�̃Z�N�^�������I�t�Z�b�g�ƂȂ邩
		//���Z�o���Ă��܂��B
	}
	else
		iFATChainIndex = sct << (12 - g_tFS_Info.ucClustorSizeShift);
    		
	//�ǂݍ��݃��[�v
	ulFirstClusterAddress    = g_tFS_Info.ulFirstClusterAddress;
	ucSectorsParCluster      = g_tFS_Info.ucSectorsParCluster;
	ucSectorsParClusterShift = g_tFS_Info.ucSectorsParClusterShift;
	pusaFATchain             = &g_tFS_Info.pusaFATchain[iFATChainIndex];

	//(�w��o�C�g�ǂݍ��ނ܂Ń��[�v)
	while(iReadBytes < len)
	{
		
		//�ǂݍ��ݕ����Z�N�^�ԍ����Z�o
		ulReadSector = ulFirstClusterAddress			//�N���X�^2�̕����Z�N�^�ԍ�
						+ ((*pusaFATchain - 2)			//�I�t�Z�b�g�Z�N�^��
						   << ucSectorsParClusterShift);					
		ulReadSector += iReadedSector;					//�_�~�[�ǂݍ��ݍς݃Z�N�^�������Z(512MB�ȏ�̃J�[�h�p)
														//��2005/03/20 Add by Madoka

		//1�N���X�^���ǂݍ���
		for(i = iReadedSector;i < ucSectorsParCluster;++i)
		{
            		
			//1�Z�N�^���ǂݍ���
			mmcReadSector(ulReadSector,ucaBuff);

			//���ǂݍ��݃f�[�^��1�Z�N�^�ȏ゠�邩�H
			if((len - iReadBytes) > 512)
			{
				//�S���R�s�[
				memcpy(pReadBuff,(void*)ucaBuff,512);
				
				//1�Z�N�^���|�C���^��i�߂�
				pReadBuff  += 512;
				iReadBytes += 512;

				//���̃Z�N�^��
				ulReadSector++;
			}
			else
			{
				//�c�蕪�����R�s�[
				memcpy(pReadBuff,(void*)ucaBuff,len - iReadBytes);
				
				//�S���R�s�[���܂���
				iReadBytes = len;
				break;
			}
			
		}

		//���̃`�F�C����
		pusaFATchain++;

	}

	return iReadBytes;

}
//---------------------------------------------------------------------------
int mmcFileReadMMCSct(FILEACC *pfa,void *ptr,int sct,int len)
{

	//MMC��pceFileReadSct�k����
	//MMC����MMC�ł�1�Z�N�^(512bytes)����ǂݍ��݂܂��B
	//sct�ɂ̓t�@�C���̐擪�����MMC�ł̃Z�N�^�����w�肵�ĉ������B
	//�߂�l�͓ǂݍ��񂾃o�C�g���ł��B
	//�N���X�^�ԍ����K�؂ł͂Ȃ��ꍇ��Apfar��NULL�̏ꍇ�A�܂�MMC���ڑ�
	//����Ă��Ȃ��ꍇ�͎��s�Ƃ���0��Ԃ��܂��B
	//
	//P/ECE�ł̊g���@�\�ł���ptr=NULL�ł̌Ăяo���͈ꉞ�T�|�[�g
	//���Ă��܂����A���Ǒ�փo�b�t�@�ɓǂݍ���ł���̂ŁA�����͈����ł��B
	//�܂���֕��o�b�t�@��512�o�C�g�𒴂���A�N�Z�X�͕ۏ؂���܂���B

	unsigned char *pReadBuff;		//�������ݐ�o�b�t�@�ւ̃|�C���^
	unsigned long ulReadSector;		//�ǂݍ��ݕ����Z�N�^�ԍ�
	int iFATChainIndex;

	////MMC�̐ڑ��m�F
	//if(mmcCheckConnection() == 1)
	//	return 0;	//MMC�ڑ��G���[
 	//��2004/12/12 Delete by N.SAWA �������̂��߁A�ǂݍ��ݎ��̐ڑ��m�F�͏Ȃ��܂����B
 	
	//�t�@�C�����̗L�����`�F�b�N
	if(pfa == NULL) return 0;	//�G���[

	//�Z�N�^�ԍ��̗L�������`�F�b�N
	//�w��Z�N�^�ԍ��̈ʒu���t�@�C�����ɂ��邩�ǂ����𒲂ׂ�
	//if(sct > (((pfa->fsize + 511) >> 9) - 1)) return 0;	//�Z�N�^�ԍ��G���[ ( >> 9 = / 512)
	//��2005/04/05 Deleted by Madoka �ȉ��̐؂�l�ߏ����Ŕ���ł���̂ŁA�����s�v�ł��B

	//{{2005/04/05 Add by Madoka
	//�ǂݍ��ރo�C�g�����t�@�C���I�[�𒴂��Ă�����A�t�@�C���I�[�܂łɐ؂�l�߂܂��B
	//pceFileReadSct()�݊��̐U�镑���Ƃ��邽�߂ɂ́A���̏������K�{�ł��B
	if(len > pfa->fsize - (sct << 9))
	{
		len = pfa->fsize - (sct << 9);
	}
	if((len <= 0) && (ptr != NULL)) return 0; //0�o�C�g�ǂݍ��ݗv���A�܂��̓Z�N�^�ԍ����I�[�𒴂��Ă���
	//}}2005/04/05 Add by Madoka



	//�������ݐ�o�b�t�@�̃|�C���^��ݒ�

	//�ۑ��p�o�b�t�@�̃|�C���^��NULL����Ȃ����H
	if(ptr != NULL)
	{
		//���̂܂܎g��
		pReadBuff = ptr;
	}
	else
	{
		//�g���@�\���ꉞ�T�|�[�g
		
		////��փo�b�t�@���m�F
		//if(g_tFS_Info.pucaReadSubBuff == NULL)
		//{
		//	//�m�ۂ���Ă��Ȃ��̂Ŋm�ۂ����݂�
		//	g_tFS_Info.pucaReadSubBuff = (unsigned char*)pceHeapAlloc(512);
		//
		//	//�m�ۂł������H
		//	if(g_tFS_Info.pucaReadSubBuff == NULL)
		//	{
		//		//����API�͎��s���܂����B
		//		pfa->aptr = NULL;
		//		return 0;
		//	}
		//}
		//��2004/12/12 Delete by N.SAWA ��փo�b�t�@�͏��������Ɋm�ۍς݂ł�

		//��փo�b�t�@�̃|�C���^���Z�b�g
		pReadBuff = g_tFS_Info.pucaReadSubBuff;
		pfa->aptr = pReadBuff;

		//2004/08/29 Add by Madoka
		if(len == 0) len = 512;	//�T�C�Y�w�肪�����ꍇ�͍ő�T�C�Y�ɂ���
	}

	//�Y������ꏊ��FAT�`�F�C���e�[�u���C���f�b�N�X���Z�o
	//
	//�~���������Z�N�^�ԍ��́AFAT�`�F�C���e�[�u���̃N���X�^�ԍ�����
	//�Z�o�ł��܂��B
	//
	//�܂��A���ɓǂݍ���P/ECE�ł̃Z�N�^�ԍ�����ǂݍ��ݍς݂̃o�C�g
	//�����Z�o���A�����1�N���X�^������̃o�C�g���Ŋ���ƁAFAT�`�F�C��
	//�e�[�u���̃C���f�b�N�X���킩��̂ŁA���̃C���f�b�N�X�̈ʒu��
	//�����ꂽ�N���X�^�ԍ����畨���Z�N�^�ԍ����Z�o���܂��B
	//
	//
	//�� << 9 = * 512
	//�� �{������9�V�t�g���Ă���E��g_tFS_Info.ucClustorSizeShift��
	//�� �V�t�g����K�v������̂ł����A�E�ɃV�t�g����񐔂̕����A512
	//�� ������9�V�t�g����񐔂�葽���Ȃ�̂�(1�N���X�^��512bytes�ȏ�)
	//�� (g_tFS_Info.ucClustorSizeShift - 9)�񕪉E�ɃV�t�g���Čv�Z
    //�� ���Ă��܂��B
	//��
	//�� ���Ȃ݂ɁAg_tFS_Info.ucClustorSizeShift��64MB��MMC����
	//�� 10 = 1�N���X�^ = 1024bytes ��128MB����
	//�� 11 = 1�N���X�^ = 2048bytes �ƂȂ�܂��B
	iFATChainIndex = sct >> (g_tFS_Info.ucClustorSizeShift - 9);
    
	//�ǂݍ��ݕ����Z�N�^�ԍ����Z�o
	ulReadSector = g_tFS_Info.ulFirstClusterAddress						//�N���X�^2�̕����Z�N�^�ԍ�
					+ ((g_tFS_Info.pusaFATchain[iFATChainIndex] - 2)	//�I�t�Z�b�g�Z�N�^��
					   << g_tFS_Info.ucSectorsParClusterShift);	

	//�N���X�^���̓ǂݍ��ރZ�N�^�̈ʒu�𒲐�
	ulReadSector += (sct % g_tFS_Info.ucSectorsParCluster);
	
	//1�Z�N�^���ǂݍ���
	mmcReadSector(ulReadSector,pReadBuff);

	return len;

}
//---------------------------------------------------------------------------
int mmcFileClose(FILEACC *pfa)
{

	//MMC��pceFileClose
	//����I���̏ꍇ0��Ԃ��܂��B

	//�ꉞ�`�F�b�N
	if(pfa == NULL) return 1;
	if(pfa->valid != VALIDFILEACC) return 1;

	//�t�@�C������I���
	pfa->valid = 0;

	return 0;

}
//---------------------------------------------------------------------------
#ifdef __PCEKN__		// 2004/11/15 Add by N.SAWA
//2005/04/02 Add by Madoka
unsigned char g_bMMCInitEnable;		//�J�[�l����MMC�������L���t���O

void InitMMC_API(void)
{

	//MMC�pAPI�̏�����

	//API��o�^
	pceVectorSetKs(KSNO_MMC_AppExecFile,mmcAppExecFile);
	pceVectorSetKs(KSNO_MMC_Exit,mmcExit);
	pceVectorSetKs(KSNO_MMC_FileClose,mmcFileClose);
	pceVectorSetKs(KSNO_MMC_FileFindClose,mmcFileFindClose);
	pceVectorSetKs(KSNO_MMC_FileFindNext,mmcFileFindNext);
	pceVectorSetKs(KSNO_MMC_FileFindOpen,mmcFileFindOpen);
	pceVectorSetKs(KSNO_MMC_FileOpen,mmcFileOpen);
	pceVectorSetKs(KSNO_MMC_FileReadMMCSct,mmcFileReadMMCSct);
	pceVectorSetKs(KSNO_MMC_FileReadSct,mmcFileReadSct);
	pceVectorSetKs(KSNO_MMC_GetCIDInfo,mmcGetCIDInfo);
	pceVectorSetKs(KSNO_MMC_GetCSDInfo,mmcGetCSDInfo);
	pceVectorSetKs(KSNO_MMC_Init,mmcInit);
	pceVectorSetKs(KSNO_MMC_ReadSector,mmcReadSector);			//2004/11/02 Add by Madoka
	pceVectorSetKs(KSNO_MMC_GetInitResult,mmcGetInitResult);	//2005/03/12 Add by Madoka

	//������ő�t�@�C���T�C�Y��������
	g_ulMaxFileSize = 0;

	//�O���[�o�����[�N�G���A�ւ̃|�C���^��������
	g_tFS_Info.ptaDirTable = NULL;
	g_tFS_Info.pusaFATchain = NULL;
	g_tFS_Info.pucaReadSubBuff = NULL;

	//2005/03/12 Add by Madoka
	//���������̖߂�l�ێ��ϐ���������
	g_ucInitResult = 0;

	//2005/04/02 Add by Madoka
	g_bMMCInitEnable = 0;		//�f�t�H���g��MMC���������Ȃ�
}
#endif //#ifdef PCEKN	// 2004/11/15 Add by N.SAWA
//---------------------------------------------------------------------------
#ifdef __PCEKN__		// 2004/11/15 Add by N.SAWA
int mmcAppExecFile(const char *fname,int resv)
{

	//MMC���pex�t�@�C�������s
	
	//���s��Ԃ��X�V
	appstat = 5;	//�A�v�����I�����AMMC��̃t�@�C����ǂݍ���
		
	//�ǂݍ��ރt�@�C������ۑ�
	strncpy(exec_fname, fname, sizeof(exec_fname));

	return 0;

}
#endif //#ifdef PCEKN	// 2004/11/15 Add by N.SAWA
//---------------------------------------------------------------------------

//��2005/05/04 Added by Madoka
//pceFile�nAPI�̃t�b�N��__PCEFILE_API_HOOK__����`�����
//���鎞�̂ݎ��s
#ifdef __PCEFILE_API_HOOK__
//{{2004/12/12 Add by N.SAWA

/****************************************************************************
 *	hook_mmc / unhook_mmc
 ****************************************************************************/

//2005/0611 Changed by Madoka
//#include <vector.h>
#include "ufe/vector.h"

#define KSNO_MMC_FileOpen	230
#define KSNO_MMC_FileClose	226
#define KSNO_MMC_FileReadSct	232
#define KSNO_MMC_FileFindOpen	229
#define KSNO_MMC_FileFindClose	227
#define KSNO_MMC_FileFindNext	228

static int hooked; /* 0 = �t�b�N���Ă��Ȃ�, 1 = �t�b�N���Ă��� */
static int (*old_pceFileOpen)(FILEACC* pfa, const char* fname, int mode);
static int (*old_pceFileClose)(FILEACC* pfa);
static int (*old_pceFileReadSct)(FILEACC* pfa, void* ptr, int sct, int len);
static int (*old_pceFileFindOpen)(FILEINFO* pfi);
static int (*old_pceFileFindClose)(FILEINFO* pfi);
static int (*old_pceFileFindNext)(FILEINFO* pfi);

static int
new_pceFileOpen(FILEACC* pfa, const char* fname, int mode)
{
	int retval;

	/* �{�̃t���b�V���������AMMC�̏��Ƀg���C */
	retval = old_pceFileOpen(pfa, fname, mode);
	if(retval != 0) {
		retval = mmcFileOpen(pfa, fname, mode);
	}

	return retval;
}

static int
new_pceFileReadSct(FILEACC* pfa, void* ptr, int sct, int len)
{
	int retval;

	if(pfa->chain) { /* �{�̃t���b�V�������� */
		retval = old_pceFileReadSct(pfa, ptr, sct, len);
	} else { /* MMC */
		retval = mmcFileReadSct(pfa, ptr, sct, len);
	}

	return retval;
}

static int
new_pceFileClose(FILEACC* pfa)
{
	int retval;

	if(pfa->chain) { /* �{�̃t���b�V�������� */
		retval = old_pceFileClose(pfa);
	} else { /* MMC */
		retval = mmcFileClose(pfa);
	}

	return retval;
}

/* FILEINFO.works[15] */
#define FF_PCE	0xAB	/* �{�̃t���b�V�����������쒆 */
#define FF_MMC	0xCD	/* MMC���쒆 */

static int
new_pceFileFindOpen(FILEINFO* pfi)
{
	int retval;

	retval = old_pceFileFindOpen(pfi);
	if(retval == 0) {
		pfi->works[15] = FF_PCE; /* �{�̃t���b�V�������������J�n */
	} else { /* ���肦�Ȃ���... */
		retval = mmcFileFindOpen(pfi);
		if(retval == 0) {
			pfi->works[15] = FF_MMC; /* MMC�����J�n */
		}
	}

	return retval;
}

static int
new_pceFileFindNext(FILEINFO* pfi)
{
	int retval = 0;

	if(pfi->works[15] == FF_PCE) { /* �{�̃t���b�V�������������� */
		retval = old_pceFileFindNext(pfi);
		if(!retval) { /* �{�̃t���b�V�������������I�� */
			old_pceFileFindClose(pfi);
			if(mmcFileFindOpen(pfi) == 0) {
				pfi->works[15] = FF_MMC; /* MMC�����J�n */
				/* FALLTHRU */
			}
		}
	}
	if(pfi->works[15] == FF_MMC) { /* MMC������ */
		retval = mmcFileFindNext(pfi);
	}

	return retval;
}

static int
new_pceFileFindClose(FILEINFO* pfi)
{
	int retval = -1;

	switch(pfi->works[15]) {
	case FF_PCE: /* �{�̃t���b�V�������������� */
		retval = old_pceFileFindClose(pfi);
		break;
	case FF_MMC: /* MMC������ */
		retval = mmcFileFindClose(pfi);
		break;
	}

	return retval;
}

static void
hook_mmc()
{
	if(!hooked) {
		old_pceFileOpen      = pceVectorSetKs(KSNO_FileOpen     , new_pceFileOpen     );
		old_pceFileReadSct   = pceVectorSetKs(KSNO_FileReadSct  , new_pceFileReadSct  );
		old_pceFileClose     = pceVectorSetKs(KSNO_FileClose    , new_pceFileClose    );
		old_pceFileFindOpen  = pceVectorSetKs(KSNO_FileFindOpen , new_pceFileFindOpen );
		old_pceFileFindNext  = pceVectorSetKs(KSNO_FileFindNext , new_pceFileFindNext );
		old_pceFileFindClose = pceVectorSetKs(KSNO_FileFindClose, new_pceFileFindClose);
		hooked = 1;
	}
}

static void
unhook_mmc()
{
	if(hooked) {
		pceVectorSetKs(KSNO_FileOpen     , old_pceFileOpen     );
		pceVectorSetKs(KSNO_FileReadSct  , old_pceFileReadSct  );
		pceVectorSetKs(KSNO_FileClose    , old_pceFileClose    );
		pceVectorSetKs(KSNO_FileFindOpen , old_pceFileFindOpen );
		pceVectorSetKs(KSNO_FileFindNext , old_pceFileFindNext );
		pceVectorSetKs(KSNO_FileFindClose, old_pceFileFindClose);
		hooked = 0;
	}
}

//}}2004/12/12 Add by N.SAWA
#endif //__PCEFILE_API_HOOK__
//��2005/05/04 Added by Madoka

#if 0  //2005/03/12 Vanished by Madoka
//{{2004/12/13 Add by N.SAWA

int mbr_protect = 1;	/* �Z�N�^#0�v���e�N�g 0:�������݋���/1:�������݋֎~ */

//�Z�N�^���C�g(�჌�x��MMC����֐�)
//MMC��1�Z�N�^��(512�o�C�g��z��)�f�[�^���������݂܂��B
//
//unsigned long ulSector	�ǂݏo���ΏۃZ�N�^(�����Z�N�^�ԍ�)
//const unsigned char *pBuff	�o�b�t�@(�K��512�o�C�g�ȏ゠�邱��)
//
//����ɓǂݏo�����ꍇ��1���A�G���[�����������ꍇ��0��Ԃ��܂�
unsigned char mmcWriteSector(unsigned long ulSector,const unsigned char *pucBuff)
{

	//�Z�N�^���C�g
	//MMC��1�Z�N�^��(512�o�C�g��z��)�f�[�^���������݂܂�
	//
	//      unsigned long ulSector	�ǂݏo���ΏۃZ�N�^(�����Z�N�^�ԍ�)
	//const unsigned char *pBuff	�o�b�t�@(�K��512�o�C�g�ȏ゠�邱��)
	//
	//����ɓǂݏo�����ꍇ��1���A�G���[�����������ꍇ��0��Ԃ��܂�
	unsigned long ulAddress = ulSector * g_tMMCInfo.tCSDInfo.usReadBlockLength;	//�A�h���X
	unsigned char ucRet = 0;		//�߂�l�������ɐݒ肷��
	unsigned char ucWork1;			//�f�[�^���X�|���X�g�[�N�����󂯎��
	unsigned char ucWork2;			//�r�W�[�����҂��̌��ʂ��󂯎��
	//int iIndex;						//���M�o�b�t�@�̃C���f�b�N�X

////////////////////////////////////////////////////////////////////////////////
//{{�Z�N�^#0�v���e�N�V�������K�v�Ȃ��ꍇ�́A���͈̔͂̃R�[�h���폜���Ă��������B
//////
	// * ����AmmcInit()���Ă΂���mmcWriteSector()���Ă΂��ƁA
	//   g_tMMCInfo.tCSDInfo.usReadBlockLength��0�̂܂܂Ȃ̂ŁAulAddress�̌v�Z���ʂ͏��0�ƂȂ�A
	//   ���Ȃ킿MBR�̏��������ɂȂ��Ă��܂��܂��B
	//   ���Ɋ댯�ł��̂ŁA�Z�N�^#0�̏������������͂ł��Ȃ��悤�ɂ��܂����B
	// * �����I��ulSector��0���w�肵��mmcWriteSector()���Ăт������ꍇ���A���������s�Ƃ��܂��B
	//   ���Ƃ��΁A�t�H�[�}�b�g����ulSector=0�̏����������������A��������s�����Ƃ��邽�߁A
	//   �{�v���O�����ł�MMC/SD�J�[�h���t�H�[�}�b�g�ł��܂���B
	//   �������܂��펞�g�p������̂ł��Ȃ��̂ŁA�t�H�[�}�b�g�ł��Ȃ����Ƃ������S���̂�܂����B
	// ��2004/11/05�ǋL
	// * mbr_protect�O���[�o���ϐ��ɂ���ď������݋��ł���悤�ɂ��܂����B
	if(ulAddress == 0 && mbr_protect) {
		return 0; /* ���s */
	}
//////
//}}�Z�N�^#0�v���e�N�V�������K�v�Ȃ��ꍇ�́A���͈̔͂̃R�[�h���폜���Ă��������B
////////////////////////////////////////////////////////////////////////////////

	//�J�[�h��Active��
	CARD_ENABLE

	//CMD24���M����R1���X�|���X����M
	if(mmcSendCommandAndRecvResponse(24,ulAddress) == 0x00)
	{

		// [�Q�l]
		// http://www.renesas.com/avs/resource/japan/jpn/pdf/flashcard/j203658_hb28e016mm2.pdf
		// p.64 �f�[�^���X�|���X�A�f�[�^�g�[�N��
		// p.70 SPI�o�X�^�C�~���O�E�V���O���u���b�N���C�g

		//8�N���b�N���̃_�~�[���M(Nwr)
		mmcByteSend(0xFF);

		//{{�f�[�^�u���b�N���M�J�n

		//�X�^�[�g�f�[�^�u���b�N�g�[�N�����M
		mmcByteSend(0xFE);

		//���[�U�f�[�^���M
		//for(iIndex = 0;iIndex < g_tMMCInfo.tCSDInfo.usReadBlockLength;++iIndex)
		//{
		//	mmcByteSend(pucBuff[iIndex]);
		//}
		mmcSendData(pucBuff, g_tMMCInfo.tCSDInfo.usReadBlockLength);

		//16�r�b�gCRC���M (SPI���[�h�͏�����Ԃ�CRC Off�Ȃ̂Ń_�~�[�ō\���܂���)
		mmcByteSend(0xFF);
		mmcByteSend(0xFF);

		//}}�f�[�^�u���b�N���M����

		//�f�[�^�u���b�N���M��������ɁA�f�[�^���X�|���X�g�[�N�����ԑ�����܂��B
		//�f�[�^�u���b�N���M�̌�ɁA�_�~�[�̑��M��N���b�N�����Ă͂����܂���B
		//�f�[�^���X�|���X�g�[�N���ɂ́A�X�^�[�g�r�b�g�͂���܂���B
		//
		//���]�k�ł�����ǁA�r�W�[�����܂ŏ������݂͏I����ĂȂ��͂��Ȃ̂ɁA
		//  �Ȃ�����ɐ旧����"110"���C�g�G���[�����o�ł���̂��s�v�c�ł�...
		
		//�f�[�^���X�|���X�g�[�N����M
		// xxx0sss1
		// |||||||+- 1�Œ�
		// ||||+++-- status "010"-�f�[�^���󂯓����ꂽ�B
		// ||||             "101"-CRC�G���[�ɂ��f�[�^�͋��ۂ��ꂽ�B
		// ||||             "110"-���C�g�G���[�ɂ��f�[�^�͋��ۂ��ꂽ�B
		// |||+----- 0�Œ�
		// +++------ �s��(BUFFALO RSDC-32M�ł�111�ƂȂ�悤�ł�)
		ucWork1 = mmcByteRecv();

		//�r�W�[�����҂�
		ucWork2 = mmcWaitBusy();

		//�f�[�^���X�|���X�g�[�N�����g�f�[�^���󂯓����ꂽ�h�ŁA
		//�r�W�[�����҂����^�C���A�E�g�łȂ���΁A
		//�������ݐ����A1��Ԃ�
		if(((ucWork1 & 0x1F) == 0x05) && (ucWork2 == 1))
		{
			ucRet = 1;
		}

	}
	
	//�J�[�h��NonActive��
	CARD_DISABLE

	return ucRet;

}

//}}2004/12/13 Add by N.SAWA
#endif //2005/03/12 Vanished by Madoka

