#include "node_protocol.h"
#include "uart.h"

typedef enum node_protocol_state_t
{
    NODE_PROTOCOL_READ_ID,
    NODE_PROTOCOL_READ_DATA,
    NODE_PROTOCOL_CRC,
    NODE_PROTOCOL_NACK,
    NODE_PROTOCOL_ACK,
    NODE_PROTOCOL_STORE
} node_protocol_state_t;

static node_protocol_state_t protocol_state = NODE_PROTOCOL_READ_ID;

static node_protocol_packet_t temp_packet;

// 24 41 43 4B 20 71
static node_protocol_packet_t packet_ack;

// 24 4E 41 43 4B CB
static node_protocol_packet_t packet_nack;

// 24 52 45 54 58 AA
static node_protocol_packet_t packet_retx;

static uint8_t byte_counter = 0;
static uint8_t length = 0;

static uint8_t crc8(uint8_t *data, uint8_t leng)
{
    uint8_t crc = 0;
    for (uint8_t i = 0; i < leng; i++)
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


void node_protocol_setup(void)
{
}


void node_protocol_run(void)
{
    if (uart_data_available())
    {
        switch (protocol_state)
        {
        case NODE_PROTOCOL_READ_ID:
            temp_packet.identifier = uart_read_byte();
            // start Testing code
            uint8_t a = 30 + (temp_packet.identifier & NODE_PACKET_DATA_LENGTH);
            uart_write_byte(a);
            // end Testing Code
            protocol_state = NODE_PROTOCOL_READ_DATA;
            break;

        case NODE_PROTOCOL_READ_DATA:
            length = temp_packet.identifier & NODE_PACKET_DATA_LENGTH;
            if (length > byte_counter)
            {
                temp_packet.data[byte_counter++] = uart_read_byte();
                break;
            }
            else
            {
                byte_counter = 0;
                protocol_state = NODE_PROTOCOL_CRC;
                break;
            }

        case NODE_PROTOCOL_CRC:
            temp_packet.crc = uart_read_byte();
            length = temp_packet.identifier & NODE_PACKET_DATA_LENGTH;
            if (temp_packet.crc == crc8((uint8_t *)&temp_packet, (length + 1)))
            {
                // TODO: Check if the data was expected
                protocol_state = NODE_PROTOCOL_READ_ID;
                //node_protocol_write(packet_ack);
                break;
            }
            else
            {
                protocol_state = NODE_PROTOCOL_READ_ID;
                //node_protocol_write(packet_retx);
                break;
            }
        default:
            protocol_state = NODE_PROTOCOL_READ_ID;
            break;
        }
    }
}
