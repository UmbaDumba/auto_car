

#include <avr/io.h>

typedef struct car_info{
	int mode; // 수동모드 0, 자동모드 1
	int state; // 지금 자동차 상태
					// 직진, 후진, 좌회전, 우회전, 멈춤
	int speed; // 자동차 속도
	// todo		// 틀어 줄 노래 (부저 출력노래 결정)
	// todo		// LCD 문구 (출력할것)
	// todo		// FND 문구 (출력할것)
		
}t_car_info;

int main(void)
{
	// TODO
    // 각종 초기화
    while (1) 
    {
		// TODO
		// 바퀴 굴리기
		// LED 제어하기
		// 초음파센서로 앞 물체 파악하기
		// LCD 제어하기
		// FND 제어하기
		// 부저 제어하기
		// 블루투스 커멘드 처리
		
    }
}

