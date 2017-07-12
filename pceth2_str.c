/*
 *	pceth2 - �����񏈗��֘A
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/07/23	pceth2_sys.c, common.h���番��
 */

#include <string.h>
#include <piece.h>

#include "common.h"
#include "pceth2_str.h"

/*
 *	2�o�C�g�������ǂ���
 *	*s		�X�N���v�g�f�[�^
 *	return	TRUE/FALSE
 */
#define isKanji(x)	(((x) >= 0x81 && (x) <= 0x9f) || ((x) >= 0xe0 && (x) <= 0xfc))

BOOL pceth2_isKanji(SCRIPT_DATA *s)
{
	return(isKanji(*(s->data + s->p)));
}

/*
 *	���l���擾
 *	*s		�X�N���v�g�f�[�^
 *	return	�擾�������l
 */
#define isDigit(x)	((x) >= '0' && (x) <= '9')

int pceth2_getNum(SCRIPT_DATA *s)
{
	int ret = 0;

	for(; isDigit(*(s->data + s->p)); s->p++) {
		ret *= 10;
		ret += *(s->data + s->p) - '0';
	}

	return ret;
}

/*
 *	�X�N���v�g�f�[�^���當������R�s�[
 *
 *	*dst	�R�s�[��̃|�C���^
 *	*s		�X�N���v�g�f�[�^
 *	len		�R�s�[������
 */
void pceth2_strcpy(char *dst, SCRIPT_DATA *s, DWORD len)
{
	strncpy(dst, s->data + s->p, len);
	*(dst + len) = '\0';
	s->p += len;
}
