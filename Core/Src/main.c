#include <stdint.h>
#include "init.h"

// Задержка для защиты от дребезга контактов кнопки
void delay(volatile uint32_t count)
{
    while (count--)
        ;
}

int main(void)
{
    GPIO_Ini(); // Инициализация портов GPIO

    uint8_t step = 0;     // Счётчик нажатий "нулевой" кнопки (от 0 до 6)
    uint8_t btn0_old = 0; // Предыдущее состояние кнопки 0 (PF0)

    while (1)
    {
        // Считываем 0-й бит регистра GPIOF_IDR (пин PF0) делением с остатком (без оператора &)
        uint8_t btn0_now = (GPIOF_IDR >> 0) % 2;

        // 1) Обработка нажатия "нулевой" управляющей кнопки (PF0)
        if (btn0_now == 1 && btn0_old == 0)
        {
            step++;

            if (step == 1)
            {
                // 1-е нажатие: 1-й порт (PF1) переходит в режим ВЫХОДА (бит 2 = 0x04)
                BIT_SET(GPIOF_MODER, 0x04UL);
            }
            else if (step == 2)
            {
                // 2-е нажатие: 2-й порт (PF2) переходит в режим ВЫХОДА (бит 4 = 0x10)
                BIT_SET(GPIOF_MODER, 0x10UL);
            }
            else if (step == 3)
            {
                // 3-е нажатие: 3-й порт (PF3) переходит в режим ВЫХОДА (бит 6 = 0x40)
                BIT_SET(GPIOF_MODER, 0x40UL);
            }
            else if (step == 4)
            {
                // 4-е нажатие: все порты возвращаются в режим ВХОДА
                GPIOF_MODER = 0x00UL;
            }
            else if (step == 5)
            {
                // 5-е нажатие: все 3 порта (PF1, PF2, PF3) в режим ВЫХОДА
                BIT_SET(GPIOF_MODER, 0x04UL | 0x10UL | 0x40UL);
            }
            else if (step >= 6)
            {
                // 6-е нажатие: все порты возвращаются в режим ВХОДА
                GPIOF_MODER = 0x00UL;
                // Гасим все 3 внешних светодиода (биты сброса 16+1, 16+2, 16+3 в BSRR)
                BIT_SET(GPIOF_BSRR, (1UL << 17) | (1UL << 18) | (1UL << 19));
                step = 0; // Цикл повторяется
            }

            delay(300000); // Пауза от дребезга контактов
        }
        btn0_old = btn0_now;

        // 2) Работа 1-го порта (PF1):
        if (step < 1) // Режим ВХОДА
        {
            if ((GPIOF_IDR >> 1) % 2 != 0)
            {
                BIT_SET(GPIOB_BSRR, (1UL << 0)); // Удерживается Кнопка 1 -> Зажигаем желтый/зеленый LED (PB0)
            }
            else
            {
                BIT_SET(GPIOB_BSRR, (1UL << 16)); // Отпущена -> Гасим PB0 (бит 16 в BSRR)
            }
        }
        else // Режим ВЫХОДА
        {
            BIT_SET(GPIOB_BSRR, (1UL << 16)); // Гасим светодиод на плате (PB0)
            BIT_SET(GPIOF_BSRR, (1UL << 1));  // Горит светодиод, подключенный параллельно к PF1
        }

        // 3) Работа 2-го порта (PF2):
        if (step < 2) // Режим ВХОДА
        {
            if ((GPIOF_IDR >> 2) % 2 != 0)
            {
                BIT_SET(GPIOB_BSRR, (1UL << 7)); // Удерживается Кнопка 2 -> Зажигаем синий LED (PB7, 0x80)
            }
            else
            {
                BIT_SET(GPIOB_BSRR, (1UL << 23)); // Отпущена -> Гасим PB7 (бит 7+16=23 в BSRR)
            }
        }
        else // Режим ВЫХОДА
        {
            BIT_SET(GPIOB_BSRR, (1UL << 23)); // Гасим синий LED на плате
            BIT_SET(GPIOF_BSRR, (1UL << 2));  // Горит светодиод, подключенный параллельно к PF2
        }

        // 4) Работа 3-го порта (PF3):
        if (step < 3) // Режим ВХОДА
        {
            if ((GPIOF_IDR >> 3) % 2 != 0)
            {
                BIT_SET(GPIOB_BSRR, (1UL << 14)); // Удерживается Кнопка 3 -> Зажигаем красный LED (PB14)
            }
            else
            {
                BIT_SET(GPIOB_BSRR, (1UL << 30)); // Отпущена -> Гасим PB14 (бит 14+16=30 в BSRR)
            }
        }
        else // Режим ВЫХОДА
        {
            BIT_SET(GPIOB_BSRR, (1UL << 30)); // Гасим красный LED на плате
            BIT_SET(GPIOF_BSRR, (1UL << 3));  // Горит светодиод, подключенный параллельно к PF3
        }
    }
}