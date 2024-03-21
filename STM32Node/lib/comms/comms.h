#ifndef INC_COMMS_H
#define INC_COMMS_H

#include <stdbool.h>
#include <stdint.h>

#define PACKET_DATA_LENGTH (8)

typedef struct comms_packet_t
{
    uint8_t length;
    uint8_t data[PACKET_DATA_LENGTH];
    uint8_t crc;
}comms_packet_t;


void comms_setup(void);
void comms_update(void);

bool comms_packets_avaliable(void);
void comms_write();
void comms_read();
#endif