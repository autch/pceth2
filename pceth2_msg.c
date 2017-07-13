/*
 *	pceth2 - ���b�Z�[�W�֘A
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/05/08	main.c���番��
 *	2005/05/27	getNum�d�l�ύX�ɑΉ�
 *	2005/06/15	���O�u��������ǉ��i����őS�����Ƃ������ǁ������Ă����Ă������R�s�y�Ń~�X
 *				!!�A!?�֑̋��������C���B�蓮�֑��ړI��\n�݂̂̉��s�͉��
 *	2005/06/16	�֑������ΏۂɁj��ǉ��B���ߒ��O��\n�����s�Ώۂ�
 *	2005/06/25	���O�u���������R���o�[�^���Ɉړ�
 */

#include <string.h>
#include <piece.h>
#include "zurapce/zurapce.h"
#include "ld.h"

#include "common.h"
#include "pceth2_msg.h"
#include "pceth2_sys.h"
#include "pceth2_str.h"

//=============================================================================
//	�����`��
//=============================================================================

/*
 *	�s���Ȃ�TRUE��Ԃ�
 */
BOOL pceth2_isLineTop()
{
	int x;
	FontFuchi_GetPos(&x, NULL);
	return (x <= MSG_X_MIN);
}

/*
 *	�y�[�W�擪�Ȃ�TRUE��Ԃ�
 */
BOOL pceth2_isPageTop()
{
	int y;
	FontFuchi_GetPos(NULL, &y);
	return (pceth2_isLineTop() && (y <= MSG_Y_MIN));
}

/*
 *	�y�[�W�擪�Ɉړ�����
 */
void pceth2_setPageTop()
{
	while (!pceth2_isPageTop()) {
		pceth2_putCR();
	}
}

/*
 *	���b�Z�[�W���N���A�A�o�b�t�@���N���A
 */
void pceth2_clearMessage(void)
{
	ld_VBuffClear(0, 0, DISP_X, DISP_Y);
	*play.msg = '\0';
	play.msglen = 0;
}

/*
 *	2�o�C�g������`���āA���b�Z�[�W�o�b�t�@�ɂ������
 */
void pceth2_putKanji(const char *str)
{
	FontFuchi_Printf("%c%c", *str, *(str + 1));
	*(play.msg + play.msglen++)	= *str;
	*(play.msg + play.msglen++)	= *(str + 1);
	*(play.msg + play.msglen)	= '\0';
}

/*
 *	���s���āA���b�Z�[�W�o�b�t�@�ɂ������
 */
void pceth2_putCR(void)
{
	FontFuchi_PutStr("\n");
	*(play.msg + play.msglen++)	= '\n';
	*(play.msg + play.msglen)	= '\0';
}






/*
 *	���b�Z�[�W���䏈��
 */
int pceth2_procControl(SCRIPT_DATA *s)
{
	s->p++;	// <
	switch (*(s->data + s->p++))
	{
		case 'S':	// �I�[�g����
		case 'F':
		{
			speed = pceth2_getNum(s);
			s->p++;	// >
			return 1;
		}
		break;
		case 'W':	// �E�F�C�g
		{
			wait = pceth2_getNum(s);
			s->p++;	// >
			play.gameMode = GM_TIMEWAIT;
		}
		break;
	}

	return 0;
}

/*
 *	�G�X�P�[�v�V�[�P���X����
 */
int pceth2_procEscape(SCRIPT_DATA *s)
{
	static const char *lfWords[] = {"�@", "�u", "�w", "�i"};
	int i;

	s->p++;
	switch (*(s->data + s->p++))	// ��΂�1�i�߂�̂ł�����++���Ƃ�
	{
		case 'k':	// �L�[�҂�
		{
			// ����\n�Ȃ���y�[�W�ɂ��A���ŃL�[�҂��ɂȂ�\��������̂Ő�ɉ��s���Ă���
            if (!strncmp(s->data + s->p, "\\n", 2)) {
				s->p += 2;
				if (!pceth2_isLineTop()) {	// �s���łȂ���Ή��s����
					pceth2_putCR();
					if (pceth2_isPageTop()) {
						play.gameMode = GM_KEYWAIT;
						ld_VBuffUpdate();
						return 0;
					}
				}
			}
			if (!pceth2_isPageTop()) {
				play.gameMode = GM_KEYWAIT;
				ld_VBuffUpdate();
				return 0;
			}
		}
		break;
		case 'n':	// ���s
		{
			if (!pceth2_isPageTop()) {	// �s���łȂ���Ή��s����
				if (pceth2_isKanji(s)) {	// ���ߒ��O�Ȃ���s
					for (i = 0; i < array_size(lfWords); i++) {	// �蓮�֑������̉��s�͖���������
						if (!strncmp(s->data + s->p, lfWords[i], 2)) {
							pceth2_putCR();
							if (pceth2_isPageTop()) {
								play.gameMode = GM_KEYWAIT;
								ld_VBuffUpdate();
								return 0;
							}
							break;
						}
					}
				}
			}
		}
		break;
		case 'p':	// �L�[�҂��y�[�W����
		{
			s->p += 4;	// �y�[�W�Ǘ�ID���΂�
			if (!pceth2_isPageTop()) {				// �y�[�W�擪�łȂ���΃L�[�҂����y�[�W
				pceth2_setPageTop();
				play.gameMode = GM_KEYWAIT;
				ld_VBuffUpdate();
				return 0;
			}
		}
		break;
	}

	return 1;
}


/*
 *	�֑������i�s���K�v������ꍇ1��Ԃ��j
 */
int pceth2_jpnHyphenation(const char *str)
{
	static const char *hypWords[] = {"�B", "�A", "�v", "�x", "�j"};
	int i, x;

	FontFuchi_GetPos(&x, NULL);
	if (x > MSG_X_MAX - (FONT_W + 1)) {
		for (i = 0; i < array_size(hypWords); i++) {
			if (!strncmp(str, hypWords[i], 2)) {
				return 1;
			}
		}
	}

	return 0;
}

/*
 *	P/ECE�œƓ��̉��s����
 */
int pceth2_lineFeed(const char *str)
{
	static const char *start_word[] = {"�@", "�u", "�w"};
	static const char *end_word[] = {"�B", "�v", "�x"};
	int i, j;

	if (play.msglen >= 2 && !pceth2_isLineTop()) {
		for (i = 0; i < array_size(start_word); i++) {
			if (!strncmp(str, start_word[i], 2)) {
				for (j = 0; j < array_size(start_word); j++) {
					if (!strncmp(play.msg + play.msglen - 2, end_word[j], 2)) {
						return 1;
					}
				}
			}
		}
	}

	return 0;
}
