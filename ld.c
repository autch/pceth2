/*
 *	16�K���\���֘A���C�u����
 *	�i����RAM�ɒu���Ȃ��傫���f�[�^����ѕW��PIECE_BMP�p�֐��j
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 */

#include <piece.h>
#include "ld.h"

BYTE vbuff[DISP_X * DISP_Y];
BYTE lbuff[DISP_X * DISP_Y];
BYTE dbuff[PAGE_NUM][DISP_X * DISP_Y / 4];

/*
 *	PIECE_BMP�����i�S���ʁj
 */
void Get_PieceBmp(PIECE_BMP *pbmp, BYTE *data)
{
	pbmp->header = *((PBMP_FILEHEADER *)(data));
	pbmp->buf = data + sizeof(PBMP_FILEHEADER);
	pbmp->mask = pbmp->buf + (pbmp->header.w * pbmp->header.h * pbmp->header.bpp >> 3);
}

/*
 *	PIECE_BMP�`��i4�K���܂Łj
 *//*
void Draw_Object(PIECE_BMP *pbmp, const int dx, const int dy)
{
	DRAW_OBJECT dObj;
	pceLCDSetObject(&dObj, pbmp, dx, dy, 0, 0, pbmp->header.w, pbmp->header.h, DRW_NOMAL);
	pceLCDDrawObject(dObj);
}
*/
/*
 *	vbuff�𔽓]
 *//*
void VBuffReverse()
{
	int i;

	for(i = 0; i < DISP_X * DISP_Y; i += 4) {
		*(int *)(vbuff + i) ^= 0x03030303;
	}
}*/
