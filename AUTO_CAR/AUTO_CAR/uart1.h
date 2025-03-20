/*
	블루투스 통신을 위한 uart1
	
	command가 문자열이 아닌 '문자' 하나로 들어온다.
	command 뒤에 \n이 들어오지 않는다는것 주의! (char 한개단위로 처리)
 */ 


#ifndef UART1_H_
#define UART1_H_

#define  F_CPU 16000000UL  // 16MHZ
#include <avr/io.h>
#include <util/delay.h>  // _delay_ms _delay_us
#include <avr/interrupt.h>

extern volatile uint8_t bt_data;

void init_uart1(void);

#endif /* UART1_H_ */