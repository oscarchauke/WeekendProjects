#include "uart.h"
#include "utils.h"
#include "node_protocol.h"

static void delay(void){
    for (uint32_t i = 0; i < 100000; i++)
    {
        __asm__("nop");
    }
}
int main(void)
{
    node_rcc_setup();
    node_gpio_setup();
    uart_setup();

    while (1)
    {
    
        node_protocol_run();
        //if(uart_data_available()){
        //    gpio_set(LED_PORT,LED_GREEN);
        //    uart_write_byte(uart_read_byte() + 1);
        //}
        delay();
        //gpio_clear(LED_PORT,LED_GREEN);
    }

    return 0;
}