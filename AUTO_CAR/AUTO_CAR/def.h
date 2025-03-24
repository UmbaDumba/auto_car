/*
 * def.h
 *
 * Created: 2025-03-20 오후 3:07:00
 *  Author: microsoft
 */ 


#ifndef DEF_H_
#define DEF_H_

#define COMMAND_NUMBER 10 // command 개수
#define COMMAND_LENGTH 40 // command 길이

#define MODE_NUM	4
#define MANUAL_MODE	0	// 수동모드
#define AUTO_MODE_A	1	// 자동모드 (앰뷸런스)
#define AUTO_MODE_B 2	// 자동모드 (소방차)
#define AUTO_MODE_C	3	// 자동모드 (동요)

#define FORWARD		0
#define BACKWARD	1
#define TURN_LEFT	2
#define TURN_RIGHT	3
#define STOP		4
#define BACK_LEFT	5
#define BACK_RIGHT	6
#define BI_LEFT		7
#define BI_RIGHT	8

#define SEQUENCE_TIME	100

#define OBSTACLE_VALUE_A	5
#define OBSTACLE_VALUE_B	10
#define OBSTACLE_VALUE_C	15
#define OBSTACLE_VALUE_D	25

#define SPEED_BOUNDARY_NUM	10

#define FND_blank	0xff
#define FND_F		0x8e
#define FND_b		0x83
#define FND_L		0xc7
#define FND_R		0x88
#define FND_S		0x92

typedef struct car_info{
	int mode;						// 수동모드 0, 자동모드 1
	int state;						// 지금 자동차 상태
												// 직진, 후진, 좌회전, 우회전, 멈춤
	int sequence_state;				// 연속으로 500ms를 해야하는 state의 경우, sequence_state가 500을 넘겨야 다른동작을 받음
	int speed;						// 자동차 속도
	char *lcd_text_up;				// LCD 문구 (출력할것) -> 문자열
	char *lcd_text_down;
	int* fnd_char;					// FND 문구 (출력할것) -> font 배열 포인터??
	void (*music_func)(void);		// 틀어 줄 노래 (부저 출력노래 결정)
	int led_duration;				// led가 몇ms 단위로 깜빡일것인지
}t_car_info;

#endif /* DEF_H_ */