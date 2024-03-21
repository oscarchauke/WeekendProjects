#ifndef INC_NODE_PROTOCOL_H
#define INC_NODE_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

#define NODE_PACKET_LENGTH              (18)
#define NODE_PACKET_DATA_LENGTH         (16)

#define NODE_PROTOCOL_LENGTH_MASK       (0x0f)
#define NODE_PROTOCOL_FUNCTION_MASK     (0xf0)


typedef struct node_protocol_packet_t
{
    uint8_t identifier;
    uint8_t data[NODE_PACKET_DATA_LENGTH];
    uint8_t crc;
}node_protocol_packet_t;

void node_protocol_setup(void);
void node_protocol_run(void);

//bool node_protocol_packet_avaliable();
//bool node_protocol_write();
//bool node_protocol_read();

#endif