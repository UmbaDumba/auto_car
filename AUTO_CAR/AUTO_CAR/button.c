/*
 * button.c
 *
 * Created: 2025-03-05 오후 2:30:27
 *  Author: microsoft
 */ 
#include "button.h"
 

                                                
void init_button(void)
{
	BUTTON_DDR &= ~( 1 << BUTTON0PIN);
}

int get_button(int button_num, int button_pin)
{
	static unsigned char button_status[BUTTON_NUMER] =
	{BUTTON_RELEASE};
	// 	지역 변수에 static을 쓰면 전역 변수처럼 함수를 빠져 나갔다 다시 들어 와도 값을 유지 한다.  
	int currtn_state;
	
	currtn_state = BUTTON_PIN & (1 << button_pin);   // 버튼을 읽는다. 
	if (currtn_state && button_status[button_num] == BUTTON_RELEASE)  // 버튼이 처음 눌려진 noise high 
	{
		_delay_ms(60);   // noise가 지나가기를 기다린다. 
		button_status[button_num] = BUTTON_PRESS;   // noise가 지나간 상태의 High 상태 
		return 0;   // 아직은 완전히 눌렸다 떼어진 상태가 아니다. 
	}
	else if (currtn_state==BUTTON_RELEASE && button_status[button_num] == BUTTON_PRESS)
	{
		_delay_ms(60);
		button_status[button_num] = BUTTON_RELEASE;   // 다음 버튼 체크를 위해서 초기화
		return 1;   // 완전히 1번 눌렸다 떼어진 상태로 인정		
	}
	
	return 0;   // 버튼이 open상태 
}