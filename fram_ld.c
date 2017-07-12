/*
 *	16�K���\���֘A���C�u����
 *	�i����RAM�z�u���Bld.h�̐�����ǂ�ō���RAM�ɔz�u���邱�Ɓj
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 */

#include <piece.h>
#include <string.h>
#include "ld.h"

int lbuffUpdate = 0, vbuffUpdate = 0/*, darkUpdate = 0*/;

//	fram_ld.c���ł̂ݎg�p����֐��̃v���g�^�C�v�錾
static void ld_LBuffCopy(const int page);
static void ld_VBuffCopy(const int page);
//void ld_DbuffDark(const int page);

/*
 *	LCD�_�C���N�g�]��
 */
void ld_LCDTransDirect()
{
	static int count = 0;
	if (lbuffUpdate) {
		ld_LBuffCopy(count);
		lbuffUpdate--;
	}
/*	if (darkUpdate) {
		ld_DbuffDark(count);
		darkUpdate--;
	}
*/	if (vbuffUpdate) {
		ld_VBuffCopy(count);
		vbuffUpdate--;
	}
	pceLCDTransDirect(dbuff[count]);
	count = (count + 1) % PAGE_NUM;	// 13�K���Ȃ�&3�ő����ۂ��񂾂���
}

/* dbuff��lbuff���R�s�[���� */
void ld_LBuffUpdate()	{ lbuffUpdate = PAGE_NUM; vbuffUpdate = 0; }
/* vbuff�̃N���A�idbuff�ɃR�s�[����ۂ̃}�X�N0x80�j */
void ld_VBuffClear(int x, int y, int w, int h)
{
	int i;

	// �N���b�s���O
	if (x >= DISP_X || y >= DISP_Y) { return; }
	if (x < 0) { w += x; x = 0; }
	if (y < 0) { h += y; y = 0; }
	if (w <= 0 || h <= 0) {	return; }
	if (x + w >= DISP_X) { w = DISP_X - x; }
	if (y + h >= DISP_Y) { h = DISP_Y - y; }

	for (i = y; i < y + h; i++) {
		memset(vbuff + i * DISP_X + x, V_TRANS, w);
	}
}
/* dbuff��vbuff���R�s�[���� */
void ld_VBuffUpdate()	{ vbuffUpdate = PAGE_NUM; }
/* dbuff�ɈÉ��t�B���^�������� */
//void ld_FilterDark()	{ darkUpdate = PAGE_NUM; }

/*
 *	lbuff����dbuff�Ƀ}�X�N�Ȃ��R�s�[
 */
static void ld_LBuffCopy(const int page)
{
	static const BYTE color_table[][PAGE_NUM] = {
		{0,0,0,0,0}, {1,0,0,0,0}, {1,0,1,0,0}, {1,0,1,0,1}, {1,1,1,1,0},
		{1,1,1,1,1}, {2,1,1,1,1}, {2,1,2,1,1}, {2,1,2,1,2}, {2,2,2,2,1},
		{2,2,2,2,2}, {3,2,2,2,2}, {3,2,3,2,2}, {3,2,3,2,3}, {3,3,3,3,2},
		{3,3,3,3,3}
	};
	int i, j;
	BYTE *d_ptr = dbuff[page], *l_ptr = lbuff;
	BYTE c, h_bit, l_bit;

	for (i = 0; i < DISP_X / 8; i += 1) {	// x���W
		for (j = 0; j < DISP_Y; j += 1) {	// y���W
			c = color_table[*l_ptr++][page];	h_bit = c >> 1;	l_bit = c & 1;
			c = color_table[*l_ptr++][page];	h_bit |= c & 2;	l_bit |= (c & 1) << 1;
			c = color_table[*l_ptr++][page];	h_bit |= (c & 2) << 1;	l_bit |= (c & 1) << 2;
			c = color_table[*l_ptr++][page];	h_bit |= (c & 2) << 2;	l_bit |= (c & 1) << 3;
			c = color_table[*l_ptr++][page];	h_bit |= (c & 2) << 3;	l_bit |= (c & 1) << 4;
			c = color_table[*l_ptr++][page];	h_bit |= (c & 2) << 4;	l_bit |= (c & 1) << 5;
			c = color_table[*l_ptr++][page];	h_bit |= (c & 2) << 5;	l_bit |= (c & 1) << 6;
			c = color_table[*l_ptr++][page];	h_bit |= (c & 2) << 6;	l_bit |= (c & 1) << 7;
			*d_ptr++ = h_bit;
			*d_ptr++ = l_bit;
			l_ptr += DISP_X - 8;
		}
		l_ptr += -(DISP_X * DISP_Y) + 8;
	}
}

/*
 *	�}�X�N�t��16�K���摜�`��
 */
void ld_DrawObject(const PIECE_BMP *pbmp, int dx, int dy)
{
	int dw, h, sx, sy, sw, i, j, m_bit;
	BYTE *l_ptr, *b_ptr, b = 0, *m_ptr, m = 0, bit;
	
	sx = sy = 0;
	dw = sw = pbmp->header.w;
	h = pbmp->header.h;
	m_bit = pbmp->header.mask;
	// �N���b�s���O
	if (dx < 0)				{ dw += dx;	sx = -dx;	dx = 0; }
	if (dx + dw > DISP_X)	{ dw = DISP_X - dx; }
	if (dy < 0)				{ h += dy;	sy = -dy;	dy = 0; }
	if (dy + h > DISP_Y)		{ h = DISP_Y - dy; }

	l_ptr = lbuff + (dx + DISP_X * dy);
	b_ptr = pbmp->buf + ((sx + sw * sy) >> 1);
	m_ptr = pbmp->mask + ((sx + sw * sy) >> 3);

	for (i = 0; i < h; i += 1) {	// y���W
		bit = sx;
		if (bit & 1) { b = *b_ptr++ << 4; }
		if (bit & 7) { m = (m_bit)? (*m_ptr++ << (bit & 7)) : 0; }
		for (j = 0; j < dw; j += 1) {	// x���W
			if (!(bit   & 1)) { b = *b_ptr++; }
			if (!(bit++ & 7)) { m = (m_bit)? *m_ptr++ : 0; }
			*l_ptr = (m & 0x80)? *l_ptr : (b >> 4);
			l_ptr++;
			b <<= 4;
			m <<= 1;
		}
		l_ptr += DISP_X - dw;
		b_ptr += ((sw - dw) >> 1);
		m_ptr += ((sw - dw) >> 3);
	}

	ld_LBuffUpdate();
}

/*
 *	�_�`��
 */
void ld_LCDPoint(const BYTE c, const int x, const int y)
{
	// �N���b�s���O
	if (x >= DISP_X)	return;
	if (x <       0)	return;
	if (y >= DISP_Y)	return;
	if (y <       0)	return;
	
	*(lbuff + (x + DISP_X * y)) = (c > COLOR_MAX)? COLOR_MAX : c;
	ld_LBuffUpdate();
}

/*
 *	�h��Ԃ�
 */
void ld_LCDPaint(BYTE c, int x, int y, int w, int h)
{
	int i;
	BYTE *l_ptr;

	// �N���b�s���O
	if (x >= DISP_X || y >= DISP_Y) { return; }
	if (x < 0)			{ w += x;	x = 0; }
	if (y < 0)			{ h += y;	y = 0; }
	if (x + w > DISP_X)	{ w = DISP_X - x; }
	if (w <= 0 || h <= 0) {	return; }
	if (y + h > DISP_Y)	{ h = DISP_Y - y; }

	c = (c > COLOR_MAX)? COLOR_MAX : c;
	l_ptr = lbuff + (x + DISP_X * y);
	for (i = 0; i < h; i += 1) {
		memset(l_ptr, c, w);
		l_ptr += DISP_X;
	}

	ld_LBuffUpdate();
}

/*
 *	vbuff����dbuff�Ƀ}�X�N�t���R�s�[
 */
static void ld_VBuffCopy(const int page)
{
	int i, j;
	BYTE *d_ptr = dbuff[page], *v_ptr = vbuff, p, mask, h_bit, l_bit;

	for (i = 0; i < DISP_X / 8; i += 1) {	// x���W�i8�s�N�Z������
		for (j = 0; j < DISP_Y; j += 1) {	// y���W
			p = *v_ptr++;	mask  = (p & V_TRANS) >> 2;	h_bit = p >> 1;	l_bit = p & 1;
			p = *v_ptr++;	mask |= (p & V_TRANS) >> 1;	h_bit |= p & 2;	l_bit |= (p & 1) << 1;
			p = *v_ptr++;	mask |= (p & V_TRANS);		h_bit |= (p & 2) << 1;	l_bit |= (p & 1) << 2;
			p = *v_ptr++;	mask |= (p & V_TRANS) << 1;	h_bit |= (p & 2) << 2;	l_bit |= (p & 1) << 3;
			p = *v_ptr++;	mask |= (p & V_TRANS) << 2;	h_bit |= (p & 2) << 3;	l_bit |= (p & 1) << 4;
			p = *v_ptr++;	mask |= (p & V_TRANS) << 3;	h_bit |= (p & 2) << 4;	l_bit |= (p & 1) << 5;
			p = *v_ptr++;	mask |= (p & V_TRANS) << 4;	h_bit |= (p & 2) << 5;	l_bit |= (p & 1) << 6;
			p = *v_ptr;  	mask |= (p & V_TRANS) << 5;	h_bit |= (p & 2) << 6;	l_bit |= (p & 1) << 7;
			h_bit &= ~mask;	*d_ptr &= mask;	*d_ptr++ |= h_bit;
			l_bit &= ~mask;	*d_ptr &= mask;	*d_ptr++ |= l_bit;
			v_ptr += DISP_X - 7;
		}
		v_ptr += -(DISP_X * DISP_Y) + 8;
	}
}

/*
 *	��ʑS�̔������É��t�B���^
 *//*
void ld_DbuffDark(const int page)
{
	int i;
	BYTE *d_ptr, h_bit, l_bit;
	
	d_ptr = dbuff[page];
	for(i = 0; i < (DISP_X * DISP_Y >> 3); i += 1) {
		h_bit = *d_ptr;
		l_bit = *(d_ptr + 1);
		*d_ptr++ = h_bit | l_bit;
		*d_ptr++ = h_bit | ~l_bit;
	}
}*/
