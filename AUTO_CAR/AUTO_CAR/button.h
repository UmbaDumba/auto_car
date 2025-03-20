/*
 * button.h
 *
 * Created: 2025-03-05 오후 2:30:04
 *  Author: microsoft
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#define  F_CPU 16000000UL  
#include <avr/io.h>  
#include <util/delay.h>  


#define BUTTON_DDR DDRA 
#define BUTTON_PIN PINA    

#define BUTTON0PIN  3

#define BUTTON0   0   
#define BUTTON_NUMER 1   // button갯수 

#define BUTTON_PRESS  1   // 버튼을 누르면 high(active-high)
#define BUTTON_RELEASE 0  // 버튼을 떼면 low

void init_button(void);
int get_button(int button_num, int button_pin);

#endif /* BUTTON_H_ */