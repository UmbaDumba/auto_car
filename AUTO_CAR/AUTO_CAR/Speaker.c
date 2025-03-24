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

// 1 : �÷�����, 2 : �÷��̿Ϸ�, 3 : 10ms ��ٸ�����
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

// Timer3 ������ PWM
void init_speaker(void)
{
	DDRE |= 0x08;   // PWM CHANNEL  OC3A(PE3) ��� ���� ���� �Ѵ�. 
	TCCR3A = (1<<COM3A0); // COM3A0 : ����ġ�� PE3 ��� ���� (P328 ǥ14-6 ����)
	TCCR3B = (1<<WGM32) | (1<<CS31);   // WGM32 : CTC 4(P327 ǥ14-5) CS31: 8����(P318)
	// CTC mode : ����ġ�� �Ǹ� ī���ʹ� reset�Ǹ鼭 PWM ���� ��� ���� ����� ���� ��. 
	// ������� CTC����� �������� ����ġ �߻��� TCNT1�� �������Ͱ��� 0���� ���� �ϴ��� ���� �̴�. 
	// �����带 ���� TCNT1�� �ڵ����� 0���� ���� ���� �ʾ� ���ͷ�Ʈ ��ƾ���� TCNT1�� 0���� ���� �� �־���. 
	// ������ PWM mode4  CTC ���ֺ� 8  16000000/8 ==> 2,000,000HZ(2000KHZ) : 
	// up-dounting: ����ġ�� LOW, down-counting�� HIGH���
	// 1/2000000 ==> 0.0000005sec (0.5us)
	// P599 TOP �� ��� ����
	// PWM���ļ� = OSC(16M) / ( 2(up.down)x N(������)x(1+TOP) ) 
	// TOP =  (fOSC(16M) /  2(up.down)x N(������)x ���ϴ����ļ� )) -1 
	//-----------------------------------------------------------
	// - BOTTOM :  ī���Ͱ� 0x00/0x0000 �϶��� ����Ų��.
    // - MAX : ī���Ͱ� 0xFF/0xFFFF �� ���� ����Ų��.
    // - TOP?:  ī���Ͱ� ���� �� �ִ� �ִ밪�� ����Ų��. �����÷ο� ���ͷ�Ʈ�� ��� TOP�� 0xFF/0xFFFF
    //          ������ ����ġ ���ͷ�Ʈ�� ��� ����ڰ� ������ ���� �ȴ�. 
    
	TCCR3C = 0;  // P328 �׸� 14-11 ���� 
	SPEAKER_PWM = 0;   // �� ��ġ���� OCR3A�� �ִ´�. 
	
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