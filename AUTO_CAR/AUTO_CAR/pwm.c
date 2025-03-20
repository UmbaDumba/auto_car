/*
 * pwm.c
 *
 * Created: 2025-03-13 오후 12:48:56
 *  Author: microsoft
 */ 

#include "pwm.h"



void init_timer1(void)
{
	// 분주비 설정 (64)
	TCCR1B |= 0 << CS12 | 1 << CS11 | 1 << CS10;
	
	// 모드설정 : 14번 (ICR1이 TOP인 고속 PWM)
	TCCR1A |= 0 << WGM10 | 1 << WGM11;
	TCCR1B |= 1 << WGM12 | 1 << WGM13;
	
	// 비반전 모드 (TOP : ICR1레지스터)
	TCCR1A |= 1 << COM1A1 | 1 << COM1B1;
	
	// ICR1레지스터 설정 (TOP 1023으로 설정, TOP까지 4ms로 설정함)
	ICR1 = 0x3ff;
}

void init_L298N(void)
{
	// DDR 출력모드 (PWM, 방향)
	MOTOR_PWM_DDR |= 1 << MOTOR_LEFT_PORT_DDR_NUM | 1 << MOTOR_RIGHT_PORT_DDR_NUM;
	MOTOR_DRIVER_DIRECTION_DDR |= 1 << MOTOR_DRIVER_IN1_NUM | 1 << MOTOR_DRIVER_IN2_NUM |
								  1 << MOTOR_DRIVER_IN3_NUM | 1 << MOTOR_DRIVER_IN4_NUM;
	
	// direction port reset
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << MOTOR_DRIVER_IN1_NUM | 1 << MOTOR_DRIVER_IN2_NUM |
									 1 << MOTOR_DRIVER_IN3_NUM | 1 << MOTOR_DRIVER_IN4_NUM);
	
	// direction 전진모드 설정 (정회전)
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << MOTOR_DRIVER_IN1_NUM | 1 << MOTOR_DRIVER_IN3_NUM;
}

// motor driver IN값을 전부 1로 set한다
void motor_stop(void)
{
	// direction port reset
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << MOTOR_DRIVER_IN1_NUM | 1 << MOTOR_DRIVER_IN2_NUM |
								     1 <<MOTOR_DRIVER_IN3_NUM | 1 << MOTOR_DRIVER_IN4_NUM);
	
	// direction port set
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << MOTOR_DRIVER_IN1_NUM | 1 << MOTOR_DRIVER_IN2_NUM |
								   1 << MOTOR_DRIVER_IN3_NUM | 1 << MOTOR_DRIVER_IN4_NUM;
}

void motor_direction_forward(int speed)
{
	// direction port reset
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << MOTOR_DRIVER_IN1_NUM | 1 << MOTOR_DRIVER_IN2_NUM |
									 1 << MOTOR_DRIVER_IN3_NUM | 1 << MOTOR_DRIVER_IN4_NUM);
		
	// direction 전진모드 설정 (정회전)
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << MOTOR_DRIVER_IN1_NUM | 1 << MOTOR_DRIVER_IN3_NUM;
	
	// 속도 적용 (양쪽 바퀴에 같은 속도)
	MOTOR_LEFT_PWM = MOTOR_RIGHT_PWM = speed;
}

void motor_direction_backward(int speed)
{
	// direction port reset
	MOTOR_DRIVER_DIRECTION_PORT &= ~(1 << MOTOR_DRIVER_IN1_NUM | 1 << MOTOR_DRIVER_IN2_NUM |
									 1 << MOTOR_DRIVER_IN3_NUM | 1 << MOTOR_DRIVER_IN4_NUM);
		
	// direction 후진모드 설정 (역회전)
	MOTOR_DRIVER_DIRECTION_PORT |= 1 << MOTOR_DRIVER_IN2_NUM | 1 << MOTOR_DRIVER_IN4_NUM;
	
	// 속도 적용 (양쪽 바퀴에 같은 속도)
	MOTOR_LEFT_PWM = MOTOR_RIGHT_PWM = speed;
}

void motor_turn_left(int speed)
{
	motor_direction_forward(0); // 전진모드 설정
	
	MOTOR_LEFT_PWM = 0;
	MOTOR_RIGHT_PWM = speed;
}

void motor_turn_right(int speed)
{
	motor_direction_forward(0); // 전진모드 설정
	MOTOR_LEFT_PWM = speed;
	MOTOR_RIGHT_PWM = 0;
}

