#include "audio_gpio.h"

// Bit mapping:
// bit 0 -> PF0
// bit 1 -> PF1
// bit 2 -> PA8
// bit 3 -> PA0
// bit 4 -> PA1
// bit 5 -> PA4
// bit 6 -> PA5
// bit 7 -> PA6

void AudioGPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure PF0, PF1
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    // Configure PA0, PA1, PA4, PA5, PA6, PA8
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 |
                          GPIO_PIN_4 | GPIO_PIN_5 |
                          GPIO_PIN_6 | GPIO_PIN_8;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void AudioGPIO_Output(uint8_t sample)
{
    uint32_t setA = 0, resetA = 0;
    uint32_t setF = 0, resetF = 0;

    // PF0 (bit 0)
    if (sample & (1 << 0)) setF |= GPIO_PIN_0;
    else resetF |= GPIO_PIN_0;

    // PF1 (bit 1)
    if (sample & (1 << 1)) setF |= GPIO_PIN_1;
    else resetF |= GPIO_PIN_1;

    // PA8 (bit 2)
    if (sample & (1 << 2)) setA |= GPIO_PIN_8;
    else resetA |= GPIO_PIN_8;

    // PA0 (bit 3)
    if (sample & (1 << 3)) setA |= GPIO_PIN_0;
    else resetA |= GPIO_PIN_0;

    // PA1 (bit 4)
    if (sample & (1 << 4)) setA |= GPIO_PIN_1;
    else resetA |= GPIO_PIN_1;

    // PA4 (bit 5)
    if (sample & (1 << 5)) setA |= GPIO_PIN_4;
    else resetA |= GPIO_PIN_4;

    // PA5 (bit 6)
    if (sample & (1 << 6)) setA |= GPIO_PIN_5;
    else resetA |= GPIO_PIN_5;

    // PA6 (bit 7)
    if (sample & (1 << 7)) setA |= GPIO_PIN_6;
    else resetA |= GPIO_PIN_6;

    // Apply changes (atomic per port)
    GPIOF->BSRR = (resetF << 16) | setF;
    GPIOA->BSRR = (resetA << 16) | setA;
}