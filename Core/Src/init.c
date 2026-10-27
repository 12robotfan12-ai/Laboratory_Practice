#include "init.h"

void GPIO_Ini(void)
{
    // 1. Включение тактирования портов GPIOB и GPIOF
    BIT_SET(RCC_AHB1ENR, RCC_GPIOB | RCC_GPIOF);

    // 2. Настройка светодиодов платы (PB0 - желтый/зеленый, PB7 - синий, PB14 - красный) на ВЫХОД
    // PB0 -> бит 0 (0x01), PB7 -> бит 14 (0x4000), PB14 -> бит 28 (0x10000000)
    BIT_SET(GPIOB_MODER, 0x00000001UL | 0x00004000UL | 0x10000000UL);

    // Настройка скорости работы PB0, PB7, PB14 на среднюю
    BIT_SET(GPIOB_OSPEEDR, 0x00000001UL | 0x00004000UL | 0x10000000UL);

    // 3. Изначально все порты PF0, PF1, PF2, PF3 настроены на ВХОД (в GPIOF_MODER нули по умолчанию)
    BIT_SET(GPIOF_MODER, 0x00UL);

    // Включаем подтяжку к земле (Pull-Down = 10b) для PF0, PF1, PF2, PF3:
    // PF0 -> 0x02, PF1 -> 0x08, PF2 -> 0x20, PF3 -> 0x80 (в сумме 0xAA)
    BIT_SET(GPIOF_PUPDR, 0x02UL | 0x08UL | 0x20UL | 0x80UL);
}