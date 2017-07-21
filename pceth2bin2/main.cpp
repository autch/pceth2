/*
 *	pceth2bin2
 *	ToHeart2���甲���o����BIN�t�@�C����ϊ�
 *
 *	(c)2005 �ĂƂ灚�ۂ���
 *
 *	2005/05/24	�V�K�J���B�I�v�V�����ŉ�͂ƕϊ���؂�ւ�����悤�ɂ��܂��B
 *	2005/05/25	��̓��[�h����
 *	2005/05/26	�X�N���v�g�ϊ����[�h��pceth2bin�ɐ�����ǉ��������x���܂Ŋ���
 *	2005/05/26	0x03�`0x06������
 *	2005/05/30	�����G�̑O��\���ʒu���L�����Ă����A�����w�肪�Ȃ��ꍇ������g�p
 *	2005/06/01	�����G���������ꂽ�Ƃ��ɁA�O��\���ʒu�𒆉��Ƀ��Z�b�g
 *	2005/06/08	0x4D�̈����ǂݍ��݃~�X���C��
 *	2005/06/10	�X�N���v�g�ϊ����[�h��0x62�̏������󂾂����̂ŃR�����g�A�E�g�G
 *				�ꕔ�X�N���v�g�ŃR���}�p0x02�̐��������̂��ꓖ����I�ɏC��
 *	2005/06/12	�ϊ���̃��x������2��3����
 *	2005/06/13	�s�������Ԉ���Ă����̂��C��
 *	2005/06/14	��̓��[�h�ł͋t�|�[�����h�v�Z�����v�Z���Ă��܂�Ȃ��悤��
 *				06/10�̏ꓖ����I�ȏC�����蔲���������̂�⊮
 *				�w�i�摜�̎��ԑсA�V��ɂ��ω�������
 *	2005/06/15	�E�F�C�g����
 *	2005/06/17	0x32�����B�����G�\���A�����̑������������t���O������
 *				�����G���������ꂽ�Ƃ��̑O��\���ʒu���ABG���������ŏ������ꂽ�Ƃ����s��
 *	2005/06/18	�����G�\���A�����ɃX���C�h�C���A�A�E�g��ǉ�
 *	2005/06/20	�t�|�[�����h�v�Z��-1�̏ꍇ�f�t�H���g�l��Ԃ�������
 *	2005/06/23	BG�̍��̍炫��𓝈�iP/ECE���ōĕϊ�����j
 *	2005/06/29	�����G�\���ʒu�̎����ݒ�̏�����������
 *	2005/06/30	0x4F��SE��~�ۂ��̂Œǉ�
 *				�ꕔ�X�N���v�g�Ŗ��ߏI�[�����̖��ߊJ�n��0x02�̐��������̂��C��
 *	2005/07/19	BG�\�����߂ō�����t�ɍ��킹�Ȃ��P�[�X�i��z�Ȃǁj�ɑΉ��i��2������10000�����j
 *				0x79�i�J�����_�[�j���ߒǉ�
 *	2005/07/20	�����G�\���ʒu�̎����ݒ�A���x���������Ă邩��
 *				�\��������̃S�~�i8,&�j������
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

#define	DUMMY_SIZE	239

#define array_size(a)	(sizeof(a)/sizeof(a[0]))

typedef struct tagBIN_HEADER {
	WORD	head;		// "VE"
	WORD	page_start;	// �J�n�y�[�W�ԍ�
	long	size;		// �w�b�_�E���x���������T�C�Y
	long	label_num;	// ���x����
	WORD	page_end;	// �I���y�[�W�ԍ�
	WORD	reserved;	// 00 00
} BIN_HEADER;

typedef struct tagBIN_LABEL {
	char	name[32];	// ���x���i@�ŊJ�n�j
	DWORD	addr;		// �W�����v��iDWORD�j�w�b�_���������ʒu�ł���
} BIN_LABEL;

int main(int argc, char *argv[])
{
	FILE	*fpin = NULL, *fpout = NULL;
	char	output_flag = 'a';
	char	path[_MAX_PATH];
	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];
	BIN_HEADER	bh;
	BIN_LABEL	*bl;
	BYTE	*buf;

	BYTE	code;
	int		num[10];
	char	str[100];
	int		len;
	int		pos[28];	// �����G�\���ʒu�i�ʒu�ȗ��̂��̂͑O��Ɠ����ʒu�Ƃ݂Ȃ��j

	if (argc == 1) {	// usage
		fprintf(stderr, "ToHeart2 BIN�t�@�C���R���o�[�^\n");
		fprintf(stderr, "pceth2bin2 [option] filename ..\n");
		fprintf(stderr, "-s �X�N���v�g�o��(.scp)\n");
		fprintf(stderr, "-a ��͏o��(.txt)�f�t�H���g\n");
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "-a")) {	// �Ȍ�̃t�@�C���͉�͏o��
			output_flag = 'a';
			continue;
		}
		if (!strcmp(argv[i], "-s")) {	// �Ȍ�̃t�@�C���̓X�N���v�g�o��
			output_flag = 's';
			continue;
		}
		if (!strcmp(argv[i], "-d")) {	// �Ȍ�̃t�@�C���̓X�N���v�g�o�́i�f�o�b�O���[�h�j
			output_flag = 'd';
			continue;
		}

		if ((fpin = fopen(argv[i], "rb")) == NULL)	goto ERR;

		_splitpath(argv[i], drive, dir, fname, ext);
		if (output_flag == 'a') {
			_makepath(path, drive, dir, fname, ".txt");
		} else {
			_makepath(path, drive, dir, fname, ".scp");
		}
		if((fpout = fopen(path, "wt")) == NULL)	goto ERR;

		// BIN_HEADER
		fread(&bh, 1, sizeof(BIN_HEADER), fpin);
		if (strncmp((char*)&bh.head, "VE", 2)) goto ERR;
		// BIN_LABEL
		if ((bl = (BIN_LABEL *)malloc(sizeof(BIN_LABEL) * bh.label_num)) == NULL)	goto ERR;
		for (int j = 0; j < bh.label_num; j++) {
			fread(&bl[j], 1, sizeof(BIN_LABEL), fpin);
		}
		// ��̃f�[�^
		fseek(fpin, DUMMY_SIZE, SEEK_CUR);
		bh.size -= DUMMY_SIZE;
		// �ǂݍ���
		if ((buf = (BYTE*)malloc(bh.size)) == NULL)	goto ERR;
		fread(buf, 1, bh.size, fpin);
		fclose(fpin);

		for (int j = 0; j < array_size(pos); j++) { pos[j] = 1; }	// �����G�\���ʒu������
		manaka_count = 0;

		for (int j = 0; j < bh.size; ) {
			if (*(buf + j + 1) == 0x02)	j++;		// 0x02��1�����ꍇ1�X�L�b�v
			if (*(buf + j++) != 0x02)	goto ERR;	// 0x02

			for (int k = 0; k < bh.label_num; k++) {	// ���x���}��
				if (j + DUMMY_SIZE == bl[k].addr) {
					if (output_flag == 'a') {
						fprintf(fpout, "%s\n", bl[k].name);	// ��̓��[�h
					} else {
						fprintf(fpout, "@%03d",k);			// �X�N���v�g
					}
				}
			}

			switch(*(buf + j++)) {
			case 0x08:	// ����
			{
				code = *(buf + j++);
				for (int k = 0; k < 10; k++) { num[k] = 0; }

				if (output_flag == 'a') {	// ��̓��[�h�ł͂��̂܂܏����o���܂�
					fprintf(fpout, "%02x", code);
					if (code != 0x20) {	// ���̖��߂̂ݖ�����0x02������
						do {
							fprintf(fpout, ",");
							while(*(buf + j) != 0x02) {	// �R���}
								if (len = getString(buf + j, str)) {
									fprintf(fpout, " %s", str);
									j += len;
								} else if (len = getLabel(buf + j, str)) {
									fprintf(fpout, " %s", str);
									j += len;
/*								} else if (len = calcRevPolish(buf + j, &num[0])) {
									fprintf(fpout, " %d", num[0]);
									j += len;
*/								} else if (len = getRevPolish(buf + j, str)) {
									fprintf(fpout, "%s", str);
									j += len;
								} else {
									goto ERR;
								}
							}
							j++;
						} while (*(buf + j) != 0x02);	// ���̐���̋N�_�ɓ��B����܂�
					}
					fprintf(fpout, "\n");
				} else {
					if (output_flag == 'd') {	// �f�o�b�O�o��
						printf("%02x\n", code);
					}
					switch (code) {

					case 0x03:	// reg[a]��b�����[�h
//					case 0x04:	// reg[a]�ɐ���b�����[�h�i����g���Ă��d�������Ȃ̂ŏȗ��j
						j += getNumber(buf + j, &num[0]) + 1;
						j += getRevPolish(buf + j, str) + 1;
                        fprintf(fpout, "=%d,%s", num[0], str);
						break;
					case 0x05:	// reg[a]���C���N�������g
						j += getNumber(buf + j, &num[0]) + 1;
                        fprintf(fpout, "+%d", num[0]);
						break;
					case 0x06:	// reg[a]���f�N�������g
						j += getNumber(buf + j, &num[0]) + 1;
                        fprintf(fpout, "-%d", num[0]);
						break;
					case 0x19:	// �E�F�C�g�i33ms�P�ʁ�10ms�P�ʂɕϊ��j
						j += calcRevPolish(buf + j, &num[0]) + 1;
						fprintf(fpout, "w%03d", num[0] * 1000 / 30 / 10);
						break;
					case 0x1D:	// flag[a]��reg[b]�Ƀ��[�h(num a, num b)
						for (int k = 0; k < 2; k++) { j += calcRevPolish(buf + j, &num[k]) + 1; }
						fprintf(fpout, "l%d,%d", num[0], num[1]);
						break;
					case 0x1E:	// flag[a]�ɐ���b���X�g�A(num a, exp b)
						j += calcRevPolish(buf + j, &num[0]) + 1;
						j += getRevPolish(buf + j, str) + 1;
						fprintf(fpout, "s%d,%s", num[0], str);
						break;
					case 0x1F:	// �X�N���v�ga.scp�ɃW�����v(str a)
						j += getString(buf + j, str) + 1;
						fprintf(fpout, "J%s.scp", str);
						break;
					case 0x20:	// ���ǃy�[�W�̊Ǘ��H
						break;	// ���̖��߂̂ݖ�����0x02������

					case 0x22:	// �I����
						j += getString(buf + j, str) + 1;
						fprintf(fpout, "q%s ", str);
						break;
					case 0x23:	// reg[a]�Ɍ��ʂ�������I���X�^�[�g
						j += getRegIndex(buf + j, &num[0]) + 1;
						fprintf(fpout, "Q%d", num[0]);
						break;

					case 0x27:	// �w�i�摜B yyy zzz .pgx�̕\���i�����G�������j
					case 0x28:	// �i�����G�͂��̂܂܁j
					case 0x29:	// 0x27�ƈꏏ�H
					case 0x2A:	// 0x28�ƈꏏ�H
						for (int k = 0; k < 3; k++) {
							if (len = calcRevPolish(buf + j, &num[k])) { j += len + 1; }
						}
						if (num[1] >= 0) { // 0x6A �̌�ɏo�Ă��邱�Ƃ����� -1 �͖�������
							num[1] = convertBGNum(num[1]);	// ������
//**					if (!(code == 0x27 && num[0] == 0)) {	// ����̓��C���A�j����������񂾂�����\�����Ȃ������H
/*							if (code & 1) {	// �����G��������Ȃ�ʒu������
								for (int q = 0; q < array_size(pos); q++) { pos[q] = 1; }
							}
							// 10000������Ă�ƍ��w�i�����t�������ĕ\������炵���i��z�Ȃǁj
*/							fprintf(fpout, "B%03d%03d.pgx,%1d", num[1] % 1000, num[2], (code - 0x27) % 2);
						}
						break;
/*					case 0x2E:	// �����G�摜C xxx yyyyy .pgx�̕\���i�ʒuz�j�i�����ɕ`���j
					case 0x2F:	// �i�����ɂ͕`���Ȃ��j��P/ECE�ł͕ς��Ȃ��̂ŋ��ʂɂ��܂���
								// ���� 4:�i1��,2�E�j����X���C�h�C���i0,3�͖����j
								//		5:�D��x�i���R��L��C�̏��ŕ`���܂��A�����j
								//		8:�X���C�h�C���̃t���[�����x
						num[2] = 1;
						for (int k = 0; k < 8; k++) {
							if (len = calcRevPolish(buf + j, &num[k])) { j += len + 1; }
						}
						if (num[2] > 2) { num[2] = ((num[2] % 2)? 0 : 2); }	// 3�ȏ�͊���A�����E�i�{���͈ʒu�Ⴄ���ǖ����j
//						if (num[2] == 1) { num[2] = pos[num[0]]; }	// �O��̕\���ʒu�ɂ���
//						pos[num[0]] = num[2];	// �\���ʒu���L��
						if (code == 0x2E && num[3] == 0) {
							num[2] += 5;	// �����G�\���ʒu�����ݒ�̏����H
						}
						fprintf(fpout, "C%03d%05d.pgx,%1d", num[0], num[1], num[2]);
						if (code == 0x2E && (num[3] == 1 || num[3] == 2)) {
							fprintf(fpout, ",%d", num[3]);	// �X���C�h
						}
						break;
*/
					case 0x2E:
						num[2] = 1;	// �f�t�H���g�l�F����
						for (int k = 0; k < 8; k++) {
							if (len = calcRevPolish(buf + j, &num[k])) { j += len + 1; }
						}
						if (num[2] > 2) { num[2] = ((num[2] % 2)? 0 : 2); }	// 3�ȏ�͊���A�����E�i�{���͈ʒu�Ⴄ���ǖ����j
						if (num[3] == 0) {
							num[2] += 5;	// �����G�\���ʒu�����ݒ�̏����H
						}
						fprintf(fpout, "C%03d%05d.pgx,%1d", num[0], num[1], num[2]);
						if (num[3] == 1 || num[3] == 2) {
							fprintf(fpout, ",%d", num[3]);	// �X���C�h
						}
						break;

					case 0x2F:
						num[2] = -1;
						for (int k = 0; k < 8; k++) {
							if (len = calcRevPolish(buf + j, &num[k])) { j += len + 1; }
						}
						if (num[2] > 2) { num[2] = ((num[2] % 2)? 0 : 2); }	// 3�ȏ�͊���A�����E�i�{���͈ʒu�Ⴄ���ǖ����j
						if (num[2] < 0) { num[2] = 6; }	// �f�t�H���g�l�F�����������ʒu�ݒ�
						fprintf(fpout, "C%03d%05d.pgx,%1d", num[0], num[1], num[2]);
						break;

					case 0x30:	// �L�����ԍ��i�����G�摜��xxx�jx�̗����G�������i�����ɏ����j
					case 0x31:	// �i�����ɂ͏����Ȃ��j��P/ECE�ł͕ς��Ȃ��̂ŋ��ʂɂ��܂���
								// ���� 2:�i1��,2�E�j�ɃX���C�h�A�E�g
								//		3:�X���C�h�A�E�g�̃t���[�����x 
						for (int k = 0; k < 3; k++) {
							if (len = calcRevPolish(buf + j, &num[k])) { j += len + 1; }
						}
						pos[num[0]] = 1;	// �\���ʒu�����Z�b�g
						fprintf(fpout, "c%03d", num[0]);
						if (num[1] == 1 || num[1] == 2) {
							fprintf(fpout, ",%d", num[1]);	// �X���C�h
						}
						break;
					case 0x32:	// ���ɕ\������Ă���L�����̉摜�������ւ��i�摜�\���őΉ��j
						for (int k = 0; k < 2; k++) {
							if (len = calcRevPolish(buf + j, &num[k])) { j += len + 1; }
						}
						fprintf(fpout, "C%03d%05d.pgx,6", num[0], num[1]);	// ��{�����̗����ʒu�����ݒ�
						break;
					case 0x38:	// �C�x���g�摜V yyyyy z .pgx�̕\��
					case 0x39:
						for(int k = 0; k < 3; k++) {
							if (len = calcRevPolish(buf + j, &num[k])) { j += len + 1; }
						}
						for (int q = 0; q < array_size(pos); q++) { pos[q] = 1; }	// �����G��������̂ňʒu������
						fprintf(fpout, "V%05d%01d.pgx,0", num[1], num[2]);
						break;
					case 0x46:	// BGM���t
						j += calcRevPolish(buf + j, &num[0]) + 1;
						fprintf(fpout, "M%02d.pmd", num[0]);
						break;
					case 0x47:	// BGM��~
						fprintf(fpout, "Mxx.pmd");
						break;
					case 0x4C:	// SE�i�P���j
						j += getNumber(buf + j, &num[0]) + 1;
						fprintf(fpout, "SE_%04d.ppd,0", num[0]);
						break;
					case 0x4D:	// SE�i���s�[�g����j
						for(int k = 0; k < 4; k++) {
							if (len = calcRevPolish(buf + j, &num[k])) { j += len + 1; }
						}
						fprintf(fpout, "SE_%04d.ppd,%1d", num[1], num[3]);
						break;
					case 0x4E:	// SE��~
					case 0x4F:
						fprintf(fpout, "SE_xxxx.ppd,0");	// �v�͓ǂݍ��ݎ��s��SE�o�b�t�@����ɂ���
						break;
					case 0x5F:	// �w�i�摜�̎��ԑюw��i0�`3�j
						j += calcRevPolish(buf + j, &num[0]) + 1;
						fprintf(fpout, "G%d", num[0]);
						break;
					case 0x60:	// �w�i�摜�̓V��w��i0�`1�j
						j += calcRevPolish(buf + j, &num[0]) + 1;
						fprintf(fpout, "G%d", num[0] + 5);	// �������߂ł�肭�肷�邽�߁i5�ȏ�Ȃ�V��ω��j
						break;
					case 0x61:	// ���x��@$�Ƀu�����`
						j += getRevPolish(buf + j, str) + 1;
						fprintf(fpout, "b%s", str);
						j += getLabel(buf + j, str) + 1;
						for (int k = 0; k < bh.label_num; k++) {
							if (!strcmp(str, bl[k].name)) { fprintf(fpout, ",%03d", k); }
						}
						break;
//					case 0x62:	// �u�����`�����ǁA������������Ȃ���V�i���I����ɂ͎g���Ă��Ȃ��̂ŃX���[�i��
//						break;
					case 0x63:	// ���x��@$�ɃW�����v
						j += getLabel(buf + j, str) + 1;
						for (int k = 0; k < bh.label_num; k++) {
							if (!strcmp(str, bl[k].name)) { fprintf(fpout, "j%03d", k); }
						}
						break;
					case 0x6A:	// �A�j���[�V�����i���W�Ɏg���Ă��鉉�Z���� 0x04 ���X�L�b�v���Ă���̂ŁA�P���ɕ\�����邾���j
						for (int k = 0; k < 2; k++) { j += getNumber(buf + j, &num[k]) + 1; }
						j += getString(buf + j, str) + 1;
						*str = toupper(*str);
						if (*str != 'F') { // ���ʂ͎g�p���Ȃ�
							*strchr(str, '.') = '\0';
							fprintf(fpout, "%s.pgx,0", str);
						}
						break;
					case 0x74:	// �}�b�v�ړ��I������ǉ�
						for(int k = 0; k < 3; k++) { j += getNumber(buf + j, &num[k]) + 1; }
						j += getString(buf + j, str) + 1;
						fprintf(fpout, "m%d,%d%d,%s.scp", num[1], num[0], num[2], str);
						break;
					case 0x76:	// ���[�r�[�Đ��i�Ȃ񂾂��ǁA�G�s���[�O�ɍs���i���������j����Ɏg���j
						fprintf(fpout, "z");
						break;
					case 0x77:	// �^�C�g���ɖ߂�
						j++;	// ������
						fprintf(fpout, "Z");
						break;
					case 0x78:	// ���v��\���i�Ƃ肠���������G�Ɠ�����@�ł����܂���[�j
						j += getNumber(buf + j, &num[0]) + 1;
						fprintf(fpout, "CLOCK%02d.pgx,1", num[0]);	// �X�N���v�g�g�p�ŋ����ɏ������邗
						fprintf(fpout, "w100cLOC");
						break;
					case 0x79:	// �X�N���v�g���ŃJ�����_�[�i��[���V�i���I�Ŏg�p
						for (int k = 0; k < 2; k++) {
							j += getNumber(buf + j, &num[k]) + 1;
						}
						fprintf(fpout, "D%d,%d", num[0], num[1]);
						break;
					}
					j += skipScript(buf + j);
				}
				break;
			}
			case 0x0B:	// ���b�Z�[�W
			{
				char msg[1000];
				if(*(buf + j++) != 0x00)	goto ERR;	// 0x00
				j += getString(buf + j, msg);
				replaceName(msg);
				fprintf(fpout, "%s", msg);
				if (output_flag == 'a') {
					fprintf(fpout, "\n");
				}
				break;
			}
			case 0x00:	// �I�[
				break;
			default:
				goto ERR;
			}
		}

		printf("%s - �������܂����B\n", argv[i]);
		goto FREE;
ERR:
		fprintf(stderr, "%s - ���s���܂����B\n", argv[i]);
FREE:
		if(fpin  != NULL)	fclose(fpin);
		if(fpout != NULL)	fclose(fpout);
		if(buf   != NULL)	free(buf);
	}

	return 0;
}



