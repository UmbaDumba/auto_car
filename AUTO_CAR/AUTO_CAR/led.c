/*
 * led.c
 *
 * Created: 2025-03-05 오전 10:21:53
 *  Author: microsoft
 */ 

#include "led.h"



void init_led(void)
{
	LED_DDR |= 1 << LED_PORT_NUM;
	led_off();
}

void led_on(void)
{
	LED_PORT |= 1 << LED_PORT_NUM;
}

void led_off(void)
{
	LED_PORT &= ~(1 << LED_PORT_NUM);
}
