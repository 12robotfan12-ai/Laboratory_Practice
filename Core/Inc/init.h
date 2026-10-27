#ifndef INIT_H
#define INIT_H

#include <stdint.h>

// Макросы для работы с битами регистров
#define BIT_SET(REG, VAL) ((REG) |= (VAL))
#define BIT_CLEAR(REG, VAL) ((REG) &= ~(VAL))
#define BIT_READ(REG, PIN) (((REG) >> (PIN)) & 1U)

// --- Регистр тактирования RCC ---
#define RCC_AHB1ENR (*(volatile uint32_t *)(0x40023800UL + 0x30UL))
#define RCC_GPIOB 0x02UL // Бит 1 (тактирование GPIOB)
#define RCC_GPIOF 0x20UL // Бит 5 (тактирование GPIOF)

// --- Регистры порта GPIOB (Светодиоды на плате: PB0, PB7, PB14) ---
#define GPIOB_MODER (*(volatile uint32_t *)(0x40020400UL + 0x00UL))
#define GPIOB_OTYPER (*(volatile uint32_t *)(0x40020400UL + 0x04UL))
#define GPIOB_OSPEEDR (*(volatile uint32_t *)(0x40020400UL + 0x08UL))
#define GPIOB_PUPDR (*(volatile uint32_t *)(0x40020400UL + 0x0CUL))
#define GPIOB_BSRR (*(volatile uint32_t *)(0x40020400UL + 0x18UL))

// --- Регистры порта GPIOF (Кнопки: PF0..PF3, Внешние LED: PF7..PF9) ---
#define GPIOF_MODER (*(volatile uint32_t *)(0x40021400UL + 0x00UL))
#define GPIOF_OTYPER (*(volatile uint32_t *)(0x40021400UL + 0x04UL))
#define GPIOF_OSPEEDR (*(volatile uint32_t *)(0x40021400UL + 0x08UL))
#define GPIOF_PUPDR (*(volatile uint32_t *)(0x40021400UL + 0x0CUL))
#define GPIOF_IDR (*(volatile uint32_t *)(0x40021400UL + 0x10UL))
#define GPIOF_BSRR (*(volatile uint32_t *)(0x40021400UL + 0x18UL))

void GPIO_Ini(void);

#endif