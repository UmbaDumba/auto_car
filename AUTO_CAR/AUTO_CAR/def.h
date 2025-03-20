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


#define MANUAL_MODE	0	// 수동모드
#define AUTO_MODE	1	// 자동모드

#define FORWARD		0
#define BACKWARD	1
#define TURN_LEFT	2
#define TURN_RIGHT	3
#define STOP		4

#define SPEED_BOUNDARY_NUM	10

typedef struct car_info{
	int mode; // 수동모드 0, 자동모드 1
	int state; // 지금 자동차 상태
					// 직진, 후진, 좌회전, 우회전, 멈춤
	int speed; // 자동차 속도
	// todo		// 틀어 줄 노래 (부저 출력노래 결정)
	// todo		// LCD 문구 (출력할것)
	// todo		// FND 문구 (출력할것)
	
}t_car_info;

#endif /* DEF_H_ */