#include "uart.h"

#define BAUDRATE 115200
#define UART_BUFFER_SIZE (64)

static ring_buffer_t uart_buffer = {0U};
static uint8_t data_buffer[UART_BUFFER_SIZE] = {0U};

void usart2_isr(void)
{
    bool overrun_occured = usart_get_flag(USART2, USART_FLAG_ORE) == 1;
    bool received_data = usart_get_flag(USART2, USART_FLAG_RXNE) == 1;

    if (overrun_occured || received_data)
    {
        if (ring_buffer_write(&uart_buffer, (uint8_t)usart_recv(USART2)))
        {
            // TODO: Handle failure
        }
    }
}

void uart_setup(void)
{
    ring_buffer_setup(&uart_buffer, data_buffer, UART_BUFFER_SIZE);

    rcc_periph_clock_enable(RCC_USART2);
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_set_databits(USART2, 8);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_baudrate(USART2, BAUDRATE);
    usart_set_stopbits(USART2, USART_STOPBITS_1);

    usart_enable_rx_interrupt(USART2);
    nvic_enable_irq(NVIC_USART2_IRQ);
    usart_enable(USART2);
}

void uart_write(uint8_t *data, const uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        uart_write_byte(data[i]);
    }
}

void uart_write_byte(uint8_t data)
{
    usart_send_blocking(USART2, (uint16_t)data);
}

uint32_t uart_read(uint8_t *data, const uint32_t length)
{
    for (uint32_t i = 0; i < length; i++)
    {
        if(!ring_buffer_read(&uart_buffer, &data[i])){
            return i;
        }
    }
    return length;
}

uint8_t uart_read_byte(void)
{
    uint8_t byte = 0;
    ring_buffer_read(&uart_buffer, &byte);
    return byte;
}

bool uart_data_available(void)
{
    return !ring_buffer_empty(&uart_buffer);
}