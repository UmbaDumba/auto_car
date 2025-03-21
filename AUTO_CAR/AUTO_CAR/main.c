

#define  F_CPU 16000000UL  // 16MHZ

#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdio.h>		// printf

#include "def.h"
#include "pwm.h"
#include "uart0.h"
#include "uart1.h"
#include "ultrasonic.h"
#include "button.h"
#include "I2C_LCD.h"
#include "fnd.h"
#include "speaker.h"

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile int msec_count = 0;
int sec_count = 0;
volatile int monitor_shoot = 0;

int auto_car_program(void);
int test_program(void);
void init_timer0(void);
void moving_manual_mode(t_car_info *my_car);
void moving_auto_mode(t_car_info *my_car);

void (*car_move_func[])(int) = 
{
	motor_direction_forward,
	motor_direction_backward,
	motor_turn_left,
	motor_turn_right,
	motor_stop,
	motor_backward_left,
	motor_backward_right,
	motor_turn_left_bi,
	motor_turn_right_bi,
};

void (*car_program_modes[])(t_car_info *) =
{
	moving_manual_mode,
	moving_auto_mode,
	moving_auto_mode,
	moving_auto_mode
};

int speed_boundarys[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
	
char * lcd_texts[] = 
{
	"manual mode",
	"AMBULACE",
	"FIRE ENGINE",
	"KINDERGARTEN",
	"speed 1 (min)",
	"speed 2",
	"speed 3",
	"speed 4",
	"speed 5",
	"speed 6",
	"speed 7",
	"speed 8",
	"speed 9",
	"speed 10 (max)",
	"stop"
};

int fnd_fonts[][4] = {
	{FND_F, FND_blank, FND_blank, FND_blank},	// F - - -
	{FND_b, FND_blank, FND_blank, FND_blank},	// b - - -
	{FND_L, FND_blank, FND_blank, FND_blank},	// L - - -
	{FND_R, FND_blank, FND_blank, FND_blank},	// R - - -
	{FND_S, FND_blank, FND_blank, FND_blank},	// S - - -
	{FND_b, FND_blank, FND_L, FND_blank},		// b - L -
	{FND_b, FND_blank, FND_R, FND_blank},		// b - R -
	{FND_blank, FND_blank, FND_L, FND_L},		// - - L L
	{FND_blank, FND_blank, FND_R, FND_R}		// - - R R
};

// =======================================================================================

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; // 6 ~ 256으로 돌리기 위해
	msec_count++;
	
	monitor_shoot = 1;
}

int main(void)
{
	//test_program();
	auto_car_program();
	
	return 0;
}

int test_program(void)
{
	init_timer0();
	init_timer1();
	init_L298N();
	init_uart1();
	init_uart0();
	init_ultrasonic();
	init_button();
	I2C_LCD_init();
	init_fnd();
	init_speaker();
	sei();
	
	while(1)
	{
		//_delay_ms(1000);
		_delay_ms(1);
		kinder_music();
		
		//motor_stop(0);
	}
	
	return 0;
}

int auto_car_program(void)
{
	// TODO
    // 각종 초기화
	init_timer0();
	init_timer1();
	init_L298N();
	init_uart1();
	init_uart0();
	init_ultrasonic();
	init_button();
	I2C_LCD_init();
	init_fnd();
	init_speaker();
	sei();
	stdout = &OUTPUT;
	
	t_car_info my_car;
	my_car.mode = MANUAL_MODE;
	my_car.state = STOP;
	my_car.speed = 3;
	my_car.lcd_text_up = lcd_texts[0];
	my_car.lcd_text_down = lcd_texts[my_car.speed + MODE_NUM];
	my_car.fnd_char = fnd_fonts[STOP];
	my_car.music_func = manual_music;
	
	motor_stop(0);
    while (1) 
    {
		// TODO
		// LED 제어하기
		// 부저 제어하기
		
		if(monitor_shoot)
		{
			monitor_shoot = 0;
			fnd_display_all(my_car.fnd_char);
			my_car.music_func();
			if(msec_count % 100 == 0)
			{
				I2C_LCD_clear();
				I2C_LCD_write_string_XY(0,0,my_car.lcd_text_up);
				I2C_LCD_write_string_XY(1,0,my_car.lcd_text_down);
			}
		}
		
		if(get_button(BUTTON0, BUTTON0PIN))
		{
			if(my_car.mode == MANUAL_MODE)
			{
				my_car.mode = AUTO_MODE_A;
				my_car.state = FORWARD;
				my_car.speed = 2;
				my_car.lcd_text_up = lcd_texts[1];
				my_car.lcd_text_down = lcd_texts[2 + MODE_NUM];
				my_car.fnd_char = fnd_fonts[FORWARD];
				my_car.music_func = ambulance_music;
			}else if(my_car.mode == AUTO_MODE_A)
			{
				my_car.mode = AUTO_MODE_B;
				my_car.state = FORWARD;
				my_car.speed = 2;
				my_car.lcd_text_up = lcd_texts[2];
				my_car.lcd_text_down = lcd_texts[2 + MODE_NUM];
				my_car.fnd_char = fnd_fonts[FORWARD];
				my_car.music_func = fire_music;
			}else if(my_car.mode == AUTO_MODE_B)
			{
				my_car.mode = AUTO_MODE_C;
				my_car.state = FORWARD;
				my_car.speed = 2;
				my_car.lcd_text_up = lcd_texts[3];
				my_car.lcd_text_down = lcd_texts[2 + MODE_NUM];
				my_car.fnd_char = fnd_fonts[FORWARD];
				my_car.music_func = kinder_music;
			}else if(my_car.mode == AUTO_MODE_C)
			{
				bt_data = 's';
				my_car.mode = MANUAL_MODE;
				my_car.state = STOP;
				my_car.speed = 3;
				my_car.lcd_text_up = lcd_texts[0];
				my_car.lcd_text_down = lcd_texts[3 + MODE_NUM];
				my_car.fnd_char = fnd_fonts[STOP];
				my_car.music_func = manual_music;
			}
			
		}
		
		car_program_modes[my_car.mode](&my_car);
		
		if(msec_count > 1000)
		{
			msec_count = 0;
			sec_count++;
		}
		
		
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
				(*my_car).state = FORWARD;
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
		
		my_car->fnd_char = fnd_fonts[my_car->state];
		if(my_car->state != STOP)
		{
			my_car->lcd_text_down = lcd_texts[my_car->speed + MODE_NUM];
		}else{
			my_car->lcd_text_down = lcd_texts[10 + MODE_NUM];
		}
		car_move_func[my_car->state](speed_boundarys[my_car->speed]);
}

void moving_auto_mode(t_car_info *my_car)
{
	static int back_count = 0;
	static int bi_count = 0;
	
	check_obstacle();
	
	if(my_car->state == BI_LEFT || my_car->state == BI_RIGHT)
	{
		bi_count++;
		if(bi_count < 500)
		{
			car_move_func[my_car->state](speed_boundarys[my_car->speed]);
		}else{
			bi_count = 0;
		}
	}
	
	
	if(obstacle_info[0] > OBSTACLE_VALUE_D && obstacle_info[1] > OBSTACLE_VALUE_D && obstacle_info[2] > OBSTACLE_VALUE_D)
	{
		my_car->speed = 9;
		my_car->state = FORWARD;
	}else if(obstacle_info[1] < OBSTACLE_VALUE_D && obstacle_info[2] > OBSTACLE_VALUE_C && obstacle_info[0] < obstacle_info[2])
	{
		my_car->speed = 9;
		//my_car->state = TURN_RIGHT;
		my_car->state = BI_RIGHT;
	}else if(obstacle_info[1] < OBSTACLE_VALUE_D && obstacle_info[0] > OBSTACLE_VALUE_C && obstacle_info[0] > obstacle_info[2])
	{
		my_car->speed = 9;
		//my_car->state = TURN_LEFT;
		my_car->state = BI_LEFT;
	}else if(obstacle_info[0] < OBSTACLE_VALUE_B && obstacle_info[2] > OBSTACLE_VALUE_B)
	{
		my_car->speed = 9;
		my_car->state = TURN_RIGHT;
		//my_car->state = BI_RIGHT;
	}else if(obstacle_info[2] < OBSTACLE_VALUE_B && obstacle_info[0] > OBSTACLE_VALUE_B)
	{
		my_car->speed = 9;
		my_car->state = TURN_LEFT;
		//my_car->state = BI_LEFT;
	}
	else if(obstacle_info[0] < OBSTACLE_VALUE_A && obstacle_info[1] < OBSTACLE_VALUE_A && obstacle_info[2] < OBSTACLE_VALUE_A)
	{
		my_car->speed = 3;
		my_car->state = BACKWARD;	
	}else if(obstacle_info[1] < OBSTACLE_VALUE_A)
	{
		my_car->state = STOP;
	}else
	{
		my_car->speed = 5;
		my_car->state = FORWARD;
	}
	


	my_car->fnd_char = fnd_fonts[my_car->state];
	if(my_car->state != STOP)
	{
		my_car->lcd_text_down = lcd_texts[my_car->speed + MODE_NUM];
	}else{
		my_car->lcd_text_down = lcd_texts[10 + MODE_NUM];
	}
	
	car_move_func[my_car->state](speed_boundarys[my_car->speed]);
}