/*
 * ultrasonic.c
 *
 * Created: 2025-03-12 오후 2:49:18
 *  Author: microsoft
 */ 

#include "ultrasonic.h"

extern int sec_count;

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
	_delay_us(1);
	if(ECHO_PIN & 1 << ECHO_LEFT_PIN_NUM)
	{
		TCNT2 = 0;
		overflows = 0;
	}
	else
	{
		tcnt_value = TCNT2;
		overflows_value = overflows;
		
		if(overflows_value > 0)
		{
			// 약 2ms -> 35센치 이상 넘어감
			ultrasonic_dis = 100;
		}else{
			ultrasonic_dis = tcnt_value * 128.0 / 16;
			ultrasonic_dis /= 58;

		}
		
		if(ultrasonic_dis <= MAX_LENGHT){
			obstacle_info[0] = 1;
			}else{
			obstacle_info[0] = 0;
		}
		check_distance_index = 1;
		distance_checking = 0;
	}
}

// center
ISR(INT5_vect)
{
	_delay_us(1);
	if(ECHO_PIN & 1 << ECHO_CENTER_PIN_NUM)
	{
		TCNT2 = 0;
		overflows = 0;
	}
	else
	{
		tcnt_value = TCNT2;
		overflows_value = overflows;
		
		if(overflows_value > 0)
		{
			// 약 2ms -> 35센치 이상 넘어감
			ultrasonic_dis = 100;
			}else{
			ultrasonic_dis = tcnt_value * 128.0 / 16;
			ultrasonic_dis /= 58;

		}
		
		if(ultrasonic_dis <= MAX_LENGHT){
			obstacle_info[1] = 1;
			}else{
			obstacle_info[1] = 0;
		}
		check_distance_index = 2;
		distance_checking = 0;
	}
}

// right
ISR(INT6_vect)
{
	_delay_us(1);
	if(ECHO_PIN & 1 << ECHO_RIGHT_PIN_NUM)
	{
		TCNT2 = 0;
		overflows = 0;
	}
	else
	{
		tcnt_value = TCNT2;
		overflows_value = overflows;
		
		if(overflows_value > 0)
		{
			// 약 2ms -> 35센치 이상 넘어감
			ultrasonic_dis = 100;
			}else{
			ultrasonic_dis = tcnt_value * 128.0 / 16;
			ultrasonic_dis /= 58;

		}
		
		if(ultrasonic_dis <= MAX_LENGHT){
			obstacle_info[2] = 1;
		}else{
			obstacle_info[2] = 0;
		}
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
	
	// timer 설정 (128분주)
	TCCR2 |= 1 << CS22 | 0 << CS21 | 1 << CS20;
	
	// local interrupt 마스크
	EIMSK |= 1 << INT4 | 1 << INT5 | 1 << INT6; // EXTERNAL interrupt 활성화
	
	// timer overflow interrupt 마스크
	TIMSK |= 1 << TOIE2;
}

void trigger_ultrasonic_left(void)
{
	distance_checking = 1;
	
	TRIG_PORT &= ~(1 << TRIG_LEFT_PIN_NUM); 
	
	_delay_us(1);
	
	TRIG_PORT |= 1 << TRIG_LEFT_PIN_NUM;
	
	_delay_us(15);
	
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
	
	TRIG_PORT &= ~(1 << TRIG_RIGHT_PIN_NUM);
	
	_delay_us(1);
	
	TRIG_PORT |= 1 << TRIG_RIGHT_PIN_NUM;
	
	_delay_us(15);
	
	TRIG_PORT &= ~(1 << TRIG_RIGHT_PIN_NUM);
}

void check_obstacle(void)
{
	if(!distance_checking)
	{
		//_delay_ms(1000);
		printf("%d,%d,%d\n", ultrasonic_dis,overflows_value,tcnt_value);
		printf("\ntrigger\n");
		sec_count = 0;
		triger_funcs[check_distance_index]();
	}else{
		if(sec_count > 3)
		{
			distance_checking = 0;
		}
	}
}