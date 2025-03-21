/*
 * speaker.h
 *
 * Created: 2025-03-21 오후 2:33:57
 *  Author: microsoft
 */ 


#ifndef SPEAKER_H_
#define SPEAKER_H_

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SPEAKER_PWM	OCR3A

int play_tone(int tone, int beat);
void Music_Player(int *tone, int *Beats);
void init_speaker(void);
void Beep(int repeat);
void Siren(int repeat);
void RRR(void);

void manual_music(void);
void fire_music(void);
void ambulance_music(void);
void kinder_music(void);

extern int manual_tone[];
extern int manual_beat[];
extern int School_Bell_Tune[];
extern const int School_Bell_Beats[];
extern const int Elise_Tune[];
extern const int Elise_Beats[];
extern int GrandFa_Tune[];
extern const int GrandFa_Beats[];

extern int ambulance_tone[];
extern int ambulance_beat[];
extern int fire_tone[];
extern int fire_beat[];

#define DO_01   1911
#define DO_01_H 1817
#define RE_01   1703
#define RE_01_H 1607
#define MI_01   1517
#define FA_01   1432
#define FA_01_H 1352
#define SO_01   1276
#define SO_01_H 1199
#define LA_01   1136
#define LA_01_H 1073
#define TI_01   1012
#define DO_02   956
#define DO_02_H 909
#define RE_02   851
#define RE_02_H 804
#define MI_02   758
#define FA_02   716
#define FA_02_H 676
#define SO_02   638
#define SO_02_H 602
#define LA_02   568
#define LA_02_H 536
#define TI_02   506
#define DO_03   478
#define DO_03_H 450
#define RE_03	425
#define RE_03_H 401
#define MI_03	378

#define F_CLK       16000000L //클럭
#define F_SCALER	8 //프리스케일러
#define BEAT_1_32	42
#define BEAT_1_16	86
#define BEAT_1_8	170
#define BEAT_1_4	341
#define BEAT_1_2	682
#define BEAT_1		1364


#endif /* SPEAKER_H_ */