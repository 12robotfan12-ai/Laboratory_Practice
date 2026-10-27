#include "init.h"

void GPIO_Ini(void)
{
    // 1. Включение тактирования портов GPIOB и GPIOF
    BIT_SET(RCC_AHB1ENR, RCC_GPIOB | RCC_GPIOF);

    // 2. Светодиоды на плате (PB0, PB7, PB14) на ВЫХОД (01b)
    BIT_CLEAR(GPIOB_MODER, (3UL << 0) | (3UL << 14) | (3UL << 28));
    BIT_SET(GPIOB_MODER, (1UL << 0) | (1UL << 14) | (1UL << 28));
    BIT_SET(GPIOB_OSPEEDR, (1UL << 0) | (1UL << 14) | (1UL << 28));
    // Гасим светодиоды платы при старте
    GPIOB_BSRR = (1UL << 16) | (1UL << 23) | (1UL << 30);

    // 3. Порт F:
    // Пины кнопок PF0, PF1, PF2, PF3 настраиваем на ВХОД (00b)
    BIT_CLEAR(GPIOF_MODER, 0x000000FFUL);

    // Включаем внутреннюю подтяжку к земле (Pull-Down = 10b) для кнопок PF0..PF3:
    // PF0 (бит 1), PF1 (бит 3), PF2 (бит 5), PF3 (бит 7) -> маска 0xAA
    BIT_CLEAR(GPIOF_PUPDR, 0x000000FFUL);
    BIT_SET(GPIOF_PUPDR, 0x000000AAUL);

    // Пины внешних светодиодов PF7, PF8, PF9 настраиваем на ВЫХОД (01b)
    // PF7 (биты 15:14 -> 01b, сдвиг 14)
    // PF8 (биты 17:16 -> 01b, сдвиг 16)
    // PF9 (биты 19:18 -> 01b, сдвиг 18)
    BIT_CLEAR(GPIOF_MODER, (3UL << 14) | (3UL << 16) | (3UL << 18));
    BIT_SET(GPIOF_MODER, (1UL << 14) | (1UL << 16) | (1UL << 18));
    BIT_SET(GPIOF_OSPEEDR, (1UL << 14) | (1UL << 16) | (1UL << 18));

    // Гасим внешние светодиоды при старте (сброс PF7..PF9 через BSRR: биты 16+7, 16+8, 16+9)
    GPIOF_BSRR = (1UL << 23) | (1UL << 24) | (1UL << 25);
}

// Управление 6 светодиодами в линии (индексы 0..5):
// 0 -> PB0 (LED1 на плате, зеленый)
// 1 -> PB7 (LED2 на плате, синий)
// 2 -> PB14 (LED3 на плате, красный)
// 3 -> PF7 (LED4 на макетке, CN9 пин 26)
// 4 -> PF8 (LED5 на макетке, CN9 пин 24)
// 5 -> PF9 (LED6 на макетке, CN9 пин 28)
void LED_SetState(uint8_t led_idx, uint8_t state)
{
    switch (led_idx)
    {
    case 0: // PB0 на плате
        if (state)
            GPIOB_BSRR = (1UL << 0);
        else
            GPIOB_BSRR = (1UL << 16);
        break;

    case 1: // PB7 на плате
        if (state)
            GPIOB_BSRR = (1UL << 7);
        else
            GPIOB_BSRR = (1UL << 23);
        break;

    case 2: // PB14 на плате
        if (state)
            GPIOB_BSRR = (1UL << 14);
        else
            GPIOB_BSRR = (1UL << 30);
        break;

    case 3: // PF7 на макетке (CN9, пин 26)
        if (state)
            GPIOF_BSRR = (1UL << 7);
        else
            GPIOF_BSRR = (1UL << 23);
        break;

    case 4: // PF8 на макетке (CN9, пин 24)
        if (state)
            GPIOF_BSRR = (1UL << 8);
        else
            GPIOF_BSRR = (1UL << 24);
        break;

    case 5: // PF9 на макетке (CN9, пин 28)
        if (state)
            GPIOF_BSRR = (1UL << 9);
        else
            GPIOF_BSRR = (1UL << 25);
        break;

    default:
        break;
    }
}