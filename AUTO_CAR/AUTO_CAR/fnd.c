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

// fnd0에 문자 하나 출력
void fnd_display_0(int c)
{
	FND_DIGIT_PORT |= 1 << FND_DIGIT_D1;
	FND_DATA_PORT = c;
}