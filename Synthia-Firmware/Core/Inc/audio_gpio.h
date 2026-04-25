#ifndef AUDIO_GPIO_H
#define AUDIO_GPIO_H

#include "stm32g4xx_hal.h" // adjust for your MCU family

void AudioGPIO_Init(void);
void AudioGPIO_Output(uint8_t sample);

#endif