#include "utils.h"

void node_rcc_setup(void)
{
    rcc_clock_setup_in_hsi_out_48mhz();
}

void node_gpio_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED_GREEN|LED_RED);

    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(CONSOLE_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, CONSOLE_RX|CONSOLE_TX);
    gpio_set_af(CONSOLE_PORT, GPIO_AF1, CONSOLE_RX|CONSOLE_TX);
}