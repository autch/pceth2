#ifndef __FONT_EX_H_
#define __FONT_EX_H_

typedef struct SFONT {
	int x;							//�\���ʒu�����W
	int y;							//�\���ʒu�c���W
	int xMin;						//�\���͈͉����ŏ��l
	int xMax;						//�\���͈͉����ő�l
	int yMin;						//�\���͈͏c���ŏ��l
	int yMax;						//�\���͈͏c���ő�l
	int spr;						//�����F/�w�i�F/����F/����
									/*
									0:��    /��    /�Ȃ�  /�Ȃ�
									1:��    /��    /�Ȃ�  /����
									2:��    /��    /�Ȃ�  /�Ȃ�
									3:��    /��    /�Ȃ�  /����
									5:��    /��    /��    /����
									7:��    /��    /��    /����
									*/
} SFONT;

extern SFONT sFontStatus;

void hook_FontGetAdrs();
void unhook_FontGetAdrs();

unsigned short sFontPut( int x, int y, unsigned short code );
char *sFontPutStr(const char *p);
int sFontPrintf(const char* fmt, ...);

#endif
