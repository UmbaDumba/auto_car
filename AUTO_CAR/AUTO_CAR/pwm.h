/*
	16bit timer 1
	PWM (2개 사용)
	=============
	PB5 : OC1A <- 왼쪽 바퀴
	PB6 : OC1B <- 오른쪽 바퀴
	=============
	버튼제어
	BTN0 : auto <-> manual
	=============
	방향 설정
	1. LEFT MOTOR
		PF0 : IN1
		PF1 : IN2
	2. RIGHT MOTOR
		PF2 : IN3
		PF3 : IN4
		
	IN1/3	IN2/4
	  1		  0		직진 (정회전)
	  0		  1		후진 (역회전)
	  1		  1		STOP
	=============
*/


#ifndef PWM_H_
#define PWM_H_

#define  F_CPU 16000000UL  // 16MHZ

#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>
#include <stdio.h>		// printf


#define MOTOR_PWM_DDR				DDRB
#define MOTOR_LEFT_PORT_DDR_NUM		5
#define MOTOR_RIGHT_PORT_DDR_NUM	6

#define MOTOR_DRIVER_DIRECTION_PORT	PORTF
#define MOTOR_DRIVER_DIRECTION_DDR	DDRF
#define MOTOR_DRIVER_IN1_NUM		0
#define MOTOR_DRIVER_IN2_NUM		1
#define MOTOR_DRIVER_IN3_NUM		2
#define MOTOR_DRIVER_IN4_NUM		3

#define MOTOR_LEFT_PWM		OCR1A
#define MOTOR_RIGHT_PWM		OCR1B

void init_timer1(void);
void init_L298N(void);
void motor_stop(int speed);
void motor_direction_forward(int speed);
void motor_direction_backward(int speed);
void motor_turn_left(int speed);
void motor_turn_right(int speed);
void motor_backward_left(int speed);
void motor_backward_right(int speed);


#endif /* PWM_H_ */