#ifndef INIT_H
#define INIT_H

#include <stdint.h>

// Макрос установки битов строго через побитовое ИЛИ (|=), как у тебя на скрине
#define BIT_SET(REG, VAL) ((REG) |= (VAL))

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

// --- Регистры порта GPIOF (Кнопки и внешние LED: PF0, PF1, PF2, PF3) ---
#define GPIOF_MODER (*(volatile uint32_t *)(0x40021400UL + 0x00UL))
#define GPIOF_PUPDR (*(volatile uint32_t *)(0x40021400UL + 0x0CUL))
#define GPIOF_IDR (*(volatile uint32_t *)(0x40021400UL + 0x10UL))
#define GPIOF_BSRR (*(volatile uint32_t *)(0x40021400UL + 0x18UL))

void GPIO_Ini(void);

#endif