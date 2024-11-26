#ifndef __BSP_VOICE_H
#define __BSP_VOICE_H
#include "main.h"


#define  VOICE_SOUND()       do{VOICE_MUTE_GPIO_Port->BRR = VOICE_MUTE_Pin;}while(0)
#define  VOICE_MUTE()        do{VOICE_MUTE_GPIO_Port ->BSRR |= VOICE_MUTE_Pin;}while(0)


void voice_power_on_sound(void);

void voice_power_off_sound(void);

void voice_fan_warning_sound(void);

void voice_set_timer_timing_value(uint8_t set_hours);

void voice_cancel_timer_timing(void);

void Voice_Warning_Sound_Fan(void);


#endif 

