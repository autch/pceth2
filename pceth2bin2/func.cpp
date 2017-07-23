#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define array_size(a)	(sizeof(a)/sizeof(a[0]))
#define isKanji(x)	(((x) >= 0x81 && (x) <= 0x9f) || ((x) >= 0xe0 && (x) <= 0xfc))

/*
 *	���l���擾
 *	�߂�l	�|�C���^��i�߂鐔(6)
 */
int getNumber(const BYTE *ptr, int *num)
{
	if (*ptr != 0x09) return 0;
	*num = (int)(*(float*)(ptr + 2));

	// 020003800.BIN, 070000300.BIN�p�̑΍�i�ꓖ����I
	if (*(ptr + 6) == 0x02 && *(ptr + 7) == 0x02 && *(ptr + 8) == 0x09) {
		return 7;
	}

	return 6;
}

/*
 *	���W�X�^�C���f�b�N�X���擾
 *	�߂�l	�|�C���^��i�߂鐔(2)
 */
int getRegIndex(const BYTE *ptr, int *num)
{
	if (*ptr != 0x06)	return 0;
	*num = (int)(*(ptr + 1));

	return 2;
}

/*
 *	���Z�q���擾
 *	�߂�l	�|�C���^��i�߂鐔(1)
 */
int getOperator(const BYTE *ptr, char *ope)
{
	switch (*ptr) {
	case 0x17:	strcpy(ope, "�{");	break;
	case 0x18:	strcpy(ope, "�|");	break;
	case 0x19:	strcpy(ope, "�~");	break;
	case 0x1A:	strcpy(ope, "��");	break;
	case 0x2A:	strcpy(ope, "��");	break;
	case 0x2B:	strcpy(ope, "��");	break;
	case 0x2C:	strcpy(ope, "��");	break;
	case 0x2D:	strcpy(ope, "��");	break;
	case 0x2E:	strcpy(ope, "��");	break;
	case 0x2F:	strcpy(ope, "��");	break;
	default:	return 0;
	}

	return 1;
}

/*
 *	�t�|�[�����h�L�@�̌v�Z�������߂�
 *	�i���W�X�^�����ނ��̂͋��߂悤���Ȃ��̂Œ��߂�j
 *	�߂�l	�|�C���^��i�߂鐔
 */
int calcRevPolish(const BYTE *ptr, int *num)
{
	int buf[8], i = 0, ret = 0;

	while(*(ptr + ret) != 0x02) {
		switch (*(ptr + ret)) {
		case 0x09:
			ret += getNumber(ptr + ret, &buf[i]);	i++;	break;
		case 0x17:
			ret++;	i--;	buf[i-1] += buf[i];	break;
		case 0x18:
			ret++;	i--;
			if (i >= 1) { buf[i-1] -= buf[i]; }
			else { buf[i] = -buf[i]; } break;
		case 0x19:
			ret++;	i--;	buf[i-1] *= buf[i];	break;
		case 0x1A:
			ret++;	i--;	buf[i-1] /= buf[i];	break;
		case 0x2A:
			ret++;	i--;	buf[i-1] = (buf[i-1] == buf[i]);	break;
		case 0x2B:
			ret++;	i--;	buf[i-1] = (buf[i-1] != buf[i]);	break;
		case 0x2C:
			ret++;	i--;	buf[i-1] = (buf[i-1] <  buf[i]);	break;
		case 0x2D:
			ret++;	i--;	buf[i-1] = (buf[i-1] <= buf[i]);	break;
		case 0x2E:
			ret++;	i--;	buf[i-1] = (buf[i-1] >  buf[i]);	break;
		case 0x2F:
			ret++;	i--;	buf[i-1] = (buf[i-1] >= buf[i]);	break;
		default:
			return 0;
		}
		continue;
	}

	if (ret == 0) {
		return 0;
	}
	*num = buf[0];
	return ret;
}

/*
 *	�t�|�[�����h�L�@�̌v�Z�������̂܂܏o�͂���
 *	�߂�l	�|�C���^��i�߂鐔
 */
int getRevPolish(const BYTE *ptr, char *str)
{
	int num, len, ret = 0;
	char buf[10];
	*str = '\0';

	while(*(ptr + ret) != 0x02) {
		if (len = getNumber(ptr + ret, &num)) {
			sprintf(buf, " %d", num);
			strcat(str, buf);
			ret += len;
		} else if (len = getRegIndex(ptr + ret, &num)) {
			sprintf(buf, "$%d", num);
			strcat(str, buf);
			ret += len;
		} else if (len = getOperator(ptr + ret, buf)) {
			strcat(str, buf);
			ret += len;
		} else {
			return 0;
		}
	}

	return ret;
}

/*
 *	��������擾
 *	�߂�l	�|�C���^��i�߂鐔
 */
int getString(const BYTE *ptr, char *str)
{
	if (*ptr != 0x0A) return 0;

	short len = *((short*)(ptr + 1));

	int i;
	for (i = 0; i < len; i++) {
		if (*(ptr + 3 + i) == 0xF0) {	// ���ꕶ��
			switch (*(ptr + 3 + i + 1)) {
			case 0x40:	memcpy(str + i, "!?", 2);	break;
			case 0x41:	memcpy(str + i, "!!", 2);	break;
			case 0x42:	// �n�[�g�}�[�N�F�X�N���v�^�őΉ�����̂ł��̂܂܏o��
				memcpy(str + i, ptr + 3 + i, 2);
				*(str + i + 2) = '\0';
				break;
			case 0x46:
			case 0x47:
                memcpy(str + i, "�`", 2);
				break;
			}
			i++;
		} else if (isKanji(*(ptr + 3 + i))) {
			memcpy(str + i, ptr + 3 + i, 2);
			i++;
		} else {
			*(str + i) = *(ptr + 3 + i);
		}
	}
	*(str + i) = '\0';

	return len + 3;	// �i�߂�o�C�g��
}

/*
 *	���x�����擾
 *	�߂�l	�|�C���^��i�߂鐔
 */
int getLabel(const BYTE *ptr, char *str)
{
	if (*ptr != 0x05) return 0;

	char len = *(ptr + 1);
	memcpy(str, ptr + 2, len);
	*(str + len) = '\0';

	return len + 2;
}

/*
 *	�K�v�Ȃ��i�󂶂�Ȃ����ǁj�������X�L�b�v
 */
int skipScript(const BYTE *ptr)
{
	int ret = 0, len, num;
	char str[100];

	while(*(ptr + ret) != 0x02) {	// �R���}
		if (len = getString(ptr + ret, str)) {
			ret += len + 1;
		} else if (len = getLabel(ptr + ret, str)) {
			ret += len + 1;
		} else if (len = calcRevPolish(ptr + ret, &num)) {
			ret += len + 1;
		} else if (len = getRevPolish(ptr + ret, str)) {
			ret += len + 1;
		}
	}

//	if (*(ptr + ret + 1) == 0x02) { ret++; }

	return ret;
}

/*
 *	BG�̍��̍炫��𓝈�iP/ECE���ōĕϊ�����j
 */
int convertBGNum(int num)
{
	switch (num) {
	case  1:	case  2:	case  3:	case  4:	case 78:	num = 88;	break;	// �Z��O
	case  5:	case  6:	case  7:	case  8:	case 79:	num = 89;	break;	// �Z���
	case 34:	case 35:	case 36:	case 37:	case 80:	num = 90;	break;	// ���̏�
	case 48:	case 49:	case 50:	case 51:	case 81:	num = 91;	break;	// �쉈��
	}

	return num;
}

int manaka_count; // �u���q�v�u�����v�ǂݑւ���

/*
 *	���O�u�������𖼑O�ɒu��
 */
void replaceName(char *str)
{
	/* �u���e�[�u���i�O����v�Ȃ̂Œu�������������̂��珑�����Ɓj */
	static const struct {
		char *src;	// �u����
		char *dst;	// �u����
	} name_table[] = {
						{"*nlk1",	"��"},			// ���i�Ђ炪�ȁj1������
						{"*nlk",	"������"},		// ���i�Ђ炪�ȁj
						{"*nl1",	"��"},			// ��1������
						{"*nl",		"�͖�"},		// ��
						{"*nfk1",	"��"},			// ���i�Ђ炪�ȁj1������
						{"*nfk2",	"��"},			// ���i�Ђ炪�ȁj2������
						{"*nfk3",	"��"},			// ���i�Ђ炪�ȁj3������
						{"*nfk4",	"��"},			// ���i�Ђ炪�ȁj4������
						{"*nfk",	"��������"},	// ���i�Ђ炪�ȁj
						{"*nf",		"�M��"},		// ��
						{"*nnk1",	"�^"},			// �j�b�N�l�[���i�J�^�J�i�j1������
						{"*nnk",	"�^�J"},		// �j�b�N�l�[���i�J�^�J�i�j
						{"*nn1",	"��"},			// �@�@�@�@�@�@�i�Ђ炪�ȁj1������
						{"*nn",		"����"},		// �@�@�@�@�@�@�i�Ђ炪�ȁj
	};
	char tmp[1000];
	*tmp = '\0';

	// 1���������S�~�������Ă��珜��
	if (*(str+1) == '\0') {
		if (*str != '<' && *str != '>') {
			*str = '\0';
		}
		return;
	}

	int len = 0;
	while (*(str+len) != '\0') {
		if (*(str+len) == '*') {
			if (!strncmp(str+len, "*h2", 3)) {		// �����̌Ăѕ�
				// ToHeart2 XRATED �\�[�X�R�[�h���A�X�N���v�g���ŋ����Ɏ����B
				// ���W�X�^�A�W�����v���x���͕��i�g���Ă��Ȃ��Ǝv������̂��g�p�B
				// ���b�Z�[�W�\�����Ƀ��W�X�^�ɗL���Ȓl�������Ă��邱�Ƃ͂Ȃ��͂��B
				char buf[38];
				sprintf(buf, "l213,5b$5 0��,8%02d����j9%02d@8%02d���q@9%02d", manaka_count, manaka_count, manaka_count, manaka_count);
				manaka_count++;
				strcat(tmp, buf);
				len += 3;
				continue;
			}
			for (int i = 0; i < array_size(name_table); i++) {
				if (!strncmp(str+len, name_table[i].src, strlen(name_table[i].src))) {
					strcat(tmp, name_table[i].dst);
					len += (int)strlen(name_table[i].src);
					break;
				}
			}
			continue;
		}
		if (isKanji(*(str+len))) {
			sprintf(tmp + strlen(tmp), "%c", *(str+len));
			len++;
		}
		sprintf(tmp + strlen(tmp), "%c", *(str+len));
		len++;
	}

	strcpy(str, tmp);
}









