

#define  F_CPU 16000000UL  // 16MHZ

#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdio.h>		// printf

#include "def.h"
#include "pwm.h"
#include "uart1.h"

volatile int msec_count = 0;


void init_timer0(void);
void moving_manual_mode(t_car_info *my_car);

void (*car_move_func[])(int) = 
{
	motor_direction_forward,
	motor_direction_backward,
	motor_turn_left,
	motor_turn_right,
	motor_stop
};

void (*car_program_modes[])(t_car_info *) =
{
	moving_manual_mode
};

int speed_boundarys[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};

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
	init_uart1();
	sei();
	
	t_car_info my_car;
	my_car.mode = MANUAL_MODE;
	my_car.state = STOP;
	my_car.speed = 3;
	
	
	motor_stop(0);
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
		
		car_program_modes[my_car.mode](&my_car);
		
    }
}

void init_timer0(void)
{
	TCNT0 = 6;
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00;
	TIMSK |= 1 << TOIE0;
}

void moving_manual_mode(t_car_info *my_car)
{
		switch(bt_data)
		{
			case 'F':
			case 'f':
				my_car->state = FORWARD;
				break;
			case 'B':
			case 'b':
				my_car->state = BACKWARD;
				break;
			case 'L':
			case 'l':
				my_car->state = TURN_LEFT;
				break;
			case 'R':
			case 'r':
				my_car->state = TURN_RIGHT;
				break;
			case 'U':
			case 'u':
				(my_car->speed)++;
				my_car->speed = (my_car->speed >= SPEED_BOUNDARY_NUM) ? SPEED_BOUNDARY_NUM - 1 : my_car->speed;
				bt_data = pre_bt_data;
				break;
			case 'D':
			case 'd':
				(my_car->speed)--;
				my_car->speed = (my_car->speed < 0) ? 0 : my_car->speed;
				bt_data = pre_bt_data;
				break;
			default:
				my_car->state = STOP;
		}
		
		car_move_func[my_car->state](speed_boundarys[my_car->speed]);
}