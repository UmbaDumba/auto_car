#include "speaker.h"

void Music_Player(int *tone, int *Beats)
{

	while(*tone != '/0')
	{
		SPEAKER_PWM = *tone;
		delay_ms(*Beats);
		tone++;
		Beats++;
		SPEAKER_PWM = 0;
		_delay_ms(10);		
	}
	return;
}  

// 1 : 플레이중, 2 : 플레이완료, 3 : 10ms 기다리는중
int play_tone(int tone, int beat)
{
	static int beat_count = 0;
	static int is_wait10ms = 0;
	
	if(is_wait10ms > 0)
	{
		is_wait10ms--;
		return 3;
	}
	
	beat_count--;
	if(beat_count == -1)
	{
		beat_count = beat;
		SPEAKER_PWM = tone;
	}else if(beat_count == 0)
	{
		is_wait10ms = 10;
		SPEAKER_PWM = 0;
		return 2;
	}
	return 1;
}

// Timer3 위상교정 PWM
void init_speaker(void)
{
	DDRE |= 0x08;   // PWM CHANNEL  OC3A(PE3) 출력 모드로 설정 한다. 
	TCCR3A = (1<<COM3A0); // COM3A0 : 비교일치시 PE3 출력 반전 (P328 표14-6 참고)
	TCCR3B = (1<<WGM32) | (1<<CS31);   // WGM32 : CTC 4(P327 표14-5) CS31: 8분주(P318)
	// CTC mode : 비교일치가 되면 카운터는 reset되면서 PWM 파형 출력 핀의 출력이 반전 됨. 
	// 정상모드와 CTC모드의 차이점은 비교일치 발생시 TCNT1의 레지스터값을 0으로 설정 하는지 여부 이다. 
	// 정상모드를 사용시 TCNT1이 자동으로 0으로 설정 되지 않아 인터럽트 루틴에서 TCNT1을 0으로 설정 해 주었다. 
	// 위상교정 PWM mode4  CTC 분주비 8  16000000/8 ==> 2,000,000HZ(2000KHZ) : 
	// up-dounting: 비교일치시 LOW, down-counting시 HIGH출력
	// 1/2000000 ==> 0.0000005sec (0.5us)
	// P599 TOP 값 계산 참고
	// PWM주파수 = OSC(16M) / ( 2(up.down)x N(분주율)x(1+TOP) ) 
	// TOP =  (fOSC(16M) /  2(up.down)x N(분주율)x 원하는주파수 )) -1 
	//-----------------------------------------------------------
	// - BOTTOM :  카운터가 0x00/0x0000 일때를 가리킨다.
    // - MAX : 카운터가 0xFF/0xFFFF 일 때를 가리킨다.
    // - TOP?:  카운터가 가질 수 있는 최대값을 가리킨다. 오버플로우 인터럽트의 경우 TOP은 0xFF/0xFFFF
    //          이지만 비교일치 인터럽트의 경우 사용자가 설정한 값이 된다. 
    
	TCCR3C = 0;  // P328 그림 14-11 참고 
	SPEAKER_PWM = 0;   // 비교 일치값을 OCR3A에 넣는다. 
	
	return;
}

void manual_music(void)
{
	SPEAKER_PWM = 0;
}

void kinder_music(void)
{
	static int music_index = 0;
	
	if(School_Bell_Tune[music_index] == '/0')
	{
		music_index = 0;
	}else
	{
		int re = play_tone(School_Bell_Tune[music_index], School_Bell_Beats[music_index]);
		if(re == 2)
		{
			music_index++;
		}
	}
}

void ambulance_music(void)
{
	static int sound_delay = 1000;
	static int my_sound[] = {610, 680};
	static int sound_index = 0;
	
	if(sound_delay > 0){
		sound_delay--;
	}else{
		sound_delay = 1000;
		sound_index = !sound_index;
	}
	SPEAKER_PWM = my_sound[sound_index];
}

void fire_music(void)
{
	static int is_up = 1;
	static int tick_count = 3;
	static int my_sound = 300;
	int pwm_down = 300;
	int pwm_up = 750;
	
	if(tick_count > 0){
		tick_count--;
	}else{
		tick_count = 3;
		if(is_up)
		{
			my_sound++;
			if(SPEAKER_PWM > pwm_up)
			{
				is_up = 0;
			} 
		}else
		{
			my_sound--;
			if(SPEAKER_PWM < pwm_down)
			{
				is_up = 1;
			}
		}
	}
	SPEAKER_PWM = my_sound;
}

void Beep(int repeat)
{
	int  i;
	
	for(i=0; i < repeat; i++)
	{
		SPEAKER_PWM = 500;  // 0.00025sec (250us) : 0.0000005 * 500
		_delay_ms(200);
		SPEAKER_PWM = 0;
		_delay_ms(200);
	}
}    

void Siren(int repeat)
{
	int i, j;
	
	SPEAKER_PWM = 900;
	
	for(i=0; i < repeat; i++)
	{
		for(j=0; j < 100; j++)
		{
			SPEAKER_PWM += 10;
			_delay_ms(20);
		}
		for(j=0; j < 100; j++)
		{
			SPEAKER_PWM -= 10;
			_delay_ms(20);
		}
	}
}

void RRR(void)
{
	int i;
	
	for(i=0; i<20; i++)
	{
		SPEAKER_PWM = 1136;
		_delay_ms(100);
		SPEAKER_PWM = 0;
		_delay_ms(20);
	}
}

void delay_ms(int ms)
{
	while(ms-- != 0)_delay_ms(1);
}