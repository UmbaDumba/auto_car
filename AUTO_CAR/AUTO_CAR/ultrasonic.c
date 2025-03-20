/*
 * ultrasonic.c
 *
 * Created: 2025-03-12 오후 2:49:18
 *  Author: microsoft
 */ 

#include "ultrasonic.h"

volatile int obstacle_info[] = {0, 0, 0}; // left, center, right
								// 기준거리 이내에 있으면 1, 없으면 0
volatile int check_distance_index = 0;
volatile int distance_checking = 0;

volatile int ultrasonic_dis = 0;
volatile int tcnt_value = 0;

volatile int overflows = 0;
int overflows_value = 0;

void (*triger_funcs[]) (void) = {
	trigger_ultrasonic_left,
	trigger_ultrasonic_center,
	trigger_ultrasonic_right
};

ISR(TIMER2_OVF_vect)
{
	overflows++;
}

// left
ISR(INT4_vect)
{
	static int state = 1;
	// 1. rising edge -> 지금 echo는 HIGH
	if(state)
	{
		TCNT2 = 0;
		state = 0;
		overflows = 0;
	}
	// 2. falling edge -> 지금 echo는 LOW
	else
	{
		tcnt_value = TCNT2;
		overflows_value = overflows;
		ultrasonic_dis = (tcnt_value + overflows_value * 256.0) * 128.0 / 16;
		ultrasonic_dis /= 58;
		
		if(ultrasonic_dis <= MAX_LENGHT){
			obstacle_info[0] = 1;
		}else{
			obstacle_info[0] = 0;
		}
		
		state = 1;
		check_distance_index = 1;
		distance_checking = 0;
	}
}

// center
ISR(INT5_vect)
{
	static int state = 1;
	if(state)
	{
		TCNT2 = 0;
		state = 0;
		overflows = 0;
	}
	else
	{
		tcnt_value = TCNT2;
		overflows_value = overflows;
		ultrasonic_dis = (tcnt_value + overflows_value * 256.0) * 128.0 / 16;
		ultrasonic_dis /= 58;
		
		if(ultrasonic_dis <= MAX_LENGHT){
			obstacle_info[1] = 1;
		}else{
			obstacle_info[1] = 0;
		}
		
		state = 1;
		check_distance_index = 2;
		distance_checking = 0;
	}
}

// right
ISR(INT6_vect)
{
	static int state = 1;
	if(state)
	{
		TCNT2 = 0;
		state = 0;
		overflows = 0;
	}
	else
	{
		tcnt_value = TCNT2;
		overflows_value = overflows;
		ultrasonic_dis = (tcnt_value + overflows_value * 256.0) * 128.0 / 16;
		ultrasonic_dis /= 10;
		
		if(ultrasonic_dis <= MAX_LENGHT){
			obstacle_info[2] = 1;
			}else{
			obstacle_info[2] = 0;
		}
		
		state = 1;
		check_distance_index = 0;
		distance_checking = 0;
	}
}


void init_ultrasonic(void)
{
	// ddr
	TRIG_DDR |= 1 << TRIG_LEFT_PIN_NUM | 1 << TRIG_CENTER_PIN_NUM | 1 << TRIG_RIGHT_PIN_NUM;
	ECHO_DDR &= ~(1 << ECHO_LEFT_PIN_NUM | 1 << ECHO_CENTER_PIN_NUM | 1 << ECHO_RIGHT_PIN_NUM);
	
	// interrupt 설정 (echo)
	EICRB |= 0 << ISC41 | 1 << ISC40 | 0 << ISC51 | 1 << ISC50 | 0 << ISC61 | 1 << ISC60;
	
	// timer 설정 (64분주)
	TCCR2 |= 1 << CS22 | 0 << CS21 | 1 << CS20;
	
	// local interrupt 마스크
	EIMSK |= 1 << INT4 | 1 << INT5 | 1 << INT6; // EXTERNAL interrupt 활성화
	
	// timer overflow interrupt 마스크
	TIMSK |= 1 << TOIE2;
}

void trigger_ultrasonic_left(void)
{
	distance_checking = 1;
	// TRIG 보내기 전에 일정시간 LOW로 둔 다음에 보내야함!
	// HIGH 보낸 다음에도 LOW로 다시 돌려두기
	
	TRIG_PORT &= ~(1 << TRIG_LEFT_PIN_NUM); // 0으로 초기화
	
	// 약간의 딜레이
	_delay_us(1);
	
	// HIGH 쏘기!!
	TRIG_PORT |= 1 << TRIG_LEFT_PIN_NUM;
	
	// 10us동안 보내기 -> 약간 여유있게 15로 했당
	_delay_us(15);
	
	// 다시 LOW로 돌리기
	TRIG_PORT &= ~(1 << TRIG_LEFT_PIN_NUM);
}

void trigger_ultrasonic_center(void)
{
	distance_checking = 1;
	
	TRIG_PORT &= ~(1 << TRIG_CENTER_PIN_NUM); 
	
	_delay_us(1);
	
	TRIG_PORT |= 1 << TRIG_CENTER_PIN_NUM;
	
	_delay_us(15);
	
	TRIG_PORT &= ~(1 << TRIG_CENTER_PIN_NUM);
}

void trigger_ultrasonic_right(void)
{
	distance_checking = 1;
	// TRIG 보내기 전에 일정시간 LOW로 둔 다음에 보내야함!
	// HIGH 보낸 다음에도 LOW로 다시 돌려두기
	
	TRIG_PORT &= ~(1 << TRIG_RIGHT_PIN_NUM); // 0으로 초기화
	
	// 약간의 딜레이
	_delay_us(1);
	
	// HIGH 쏘기!!
	TRIG_PORT |= 1 << TRIG_RIGHT_PIN_NUM;
	
	// 10us동안 보내기 -> 약간 여유있게 15로 했당
	_delay_us(15);
	
	// 다시 LOW로 돌리기
	TRIG_PORT &= ~(1 << TRIG_RIGHT_PIN_NUM);
}

void check_obstacle(void)
{

	if(!distance_checking)
	{
		printf("%d\n", ultrasonic_dis);
		printf("trigger\n");
		triger_funcs[check_distance_index]();
	}
}