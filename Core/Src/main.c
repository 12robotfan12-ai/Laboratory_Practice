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

    uint8_t step = 0;     // Счётчик нажатий "нулевой" управляющей кнопки (PF0) (от 0 до 6)
    uint8_t btn0_old = 0; // Предыдущее состояние кнопки 0 (PF0)

    while (1)
    {
        // Считываем 0-й бит регистра GPIOF_IDR (пин PF0) через макрос BIT_READ
        uint8_t btn0_now = BIT_READ(GPIOF_IDR, 0);

        // 1) Обработка нажатия управляющей кнопки (PF0)
        if (btn0_now == 1 && btn0_old == 0)
        {
            step++;
            if (step >= 6)
            {
                step = 0; // На 6-е нажатие возвращаемся к исходному режиму
            }
            delay(300000); // Пауза от дребезга контактов
        }
        btn0_old = btn0_now;

        // Определяем активность внешних светодиодов в зависимости от шага (step):
        // step == 1: горит 1-й внешний LED (PF7)
        // step == 2: горят 1-й и 2-й внешние LED (PF7, PF8)
        // step == 3: горят все 3 внешних LED (PF7, PF8, PF9)
        // step == 4: все 3 канала работают от кнопок (внешние LED погашены)
        // step == 5: горят все 3 внешних LED (PF7, PF8, PF9)
        uint8_t ext1_active = (step == 1 || step == 2 || step == 3 || step == 5) ? 1 : 0;
        uint8_t ext2_active = (step == 2 || step == 3 || step == 5) ? 1 : 0;
        uint8_t ext3_active = (step == 3 || step == 5) ? 1 : 0;

        // 2) Работа 1-го канала (PF1 -> PB0 на плате или PF7 на макетке):
        if (!ext1_active) // Режим работы от кнопки 1 (PF1)
        {
            BIT_SET(GPIOF_BSRR, (1UL << 23)); // Гасим внешний светодиод 1 (PF7, сброс 16+7=23)
            if (BIT_READ(GPIOF_IDR, 1) != 0)
            {
                BIT_SET(GPIOB_BSRR, (1UL << 0)); // Удерживается кнопка -> Зажигаем зеленый LED (PB0)
            }
            else
            {
                BIT_SET(GPIOB_BSRR, (1UL << 16)); // Отпущена -> Гасим PB0
            }
        }
        else // Режим свечения внешнего светодиода 1 (PF7)
        {
            BIT_SET(GPIOB_BSRR, (1UL << 16)); // Гасим светодиод на плате (PB0)
            BIT_SET(GPIOF_BSRR, (1UL << 7));  // Зажигаем внешний светодиод на PF7
        }

        // 3) Работа 2-го канала (PF2 -> PB7 на плате или PF8 на макетке):
        if (!ext2_active) // Режим работы от кнопки 2 (PF2)
        {
            BIT_SET(GPIOF_BSRR, (1UL << 24)); // Гасим внешний светодиод 2 (PF8, сброс 16+8=24)
            if (BIT_READ(GPIOF_IDR, 2) != 0)
            {
                BIT_SET(GPIOB_BSRR, (1UL << 7)); // Удерживается кнопка -> Зажигаем синий LED (PB7)
            }
            else
            {
                BIT_SET(GPIOB_BSRR, (1UL << 23)); // Отпущена -> Гасим PB7
            }
        }
        else // Режим свечения внешнего светодиода 2 (PF8)
        {
            BIT_SET(GPIOB_BSRR, (1UL << 23)); // Гасим синий LED на плате (PB7)
            BIT_SET(GPIOF_BSRR, (1UL << 8));  // Зажигаем внешний светодиод на PF8
        }

        // 4) Работа 3-го канала (PF3 -> PB14 на плате или PF9 на макетке):
        if (!ext3_active) // Режим работы от кнопки 3 (PF3)
        {
            BIT_SET(GPIOF_BSRR, (1UL << 25)); // Гасим внешний светодиод 3 (PF9, сброс 16+9=25)
            if (BIT_READ(GPIOF_IDR, 3) != 0)
            {
                BIT_SET(GPIOB_BSRR, (1UL << 14)); // Удерживается кнопка -> Зажигаем красный LED (PB14)
            }
            else
            {
                BIT_SET(GPIOB_BSRR, (1UL << 30)); // Отпущена -> Гасим PB14
            }
        }
        else // Режим свечения внешнего светодиода 3 (PF9)
        {
            BIT_SET(GPIOB_BSRR, (1UL << 30)); // Гасим красный LED на плате (PB14)
            BIT_SET(GPIOF_BSRR, (1UL << 9));  // Зажигаем внешний светодиод на PF9
        }
    }
}