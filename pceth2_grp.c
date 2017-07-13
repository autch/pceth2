/*
 *	pceth2 - �摜�\���֘A
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/04/27	main.c���番��
 *	2005/05/10	�ǂݍ��݁^��������̃\�[�X�����������悤��
 *	2005/05/11	�\���ʒu���������ǉ�
 *	2005/05/27	Win��pceth2bin2�ɑΉ��i�X�N���v�g�`���𑽏��ύX
 *	2005/05/30	�����G�\���ʒuAUTO��p�~�ipceth2bin2���őΉ�
 *	2005/06/17	�����G�\���A�����̑������������t���O������
 *				�����G�\���ʒu�������E��3��ނɌ��炷
 *	2005/06/18	�����G�\���A�����ɃX���C�h�C���A�A�E�g��ǉ�
 *	2005/06/23	�w�i�摜�̍��������ŏ��������i�f�o�b�O�ł͖����j
 *	2005/06/30	�X���C�h�A�E�g������A�X���C�hx���W��߂��Ă��Ȃ������̂ŁA
 *				3/9�}�b�v�I���i���݂̂́u�Ă��I�v��j�Ń`�b�v�L�����\������Ȃ������̂��C��
 */

#include <string.h>
#include <piece.h>
#include "zurapce/zurapce.h"

#include "common.h"
#include "pceth2_grp.h"
#include "pceth2_arc.h"
#include "pceth2_str.h"

extern SAVE_DATA play;

static PIECE_BMP	pbmp[GRP_NUM];
static BYTE			pgx[GRP_NUM][DISP_X * DISP_Y * 5 / 8];

// �X���C�h�Ǘ��p
static int	slide_x[GRP_BG];	// 
static int	slide_goal;			// �ڕW���W
static int	slide_pos;			// �X���C�h���Ă���摜�̃Z�b�g�ʒu�iLCR�j

//=============================================================================
//	�摜�`��
//=============================================================================

#define POS_L	30
#define POS_C	64
#define POS_R	98

static void draw_object(const PIECE_BMP *pbmp, int dx, int dy)
{
	Ldirect_DrawObject(pbmp, dx, dy, 0, 0, pbmp->header.w, pbmp->header.h);
}

/*
 *	lbuff�ɉ摜�`��
 */
void pceth2_DrawGraphic()
{
//	static int pos_table[] = {30, 64, 98};	// �����G�\�����S�ʒu�e�[�u��

	if (*play.pgxname[GRP_BG]) {	// �w�i
		draw_object(&pbmp[GRP_BG], 0, 0);
	} else {
		Ldirect_Paint(15, 0, 0, DISP_X, DISP_Y);	// ���w�i
	}

	if (*play.pgxname[GRP_R]) {	// �E
		draw_object(&pbmp[GRP_R], (POS_R - pbmp[GRP_R].header.w / 2) + slide_x[GRP_R], DISP_Y - pbmp[GRP_R].header.h);
	}
	if (*play.pgxname[GRP_L]) {	// ��
		draw_object(&pbmp[GRP_L], (POS_L - pbmp[GRP_L].header.w / 2) + slide_x[GRP_L], DISP_Y - pbmp[GRP_L].header.h);
	}
	if (*play.pgxname[GRP_C]) {	// ����
		draw_object(&pbmp[GRP_C], (POS_C - pbmp[GRP_C].header.w / 2) + slide_x[GRP_C], DISP_Y - pbmp[GRP_C].header.h);
	}
}

//=============================================================================
//	�摜�t�@�C���ǂݍ���
//=============================================================================

/*
 *	����ʒu�ɉ摜��ǂݍ���
 *
 *	*fName	�摜�t�@�C����
 *	pos		�\���ʒu
 */
void pceth2_loadGraphic(const char *fName, const int pos)
{
	pceth2_clearGraphic(pos);

	strcpy(play.pgxname[pos], fName);
	/*pgx[pos] =*/ fpk_getEntryData(play.pgxname[pos], NULL, pgx[pos]);

	if (pgx[pos] != NULL) {
		PieceBmp_Construct(&pbmp[pos], pgx[pos]);
	} else {
		*play.pgxname[pos] = '\0';
	}
}

/*
 *	����ʒu�̉摜������
 *
 *	pos	�\���ʒu
 */
void pceth2_clearGraphic(const int pos)
{
	*play.pgxname[pos] = '\0';
//	pceHeapFree(pgx[pos]);
//	pgx[pos] = NULL;
	memset(pbmp[pos], 0, sizeof(PIECE_BMP));
}

//=============================================================================
//	�摜�\���E����
//=============================================================================

/*
 *	�w�i�摜�̎��ԑсA�V����w��	G0
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_setBGOption(SCRIPT_DATA *s)
{
	s->p++;
	if (*(s->data + s->p) < '0' + BG_WEATHER_FLAG) {	// ���ԑ�
		BG_TIME		= *(s->data + s->p++);
	} else {						// �V��
		BG_WEATHER	= *(s->data + s->p++) - BG_WEATHER_FLAG;
	}

	return 1;
}

/*
 *	�w�i�摜�A�C�x���g�摜��\��	B000000.pgx,0	V000000.pgx,0
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_loadBG(SCRIPT_DATA *s)
{	// ���w�i�摜�ύX�e�[�u��
	// 2005/07/19	��z�ȂǂŒ��ډ摜�ԍ����w�肷��P�[�X������݂����Ȃ̂ŁA
	//				�g���Ă��Ȃ��摜�ԍ��ɂ��Ă݂�
	static const char *cherry[][6] = {
		{"88", "78", "03", "01", "02", "04"},	// �Z��i�O�j
		{"89", "79", "07", "05", "06", "08"},	// �Z��i���j
		{"90", "80", "36", "34", "35", "37"},	// ��
		{"91", "81", "50", "48", "49", "51"},	// �쉈���̓�
	};	//	��{, �ڂ�, �ܕ��炫, ���J, �ܕ��U��, �t��
	char buf[FNAMELEN_BG + 1];
	int i;

	// �t�@�C�������o�b�t�@�ɃR�s�[
	pceth2_strcpy(buf, s, FNAMELEN_BG);

	if (*buf == 'B') {	// �w�i�摜
		for (i = 0; i < array_size(cherry); i++) {	// ���̍炫���ύX
			if (!strncmp(buf + 2, cherry[i][0], 2)) {
				if (MONTH == 5 || (MONTH == 4 && DAY >= 28)) {	// �t���i4/28�`�j
					strncpy(buf + 2, cherry[i][5], 2);
				} else if (MONTH == 4 && DAY >= 16) {			// �ܕ��U��i4/16�`�j
					strncpy(buf + 2, cherry[i][4], 2);
				} else if (MONTH == 4 || DAY >= 29) {			// ���J�i3/29�`�j
					strncpy(buf + 2, cherry[i][3], 2);
				} else if (DAY >= 16) {							// �ܕ��炫�i3/16�`�j
					strncpy(buf + 2, cherry[i][2], 2);
				} else {										// �ڂ݁i3/1�`�j
					strncpy(buf + 2, cherry[i][1], 2);
				}
			}
		}
		strncpy(buf + 4, play.bgopt, 2);	// ���ԑсA�V��𔽉f
	}

	s->p++;	// ,
	if (!(*(s->data + s->p++) - '0')) {	// �t���O0�̏ꍇ�S�摜����
		for (i = GRP_L; i <= GRP_R; i++) {
			pceth2_clearGraphic(i);
		}
	}
	pceth2_loadGraphic(buf, GRP_BG);	// �摜���J��
	pceth2_DrawGraphic();				// �K���摜�`��

	return 1;
}

/*
 *	�����G�摜��\��	C00000000.pgx,0	CLOCK00.pgx,0
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_loadChara(SCRIPT_DATA *s)
{
	char buf[FNAMELEN_CH + 1];
	int pos, i = 0;

	// �t�@�C�������o�b�t�@�ɃR�s�[
	if (*(s->data + s->p + 1) == 'L') {	// ���v
		pceth2_strcpy(buf, s, FNAMELEN_CLOCK);
	} else {
		pceth2_strcpy(buf, s, FNAMELEN_CH);
	}

	s->p++;	// ,
	pos = (int)(*(s->data + s->p++) - '0');	// �\���ʒu���擾
	for (i = GRP_L; i <= GRP_R; i++) {
		if (!strncmp(buf, play.pgxname[i], 1 + NUMLEN_CH)) {
			pceth2_clearGraphic(i);	// �����L���������ɓǂݍ��܂�Ă��������
			if (pos >= GRP_AUTO) {	// �����ʒu����i���ɓǂݍ��܂�Ă���ʒu�Ɂj
				pos = i;
			}
		}
	}
	if (pos >= GRP_AUTO) {	// �����ʒu����ł��Ȃ������̂Ŋ���̈ʒu��
		pos -= GRP_AUTO;
	}

	pceth2_loadGraphic(buf, pos);	// �摜���J��

	slide_x[pos] = 0;
	if (*(s->data + s->p) == ',') {	// �X���C�h�p����������΃X���C�h����
		s->p++;	// ,
		i = (int)(*(s->data + s->p++) - '0');
		slide_pos = pos;
		slide_x[pos] = ((i == 1)? -128 : 128);
		slide_goal = 0;
		wait = 8;	// �t���[�����x�͖���
		play.gameMode = GM_SLIDECHR;
		return 0;
	}

	if (*(s->data + s->p) != 'B' && *(s->data + s->p) != 'C' && *(s->data + s->p) != 'c') {
		pceth2_DrawGraphic();	// �����摜�n���߂łȂ���΂����ɕ`��
	}
	return 1;
}

/*
 *	����L�����̗����G�摜������	c000
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_clearChara(SCRIPT_DATA *s)
{
	int i;
	s->p++;	// c

	// �摜�t�@�C�����ƃL�����ԍ�����v���������
	for (i = GRP_L; i <= GRP_R; i++) {
		if (*play.pgxname[i] && !strncmp(s->data + s->p, play.pgxname[i] + 1, NUMLEN_CH)) {
			break;
		}
	}
	s->p += NUMLEN_CH;

	if (i < GRP_BG) {	// �폜�ΏۂɈ�v�H
		if (*(s->data + s->p) == ',') {	// �X���C�h�A�E�g����
			s->p++;
			slide_pos = i;
			// slide_x[slide_pos]�͊���1�̂͂�
			i = (int)(*(s->data + s->p++) - '0');
			slide_goal = ((i == 1)? -128 : 128);
			wait = 8;	// �Ƃ肠�����t���[�����x�͖���
			play.gameMode = GM_SLIDECHR;
			return 0;
		}

		pceth2_clearGraphic(i);
	}

	if (*(s->data + s->p) != 'B' && *(s->data + s->p) != 'C' && *(s->data + s->p) != 'c') {
		pceth2_DrawGraphic();	// �����摜�n���߂łȂ���΂����ɕ`��
	}
	return 1;
}

//=============================================================================
//	�����G�X���C�h
//=============================================================================

/*
 *	�����G�̃X���C�h����
 */
void pceth2_slideChara()
{
	slide_x[slide_pos] += (slide_goal - slide_x[slide_pos]) / wait;

	if (--wait <= 0) {
		slide_x[slide_pos] = slide_goal;
		if (slide_x[slide_pos] != 0) {	// ��ʊO�Ȃ�����܂�
			slide_x[slide_pos] = 0;	// �Z���^�[�ɖ߂��Ă����i�}�b�v�I��΍�j
			pceth2_clearGraphic(slide_pos);
		}
		play.gameMode = GM_SCRIPT;
	}

	pceth2_DrawGraphic();
}


/*
 *	���J�����_�[���[�h���ǂ����Ԃ��i�x�X���ׂĂ�̂Ŋ֐����j
 */
int pceth2_isCalenderMode()
{
	return (*(play.pgxname[GRP_BG] + 2) == 'L');
}
