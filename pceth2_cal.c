/*
 *	pceth2 - �J�����_�[�֘A
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/05/13	�J���J�n
 *	2005/05/31	����Ǝ���
 *	2005/06/11	���t��i�߂�̂��������ōs��
 *				�A���ۂ̏����ŁA�����摜��ǂݍ��܂Ȃ��悤�ɂ��A�����߂͉摜��ǂݒ����A���܂����ۂ�����
 *	2005/07/19	pceth2_isCalenderMode()��ǉ�
 */

#include <string.h>
#include <piece.h>
#include "zurapce/zurapce.h"

#include "common.h"
#include "pceth2_cal.h"
#include "pceth2_sys.h"
#include "pceth2_grp.h"
#include "pceth2_arc.h"
#include "pceth2_str.h"

static PIECE_BMP	pcircle;
static BYTE		*circle;
static int			circleAnim;

#define MONTH_INDEX(x)	((x) - START_MONTH)

#define FNAMELEN_CAL	10
#define BG_CALENDER		"CAL_BG.pgx"
#define CIRCLE_CALENDER	"CAL_CIRCLE.pgd"

/*
 *	�X�N���v�g���ŃJ�����_�[���[�h�Ɉڍs	D4,16
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	0�i�����߂��j
 */
int pceth2_calenderInitEx(SCRIPT_DATA *s)
{
	s->p++;	// D
	MONTH = pceth2_getNum(s);	// ��
	s->p++;	// ,
	DAY = pceth2_getNum(s);	// ��
	pceth2_calenderInit();

	return 0;
}

/*
 *	�J�����_�[�\����������
 */
void pceth2_calenderInit()
{
	static const int day_num[] = {31, 30, 31};	// MONTH_INDEX(x)���̓���
	char buf[FNAMELEN_CAL + 1];

	if (DAY > day_num[MONTH_INDEX(MONTH)]) {
		MONTH++;
		DAY = 1;
	}

	if (!pceth2_isCalenderMode() || DAY == 1) {
		pceth2_loadGraphic(BG_CALENDER, GRP_BG);	// �w�i
		// �J�����_�[�摜�t�@�C�������쐬���ēǂݍ���
		pcesprintf(buf, "CAL_%02d.pgx", MONTH);
		pceth2_loadGraphic(buf, GRP_C);
		pceth2_DrawGraphic();

		Ldirect_VBuffClear(0, 0, DISP_X, DISP_Y);
	}
	circle = fpk_getEntryData(CIRCLE_CALENDER, NULL, NULL);	// ��
	PieceBmp_Construct(&pcircle, circle);
	// �������������Ă�̂ŁA�����m�ۂ��Ȃ��Ƃ����Ȃ������Bif{}����o���܂����B
	// �@���x���x�����烁�����c�ʂ����Ȃ���Ώ�ɒu���Ƃ��������ǁc�B
	circleAnim = 0;

	play.gameMode = GM_CALENDER;
}

#define CIRCLE_L	7
#define CIRCLE_T	22
#define CIRCLE_W	16
#define CIRCLE_H	12
#define ANIMATION_PROC	1

static const int zero_date[] = {0, 3, 5};	// MONTH_INDEX(x)��0���̗j��

/*
 *	���j�����Ԃ�
 */
int pceth2_getDate(int month, int day)
{
	return (day + zero_date[MONTH_INDEX(month)]) % 7;
}

/*
 *	�J�����_�[�̊ۂ�`��
 */
void pceth2_calenderDrawCircle()
{
	DRAW_OBJECT d;

	pceLCDSetObject(&d, &pcircle, \
					pceth2_getDate(MONTH, DAY) * CIRCLE_W + CIRCLE_L, \
					(DAY + zero_date[MONTH_INDEX(MONTH)]) / 7 * CIRCLE_H + CIRCLE_T, \
					0, (circleAnim / ANIMATION_PROC) * 16, 16, 16, DRW_CLR(COLOR_GLAY_B, COLOR_MASK));
	pceLCDDrawObject(d);
	Ldirect_Update();

	if (++circleAnim >= 16 * ANIMATION_PROC) {
		pceHeapFree(circle);
		circle = NULL;
		if (play.scData.size) {
			play.gameMode = GM_KEYWAIT;	// SCRIPT���̃J�����_�[���߂ɑΉ�
		} else {
			pceth2_loadEVScript();	// ����EV�X�N���v�g��ǂ�
		}
	}
}
