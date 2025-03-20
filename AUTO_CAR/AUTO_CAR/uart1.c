/*

*/

#include "uart1.h"


volatile uint8_t bt_data = 'f'; // 명령어
ISR(USART1_RX_vect)
{
	bt_data = UDR1;
}

void init_uart1(void)
{
	// 전송속도 설정
	UBRR1H = 0x00;
	UBRR1L = 207; // 2배속 모드임

	UCSR1A |= 1 << U2X1; // 2배속 통신
	
	UCSR1C |= 0x06; // 비동기, none 8bit, data 8 bit
	
	// 인터럽트, uart 기능 설정
	UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
}

