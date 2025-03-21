/*
 * fnd.c
 *
 * Created: 2025-03-06 오후 12:23:41
 *  Author: microsoft
 */ 
#include "fnd.h"

void init_fnd(void)
{
	FND_DATA_DDR = 0xff;  // 출력 모드로 설정
	
	// FND_DIGIT_DDR |= 0xf0;   // 자릿수 선택 7654 
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4; 
	
	// fnd를 all off 
	FND_DATA_PORT = ~0x00;
}

void fnd_clear_digit(void)
{
	FND_DIGIT_PORT &= ~(1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4);

}

// fnd0에 문자 하나 출력
void fnd_display_0(int c)
{
	FND_DIGIT_PORT |= 1 << FND_DIGIT_D1;
	FND_DATA_PORT = c;
}

// fnd0123에 입력받은 배열 그대로 출력
void fnd_display_all(int *fnd_font)
{
	static int i = 0;
	
	fnd_clear_digit();
	switch(i)
	{
		case 0:
			FND_DIGIT_PORT |= 1 << FND_DIGIT_D1;
			FND_DATA_PORT = fnd_font[0];
			break;
		case 1:
			FND_DIGIT_PORT |= 1 << FND_DIGIT_D2;
			FND_DATA_PORT = fnd_font[1];
			break;
		case 2:
			FND_DIGIT_PORT |= 1 << FND_DIGIT_D3;
			FND_DATA_PORT = fnd_font[2];
			break;
		case 3:
			FND_DIGIT_PORT |= 1 << FND_DIGIT_D4;
			FND_DATA_PORT = fnd_font[3];
			break;
	}
	i++;
	i %= 4;
}