#include "init.h"

// Задержка для защиты от дребезга контактов кнопки
void delay(volatile uint32_t count)
{
    while (count--)
        ;
}

int main(void)
{
    GPIO_Ini(); // Инициализация портов GPIO через CMSIS

    uint8_t step = 0;     // Счётчик нажатий управляющей кнопки (PF0) (от 0 до 6)
    uint8_t btn0_old = 0; // Предыдущее состояние кнопки 0 (PF0)

    while (1)
    {
        // Считываем состояние управляющей кнопки PF0 через макрос CMSIS READ_BIT
        uint8_t btn0_now = (READ_BIT(GPIOF->IDR, GPIO_IDR_ID0) != 0U) ? 1U : 0U;

        // 1) Обработка нажатия управляющей кнопки (PF0)
        if (btn0_now == 1U && btn0_old == 0U)
        {
            step++;
            if (step >= 6U)
            {
                step = 0U; // На 6-е нажатие цикл повторяется
            }
            delay(300000); // Пауза от дребезга контактов
        }
        btn0_old = btn0_now;

        // Флаги активности внешних светодиодов (PF7, PF8, PF9) по шагам:
        uint8_t ext1_active = (step == 1U || step == 2U || step == 3U || step == 5U) ? 1U : 0U;
        uint8_t ext2_active = (step == 2U || step == 3U || step == 5U) ? 1U : 0U;
        uint8_t ext3_active = (step == 3U || step == 5U) ? 1U : 0U;

        // 2) Работа 1-го канала (Кнопка PF1 -> PB0 на плате / внешний LED на PF7):
        if (!ext1_active) // Режим работы от кнопки PF1
        {
            WRITE_REG(GPIOF->BSRR, GPIO_BSRR_BR7); // Гасим внешний светодиод 1 (PF7)
            if (READ_BIT(GPIOF->IDR, GPIO_IDR_ID1) != 0U)
            {
                WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BS0); // Зажигаем зеленый LED на плате (PB0)
            }
            else
            {
                WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BR0); // Гасим PB0
            }
        }
        else // Режим свечения внешнего светодиода 1 (PF7)
        {
            WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BR0); // Гасим светодиод на плате (PB0)
            WRITE_REG(GPIOF->BSRR, GPIO_BSRR_BS7); // Зажигаем внешний светодиод 1 (PF7)
        }

        // 3) Работа 2-го канала (Кнопка PF2 -> PB7 на плате / внешний LED на PF8):
        if (!ext2_active) // Режим работы от кнопки PF2
        {
            WRITE_REG(GPIOF->BSRR, GPIO_BSRR_BR8); // Гасим внешний светодиод 2 (PF8)
            if (READ_BIT(GPIOF->IDR, GPIO_IDR_ID2) != 0U)
            {
                WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BS7); // Зажигаем синий LED на плате (PB7)
            }
            else
            {
                WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BR7); // Гасим PB7
            }
        }
        else // Режим свечения внешнего светодиода 2 (PF8)
        {
            WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BR7); // Гасим синий LED на плате (PB7)
            WRITE_REG(GPIOF->BSRR, GPIO_BSRR_BS8); // Зажигаем внешний светодиод 2 (PF8)
        }

        // 4) Работа 3-го канала (Кнопка PF3 -> PB14 на плате / внешний LED на PF9):
        if (!ext3_active) // Режим работы от кнопки PF3
        {
            WRITE_REG(GPIOF->BSRR, GPIO_BSRR_BR9); // Гасим внешний светодиод 3 (PF9)
            if (READ_BIT(GPIOF->IDR, GPIO_IDR_ID3) != 0U)
            {
                WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BS14); // Зажигаем красный LED на плате (PB14)
            }
            else
            {
                WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BR14); // Гасим PB14
            }
        }
        else // Режим свечения внешнего светодиода 3 (PF9)
        {
            WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BR14); // Гасим красный LED на плате (PB14)
            WRITE_REG(GPIOF->BSRR, GPIO_BSRR_BS9);  // Зажигаем внешний светодиод 3 (PF9)
        }
    }
}