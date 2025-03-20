

#define  F_CPU 16000000UL  // 16MHZ

#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdio.h>		// printf

#include "pwm.h"

volatile int msec_count = 0;

void init_timer0(void);

typedef struct car_info{
	int mode; // 수동모드 0, 자동모드 1
	int state; // 지금 자동차 상태
					// 직진, 후진, 좌회전, 우회전, 멈춤
	int speed; // 자동차 속도
	// todo		// 틀어 줄 노래 (부저 출력노래 결정)
	// todo		// LCD 문구 (출력할것)
	// todo		// FND 문구 (출력할것)
		
}t_car_info;

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; // 6 ~ 256으로 돌리기 위해
	msec_count++;
}

int main(void)
{
	// TODO
    // 각종 초기화
	init_timer0();
	init_timer1();
	init_L298N();
	sei();
	
    while (1) 
    {
		// TODO
		// 바퀴 굴리기
		// LED 제어하기
		// 초음파센서로 앞 물체 파악하기
		// LCD 제어하기
		// FND 제어하기
		// 부저 제어하기
		// 블루투스 커멘드 처리
		
		if(msec_count > 4000)
		{
			msec_count = 0;
		}else if(msec_count > 3000)
		{
			motor_turn_left(500);
		}else if(msec_count > 2000)
		{
			motor_turn_right(500);
		}else if(msec_count > 1000)
		{
			motor_direction_backward(500);
		}else
		{
			motor_direction_forward(500);
		}
		
    }
}

void init_timer0(void)
{
	TCNT0 = 6;
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	TIMSK |= 1 << TOIE0;
}