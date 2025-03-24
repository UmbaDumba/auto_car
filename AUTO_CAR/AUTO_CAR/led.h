/*
 * led.h
 *
 * Created: 2025-03-05 오전 10:24:53
 *  Author: microsoft
 */ 


#ifndef LED_H_
#define LED_H_


#define F_CPU 16000000UL // 16MHz로 시스템 클럭 설정 (AVR이니까), Unsigned Long == UL
#include <avr/io.h>      // DDRA, PORTA같은 레지스터 값 저장되어있음
#include <util/delay.h>

#define LED_DDR			DDRG
#define LED_PORT		PORTG
#define LED_PORT_NUM	3

void init_led(void);
void led_on(void);
void led_off(void);


#endif /* LED_H_ */