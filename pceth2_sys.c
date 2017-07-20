/*
 *	pceth2 - ���䖽�ߊ֘A
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/05/01	main.c���番��
 *	2005/05/09	EV_�`�X�N���v�g���J���悤��
 *				pceth2_jumpScript��EV�Ȃ�gosub�A����ȊO�Ȃ�goto��
 *	2005/05/27	Win��pceth2bin2�ɑΉ��i�X�N���v�g�`���𑽏��ύX
 *				getNum�̎d�l�ύX
 *	2005/06/11	pceth2_backTitle��ǉ�
 */

#include <string.h>
#include <piece.h>
#include "common.h"
#include "pceth2_sys.h"
#include "pceth2_arc.h"
#include "pceth2_str.h"
#include "pceth2_sav.h"

int pceth2_isCalenderMode();

//=============================================================================
//	�X�N���v�g�t�@�C���ǂݍ���
//=============================================================================

/*
 *	�X�N���v�g���J��
 *
 *	*s		�X�N���v�g�f�[�^�ւ̃|�C���^
 *	*fName	�X�N���v�g�t�@�C����
 */
void pceth2_loadScript(SCRIPT_DATA *s, const char *fName)
{
	pceth2_closeScript(s);
	strcpy(s->name, fName);
	s->data = fpk_getEntryData(s->name, &s->size, NULL);
}

/*
 *	�X�N���v�g�����
 *
 *	*s		�X�N���v�g�f�[�^�ւ̃|�C���^
 */
void pceth2_closeScript(SCRIPT_DATA *s)
{
	*s->name = '\0';
	pceHeapFree(s->data);
	s->data = NULL;
	s->p = s->size = 0;
}

//=============================================================================
//	�X�N���v�g
//=============================================================================

/*
 *	EV�X�N���v�g���J��
 */
void pceth2_loadEVScript()
{
	char buf[FNAMELEN_EV + 1];

	// EV�X�N���v�g�t�@�C�������쐬���ēǂݍ���
	pcesprintf(buf, "EV_%02d%02d%1d.scp", MONTH, DAY, TIME);
	pceth2_loadScript(&play.evData, buf);

	JUMP = 0;	// �W�����v�t���O�����Z�b�g�igosub�Ɂj

	// �ǂݍ��񂾎��_�Ŏ��̎��ԑтɐi�߂Ă���
	TIME++;

	play.gameMode = GM_EVSCRIPT;
}

/*
 *	�w��X�N���v�g�ɃW�����v	J000000000.scp
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	0�i�����߂��j
 */
int pceth2_jumpScript(SCRIPT_DATA *s)
{
	char buf[FNAMELEN_SCP + 1];
	s->p++;

	// �t�@�C�������o�b�t�@�ɃR�s�[
	pceth2_strcpy(buf, s, FNAMELEN_SCP);

	// 2005/06/20 �܂邵������āF1e, 4, 1��������goto
	if (JUMP == 1) {	// GM_SCRIPT��������ȉ��ŏ㏑������邩��֌W�Ȃ��񂾂���
		s->p = s->size;	// �Ō�܂œǂ񂾂��Ƃɂ���i�߂��Ă����瑦�I���j
	}
	pceth2_loadScript(&play.scData, buf);
	play.gameMode = (pceth2_isCalenderMode())? GM_KEYWAIT : GM_SCRIPT;

	return 0;
}

//=============================================================================
//	���x���W�����v
//=============================================================================

#define NUMLEN_LABEL	3

/*
 *	���x�����X�L�b�v���邾��	@000
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_skipLabel(SCRIPT_DATA *s)
{
	s->p += NUMLEN_LABEL + 1;
	return 1;
}

/*
 *	�w�胉�x���ɃW�����v		j000
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_jumpLabel(SCRIPT_DATA *s)
{
	DWORD ptr;
	s->p++;

	for (ptr = 0; ptr < s->size; ptr++) {
		if (*(s->data + ptr) == '@') {	// �X�N���v�g����
			ptr++;
			if (!strncmp(s->data + s->p, s->data + ptr, NUMLEN_LABEL)) {	// ���x���ƍ�
				s->p = ptr + NUMLEN_LABEL;
				break;
			}
		}
		if (pceth2_isKanji(s)) { ptr++; }	// �����̏ꍇ2bytes�i�߂�
	}

	return 1;
}

/*
 *	�����t���Ŏw�胉�x���ɃW�����v	b[exp],000
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_branchLabel(SCRIPT_DATA *s)
{
	s->p++;
	if (pceth2_calcExpression(s)) {
		pceth2_jumpLabel(s);
	} else {
		s->p += NUMLEN_LABEL + 1;
	}

	return 1;
}

//=============================================================================
//	�t���O�E���W�X�^����
//=============================================================================

unsigned short reg[REG_NUM];

/*
 *	�t���O�����W�X�^�Ƀ��[�h	l?,0
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_loadFlag(SCRIPT_DATA *s)
{
	int i, j;
	
	s->p++;
	i = pceth2_getNum(s);
	s->p++;
	j = *(s->data + s->p++) - '0';
	reg[j] = play.flag[i];

	return 1;
}

/*
 *	�t���O�ɑ��l���X�g�A	s?,[exp]
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_saveFlag(SCRIPT_DATA *s)
{
	int i, j;
	s->p++;
	i = pceth2_getNum(s);
	s->p++;
	j = pceth2_calcExpression(s);
	if (j != -1) {	// -1�i�f�t�H���g�l�j�̏ꍇ�ύX���Ȃ�
		play.flag[i] = j;
		if (i >= 81 && i <= 91) {	// �N���A�t���O�̏ꍇ
			play.flag[80] = 0;
			for (i = 81; i <= 91; i++) {	// flag[80]=�N���A�V�i���I��
				play.flag[80] += play.flag[i];
			}
			memcpy(&global.flag[0], &play.flag[80], sizeof(unsigned short) * GLOBAL_FLAG_NUM);
			pceth2_writeGlobalSaveData();	// �O���[�o���Z�[�u�ɏ�������
		}
	}

	return 1;
}

/*
 *	���W�X�^�ɑ��l���Z�b�g	=0,[exp]
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_setReg(SCRIPT_DATA *s)
{
	int i;
	s->p++;
	i = *(s->data + s->p++) - '0';
	s->p++;
	reg[i] = pceth2_calcExpression(s);

	return 1;
}

/*
 *	���W�X�^���C���N�������g	+0
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_incReg(SCRIPT_DATA *s)
{
	int i;
	s->p++;
	i = *(s->data + s->p++) - '0';
	reg[i]++;

	return 1;
}

/*
 *	���W�X�^���f�N�������g	-0
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_decReg(SCRIPT_DATA *s)
{
	int i;
	s->p++;
	i = *(s->data + s->p++) - '0';
	reg[i]--;

	return 1;
}

//=============================================================================
//	���̑�
//=============================================================================

/*
 *	���ԃE�F�C�g���w��	w?
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	0�i�����߂��j
 */
int pceth2_wait(SCRIPT_DATA *s)
{
    s->p++;
	wait = pceth2_getNum(s) * 30 / 100;
	play.gameMode = GM_TIMEWAIT;

	return 0;
}

/*
 *	�G�s���[�O�ɓ���i�����s���ɂ���j	z
 *
 *	*s		�X�N���v�g�f�[�^
 *
 *	return	1�i�����������s�j
 */
int pceth2_goEpilogue(SCRIPT_DATA *s)
{
	s->p++;
	MONTH = DAY = 0;

	return 1;
}

//=============================================================================
//	���l�v�Z
//=============================================================================

/*
 *	�I�[�o�[�t���[�`�F�b�N�����Ă��Ȃ��ʖڂȃX�^�b�N
 */
#define STACK_NUM	8
static int stack[STACK_NUM];
static int stack_index;

// �v�b�V��
static void _push(int num)
{
	stack[stack_index] = num;
	stack_index++;
}

// �|�b�v�i�X�^�b�N����̂Ƃ���0��Ԃ��j
static int _pop(void)
{
	if (stack_index > 0) {
		stack_index--;
		return stack[stack_index];
	}

	return 0;	// ����ŕ��L������肭�����ł���͂�
}

/*
 *	�t�|�[�����h�L�@�̌v�Z��������
 *	*s	�X�N���v�g�f�[�^
 *	return	�v�Z����
 */
int pceth2_calcExpression(SCRIPT_DATA *s)
{
	static const char * const operator_table[] = {
		"�{", "�|", "�~", "��", "��", "��", "��", "��", "��", "��",
	};
	int val1, val2, i;

	stack_index = 0;

	while (1) {
		switch (*(s->data + s->p))
		{
			case ' ':	// ���l
				s->p++;
				_push(pceth2_getNum(s));
				break;
			case '$':	// ���W�X�^
				s->p++;
				_push(reg[pceth2_getNum(s)]);
				break;
			default:	// ���Z�q
				val1 = _pop();
				val2 = _pop();
				for (i = 0; i < array_size(operator_table); i++) {
					if (!strncmp(s->data + s->p, operator_table[i], 2)) {
						s->p += 2;
						break;
					}
				}
				switch (i) {
					case 0:		_push(val2 +  val1);	break;	// �{
					case 1:		_push(val2 -  val1);	break;	// �|
					case 2:		_push(val2 *  val1);	break;	// �~
					case 3:		_push(val2 /  val1);	break;	// ��
					case 4:		_push(val2 == val1);	break;	// ��
					case 5:		_push(val2 != val1);	break;	// ��
					case 6:		_push(val2 <  val1);	break;	// ��
					case 7:		_push(val2 >  val1);	break;	// ��
					case 8:		_push(val2 <= val1);	break;	// ��
					case 9:		_push(val2 >= val1);	break;	// ��
					default:	return val1;
				}
		}
	}
}
