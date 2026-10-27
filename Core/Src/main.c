#include <stdint.h>
#include "init.h"

// Программная задержка через цикл for
void delay_for(volatile uint32_t iterations)
{
    for (volatile uint32_t i = 0; i < iterations; i++)
    {
        // Пустой цикл ожидания
    }
}

int main(void)
{
    GPIO_Ini(); // Инициализация портов

    // Изначально все светодиоды выключены: active_leds = 0
    uint8_t active_leds = 0; // Количество активных светодиодов в линии (0..6)

    // Индивидуальная скорость мерцания для каждого светодиода:
    // 0 - медленно, 1 - средне, 2 - быстро
    uint8_t led_speed[6] = {1, 1, 1, 1, 1, 1};

    // Периоды полуволны мигания (в шагах цикла):
    // 0 (медленно): ~1 Гц
    // 1 (средне):   ~3 Гц
    // 2 (быстро):   ~7.5 Гц
    const uint32_t blink_periods[3] = {250, 90, 35};

    uint32_t blink_counter[6] = {0, 0, 0, 0, 0, 0};
    uint8_t blink_state[6] = {1, 1, 1, 1, 1, 1};

    // Предыдущие состояния кнопок для детекции одиночного клика (фронта)
    uint8_t b1_old = 0, b2_old = 0, b3_old = 0, b4_old = 0;

    while (1)
    {
        // Прямое чтение кнопок из регистра IDR (кнопки теперь полностью независимы!):
        // PF0 -> Кнопка 1
        // PF1 -> Кнопка 2
        // PF2 -> Кнопка 3
        // PF3 -> Кнопка 4
        uint32_t idr = GPIOF_IDR;
        uint8_t b1_now = (idr >> 0) & 1U;
        uint8_t b2_now = (idr >> 1) & 1U;
        uint8_t b3_now = (idr >> 2) & 1U;
        uint8_t b4_now = (idr >> 3) & 1U;

        // --- Кнопка 3 (PF2): последовательно ЗАЖИГАЕТ светодиоды ---
        if (b3_now == 1 && b3_old == 0)
        {
            if (active_leds < 6)
            {
                active_leds++;
                uint8_t new_idx = active_leds - 1;
                blink_counter[new_idx] = 0;
                blink_state[new_idx] = 1;
                LED_SetState(new_idx, 1);
            }
            delay_for(80000); // Защита от дребезга контактов
        }

        // --- Кнопка 4 (PF3): последовательно ВЫКЛЮЧАЕТ светодиоды с конца ---
        if (b4_now == 1 && b4_old == 0)
        {
            if (active_leds > 0)
            {
                LED_SetState(active_leds - 1, 0);
                active_leds--;
            }
            delay_for(80000); // Защита от дребезга контактов
        }

        // --- Кнопка 1 (PF0): УВЕЛИЧИВАЕТ частоту мерцания ВЫБРАННОГО светодиода ---
        if (b1_now == 1 && b1_old == 0)
        {
            if (active_leds > 0)
            {
                uint8_t sel = active_leds - 1; // Выбранный светодиод (в конце линии)
                if (led_speed[sel] < 2)
                {
                    led_speed[sel]++;
                    blink_counter[sel] = 0;
                }
            }
            delay_for(80000); // Защита от дребезга контактов
        }

        // --- Кнопка 2 (PF1): УМЕНЬШАЕТ частоту мерцания ВЫБРАННОГО светодиода ---
        if (b2_now == 1 && b2_old == 0)
        {
            if (active_leds > 0)
            {
                uint8_t sel = active_leds - 1; // Выбранный светодиод (в конце линии)
                if (led_speed[sel] > 0)
                {
                    led_speed[sel]--;
                    blink_counter[sel] = 0;
                }
            }
            delay_for(80000); // Защита от дребезга контактов
        }

        b1_old = b1_now;
        b2_old = b2_now;
        b3_old = b3_now;
        b4_old = b4_now;

        // --- Мерцание всех включенных светодиодов ---
        for (uint8_t i = 0; i < active_leds; i++)
        {
            uint32_t half_period = blink_periods[led_speed[i]];
            blink_counter[i]++;
            if (blink_counter[i] >= half_period)
            {
                blink_counter[i] = 0;
                blink_state[i] = !blink_state[i];
            }
            LED_SetState(i, blink_state[i]);
        }

        // Все выключенные светодиоды в конце гарантированно гасим
        for (uint8_t i = active_leds; i < 6; i++)
        {
            LED_SetState(i, 0);
        }

        // Базовый шаг квантования времени (~1-2 мс) через for
        delay_for(1600);
    }
}