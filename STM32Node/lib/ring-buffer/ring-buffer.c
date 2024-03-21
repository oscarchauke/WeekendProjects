#include "ring-buffer.h"

void ring_buffer_setup(ring_buffer_t *ring_buffer, uint8_t *buffer, uint32_t size)
{
    ring_buffer->buffer = buffer;
    ring_buffer->read_index = 0;
    ring_buffer->write_index = 0;
    ring_buffer->mask = size - 1; // Assumes size is always power of 2
}

bool ring_buffer_empty(ring_buffer_t *ring_buffer)
{
    return ring_buffer->write_index == ring_buffer->read_index;
}

bool ring_buffer_write(ring_buffer_t *ring_buffer, uint8_t byte)
{
    uint32_t read_index = ring_buffer->read_index;
    uint32_t write_index = ring_buffer->write_index;

    uint32_t next_write_index = (write_index + 1) & ring_buffer->mask;

    // Discard the lastest byte if buffer full
    if (next_write_index == read_index)
    {
        return false;
    }

    ring_buffer->buffer[write_index] = byte;
    ring_buffer->write_index = next_write_index;
    return true;
}

bool ring_buffer_read(ring_buffer_t *ring_buffer, uint8_t *byte)
{
    uint32_t read_index = ring_buffer->read_index;
    uint32_t write_index = ring_buffer->write_index;

    if (read_index == write_index)
    {
        return false;
    }

    *byte = ring_buffer->buffer[read_index];
    read_index = (read_index + 1) & ring_buffer->mask;
    ring_buffer->read_index = read_index;
    return true;
}
