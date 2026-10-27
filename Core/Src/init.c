#include "init.h"

void GPIO_Ini(void)
{
    // 1. Включение тактирования портов GPIOB (бит 1 = 0x02) и GPIOF (бит 5 = 0x20)
    // Адрес RCC_AHB1ENR = 0x40023800 + 0x30 = 0x40023830
    (*(volatile uint32_t *)0x40023830UL) |= 0x22UL;

    // 2. Настройка светодиодов на плате (PB0, PB7, PB14) на ВЫХОД (01b)
    // Адрес GPIOB_MODER = 0x40020400 + 0x00
    // PB0 -> бит 0 (0x01), PB7 -> бит 14 (0x4000), PB14 -> бит 28 (0x10000000)
    (*(volatile uint32_t *)0x40020400UL) &= ~(0x3000C003UL);
    (*(volatile uint32_t *)0x40020400UL) |= 0x10004001UL;

    // Средняя скорость работы для PB0, PB7, PB14 (GPIOB_OSPEEDR = 0x40020408)
    (*(volatile uint32_t *)0x40020408UL) |= 0x10004001UL;

    // Гасим светодиоды платы при старте (GPIOB_BSRR = 0x40020418)
    (*(volatile uint32_t *)0x40020418UL) = (1UL << 16) | (1UL << 23) | (1UL << 30);

    // 3. Настройка порта GPIOF (базовый адрес 0x40021400):
    // Кнопки PF0, PF1, PF2, PF3 -> на ВХОД (00b в младших 8 битах GPIOF_MODER)
    // Внешние светодиоды PF7, PF8, PF9 -> на ВЫХОД (01b в битах 14, 16, 18 GPIOF_MODER)
    (*(volatile uint32_t *)0x40021400UL) &= ~(0x000FC0FFUL);
    (*(volatile uint32_t *)0x40021400UL) |= 0x00054000UL;

    // Подтяжка к земле (Pull-Down = 10b) для кнопок PF0..PF3 (GPIOF_PUPDR = 0x4002140C)
    // PF0 -> 0x02, PF1 -> 0x08, PF2 -> 0x20, PF3 -> 0x80 (в сумме 0xAA)
    (*(volatile uint32_t *)0x4002140CUL) &= ~(0x000000FFUL);
    (*(volatile uint32_t *)0x4002140CUL) |= 0x000000AAUL;

    // Гасим внешние светодиоды PF7, PF8, PF9 при старте (GPIOF_BSRR = 0x40021418)
    (*(volatile uint32_t *)0x40021418UL) = (1UL << 23) | (1UL << 24) | (1UL << 25);
}