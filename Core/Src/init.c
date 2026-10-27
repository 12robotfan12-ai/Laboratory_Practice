#include "init.h"

void GPIO_Ini(void)
{
    // 1. Включение тактирования портов GPIOB и GPIOF через макрос BIT_SET
    BIT_SET(RCC_AHB1ENR, RCC_GPIOB | RCC_GPIOF);

    // 2. Настройка светодиодов платы (PB0 - зеленый, PB7 - синий, PB14 - красный) на ВЫХОД (01b)
    BIT_CLEAR(GPIOB_MODER, (3UL << 0) | (3UL << 14) | (3UL << 28));
    BIT_SET(GPIOB_MODER, (1UL << 0) | (1UL << 14) | (1UL << 28));

    // Настройка скорости работы PB0, PB7, PB14 на среднюю
    BIT_SET(GPIOB_OSPEEDR, (1UL << 0) | (1UL << 14) | (1UL << 28));

    // Гасим светодиоды платы при старте
    BIT_SET(GPIOB_BSRR, (1UL << 16) | (1UL << 23) | (1UL << 30));

    // 3. Настройка порта GPIOF:
    // Пины кнопок PF0, PF1, PF2, PF3 -> на ВХОД (00b)
    BIT_CLEAR(GPIOF_MODER, 0x000000FFUL);

    // Подтяжка к земле (Pull-Down = 10b) для PF0, PF1, PF2, PF3:
    // PF0 -> 0x02, PF1 -> 0x08, PF2 -> 0x20, PF3 -> 0x80 (в сумме 0xAA)
    BIT_CLEAR(GPIOF_PUPDR, 0x000000FFUL);
    BIT_SET(GPIOF_PUPDR, 0x02UL | 0x08UL | 0x20UL | 0x80UL);

    // Пины внешних светодиодов PF7, PF8, PF9 -> на ВЫХОД (01b в битах 14, 16, 18)
    BIT_CLEAR(GPIOF_MODER, (3UL << 14) | (3UL << 16) | (3UL << 18));
    BIT_SET(GPIOF_MODER, (1UL << 14) | (1UL << 16) | (1UL << 18));
    BIT_SET(GPIOF_OSPEEDR, (1UL << 14) | (1UL << 16) | (1UL << 18));

    // Гасим внешние светодиоды при старте
    BIT_SET(GPIOF_BSRR, (1UL << 23) | (1UL << 24) | (1UL << 25));
}