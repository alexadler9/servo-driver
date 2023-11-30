#include "servo_driver.h"
#include "modbus/modbus_protocol.h"

static modbus_callback_result_t bus_write(uint8_t * data, uint16_t data_length, uint32_t timeout_ms)
{
#error Add your implementation
    
    return MODBUS_CALLBACK_RESULT_SUCCESS;
}

static modbus_callback_result_t bus_read(uint8_t * data, uint16_t data_length, uint32_t timeout_ms)
{
#error Add your implementation
    
    return MODBUS_CALLBACK_RESULT_SUCCESS;
}

#if (MODBUS_MODE_RTU)
static void bus_idle(uint16_t data_length)
{
#error Add your implementation
    
    /* To ensure the frame integrity during the transmission, 
     * callback must provide a time interval on the bus with 
     * a duration of at least 3.5 bytes for a given baud rate. */
}
#endif

static modbus_params_t modbus_params = 
{
    .mode = MODBUS_MODE_RTU ? MODBUS_PROTOCOL_MODE_RTU : MODBUS_PROTOCOL_MODE_ASCII,
    .write = bus_write,
    .read = bus_read,
#if (MODBUS_MODE_RTU)
    .idle = bus_idle
#else
    .idle = NULL
#endif
};

void servo_initialize(void)
{
    modbus_protocol_initialize(&modbus_params);
}

bool servo_nwords_read(uint8_t axis, uint16_t address, uint16_t * words, uint16_t words_num)
{
    modbus_protocol_result_t protocol_result;
    uint8_t servo_buffer[256] = { 0 };
    
    if ((axis < 1 || 127 < axis) || (words_num >= 30))
    {
        return false;
    }
    
    servo_buffer[0] = axis;
    servo_buffer[1] = 0x03;
    servo_buffer[2] = (uint8_t)(address >> 8);
    servo_buffer[3] = (uint8_t)(address & 0xFF);
    servo_buffer[4] = (uint8_t)(words_num >> 8);
    servo_buffer[5] = (uint8_t)(words_num & 0xFF);
    
    protocol_result = modbus_request_write(servo_buffer, 6);
    if (protocol_result != MODBUS_PROTOCOL_RESULT_SUCCESS)
    {
        return false;
    }
    
    protocol_result = modbus_answer_read(servo_buffer, 2 * words_num + 3);
    if (protocol_result != MODBUS_PROTOCOL_RESULT_SUCCESS)
    {
        return false;
    }
    
    uint8_t answer_axis = servo_buffer[0];
    uint8_t answer_command = servo_buffer[1];
    uint8_t answer_bytes_num = servo_buffer[2];
    if ((answer_axis != axis) || (answer_command != 0x03) || (answer_bytes_num != words_num * 2))
    {
        return false;
    }
    
    for (uint16_t i = 0; i < words_num; i++)
    {
        words[i] = (servo_buffer[2 * i + 3] << 8) | servo_buffer[2 * i + 4];
    }
    
    return true;
}

bool servo_nwords_write(uint8_t axis, uint16_t address, uint16_t * words, uint16_t words_num)
{
    modbus_protocol_result_t protocol_result;
    uint8_t servo_buffer[256] = { 0 };
    
    if ((axis < 1 || 127 < axis) || (words_num >= 30))
    {
        return false;
    }
    
    servo_buffer[0] = axis;
    servo_buffer[1] = 0x10;
    servo_buffer[2] = (uint8_t)(address >> 8);
    servo_buffer[3] = (uint8_t)(address & 0xFF);
    servo_buffer[4] = (uint8_t)(words_num >> 8);
    servo_buffer[5] = (uint8_t)(words_num & 0xFF);
    servo_buffer[6] = (uint8_t)(words_num * 2);
    
    for (uint16_t i = 0; i < words_num; i++)
    {
        servo_buffer[2 * i + 7] = (uint8_t)(words[i] >> 8);
        servo_buffer[2 * i + 8] = (uint8_t)(words[i] & 0xFF);
    }
    
    protocol_result = modbus_request_write(servo_buffer, 2 * words_num + 7);
    if (protocol_result != MODBUS_PROTOCOL_RESULT_SUCCESS)
    {
        return false;
    }
    
    protocol_result = modbus_answer_read(servo_buffer, 6);
    if (protocol_result != MODBUS_PROTOCOL_RESULT_SUCCESS)
    {
        return false;
    }
    
    uint8_t answer_axis = servo_buffer[0];
    uint8_t answer_command = servo_buffer[1];
    uint16_t answer_address = (servo_buffer[2] << 8) | servo_buffer[3];
    uint16_t answer_words_num = (servo_buffer[4] << 8) | servo_buffer[5];
    if ((answer_axis != axis) || (answer_command != 0x10) || (answer_address != address) || (answer_words_num != words_num))
    {
        return false;
    }
    
    return true;
}

bool servo_oneword_write(uint8_t axis, uint16_t address, uint16_t word)
{
    modbus_protocol_result_t protocol_result;
    uint8_t servo_buffer[256] = { 0 };
    
    if (axis < 1 || 127 < axis)
    {
        return false;
    }
    
    servo_buffer[0] = axis;
    servo_buffer[1] = 0x06;
    servo_buffer[2] = (uint8_t)(address >> 8);
    servo_buffer[3] = (uint8_t)(address & 0xFF);
    servo_buffer[4] = (uint8_t)(word >> 8);
    servo_buffer[5] = (uint8_t)(word & 0xFF);
    
    protocol_result = modbus_request_write(servo_buffer, 6);
    if (protocol_result != MODBUS_PROTOCOL_RESULT_SUCCESS)
    {
        return false;
    }
    
    protocol_result = modbus_answer_read(servo_buffer, 6);
    if (protocol_result != MODBUS_PROTOCOL_RESULT_SUCCESS)
    {
        return false;
    }
    
    uint8_t answer_axis = servo_buffer[0];
    uint8_t answer_command = servo_buffer[1];
    uint16_t answer_address = (servo_buffer[2] << 8) | servo_buffer[3];
    uint16_t answer_word = (servo_buffer[4] << 8) | servo_buffer[5];
    if ((answer_axis != axis) || (answer_command != 0x06) || (answer_address != address) || (answer_word != word))
    {
        return false;
    }
    
    return true;
}
