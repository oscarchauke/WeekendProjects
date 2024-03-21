#ifndef INC_UTILS_H
#define INC_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define LED_PORT        (GPIOC)
#define LED_GREEN       (GPIO9)
#define LED_RED         (GPIO8)

#define CONSOLE_PORT    (GPIOA)
#define CONSOLE_RX      (GPIO3)
#define CONSOLE_TX      (GPIO2)

void node_gpio_setup(void);
void node_rcc_setup(void);

#endif