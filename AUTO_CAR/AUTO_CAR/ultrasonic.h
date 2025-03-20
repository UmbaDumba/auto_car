/*
 * ultrasonic.h
 *
 * Created: 2025-03-12 오후 2:49:06
 *  Author: microsoft
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>

#define MAX_LENGHT			6

#define TRIG_PORT			PORTA
#define TRIG_DDR			DDRA
#define TRIG_LEFT_PIN_NUM	0
#define TRIG_CENTER_PIN_NUM	1
#define TRIG_RIGHT_PIN_NUM	2

#define ECHO_PIN			PINE
#define ECHO_DDR			DDRE
#define ECHO_LEFT_PIN_NUM	4
#define ECHO_CENTER_PIN_NUM	5
#define ECHO_RIGHT_PIN_NUM	6

volatile int obstacle_info[];

void init_ultrasonic(void);
void trigger_ultrasonic_left(void);
void trigger_ultrasonic_center(void);
void trigger_ultrasonic_right(void);
void check_obstacle(void);


#endif /* ULTRASONIC_H_ */