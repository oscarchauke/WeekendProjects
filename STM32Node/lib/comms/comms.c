#include "comms.h"
#include "uart.h"

typedef enum comms_state_t
{
    COMMS_STATE_LENGTH,
    COMMS_STATE_DATA,
    COMMS_STATE_CRC

} comms_state_t;

static comms_state_t comms_state = COMMS_STATE_LENGTH;

static comms_packet_t temp_packet = {.length = 0, .crc = 0, .data = {0}};
static comms_packet_t retx_packet = {.length = 1, .crc = 0, .data = {0x19}};

static uint8_t crc8(uint8_t *data, uint8_t length)
{
    uint8_t crc = 0;
    for (uint8_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x07;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void comms_setup(void)
{
}
void comms_update(void)
{
    while (uart_data_available())
    {
        switch (comms_state)
        {
        case COMMS_STATE_LENGTH:
        {
            temp_packet.length = uart_read_byte();
            comms_state = COMMS_STATE_DATA;
        }
        break;
        case COMMS_STATE_DATA:
        {
            for (uint8_t i = 0; i < temp_packet.length; i++)
            {
                temp_packet.data[i] = uart_read_byte();
            }
            comms_state = COMMS_STATE_CRC;
        }
        break;
        case COMMS_STATE_CRC:
        {
            temp_packet.crc = uart_read_byte();
            if (temp_packet.crc != crc8((uint8_t*)&temp_packet.data, (temp_packet.length + 1)))
            {
                comms_write(&retx_packet);
                comms_state = COMMS_STATE_LENGTH;
            }
            comms_state = COMMS_STATE_LENGTH;
        }
        break;
        default:
            comms_state = COMMS_STATE_LENGTH;
            break;
        }
    }
}

bool comms_packets_avaliable(void)
{
}
void comms_write()
{
}
void comms_read()
{
}