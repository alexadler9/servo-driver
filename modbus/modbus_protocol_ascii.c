#include "modbus_protocol_ascii.h"

/**@brief Converting ASCII character to byte.
 */
static uint8_t char_to_hex(uint8_t character)
{
    if ('0' <= character && character <= '9')
    {
        return (uint8_t)(character - '0');
    }
    
    if ('A' <= character && character <= 'F')
    {
        return (uint8_t)(character - 'A' + 0x0A);
    }
    
    return 0xFF;
}

/**@brief Converting byte to ASCII character.
 */
static uint8_t hex_to_char(uint8_t byte)
{
    if (byte <= 0x09)
    {
        return (uint8_t)('0' + byte);
    }
    
    if ((byte >= 0x0A) && (byte <= 0x0F))
    {
        return (uint8_t)(byte - 0x0A + 'A');
    }
    
    return '0';
}

/**@brief Calculating LRC (Longitudinal Redundancy Check).
 */
static uint8_t checksum_calculate(uint8_t * data, uint16_t data_length)
{
    uint8_t checksum = 0;
    
    while (data_length--)
    {
        checksum += *data++;
    }
    
    //Return twos complement
    checksum = (uint8_t)(-((int8_t)checksum));
    
    return checksum;
}

modbus_protocol_result_t modbus_protocol_ascii_request_write(modbus_write_callback_t write, 
                                                             uint8_t * data, 
                                                             uint16_t data_length)
{
    modbus_callback_result_t callback_result;
    uint8_t modbus_buffer[MODBUS_PROTOCOL_BUFFER_SIZE] = { 0 };
    
    const uint8_t checksum = checksum_calculate(data, data_length);
    const uint16_t length = data_length * 2 + 5;
    if (length > MODBUS_PROTOCOL_BUFFER_SIZE)
    {
        return MODBUS_PROTOCOL_RESULT_NO_BUFFER_SPACE;
    }
    
    uint16_t pos = 0;
    modbus_buffer[pos++] = ':';
    for (uint16_t i = 0; i < data_length; i++)
    {
        modbus_buffer[pos++] = hex_to_char((uint8_t)(data[i] >> 4));
        modbus_buffer[pos++] = hex_to_char((uint8_t)(data[i] & 0x0F));
    }
    modbus_buffer[pos++] = hex_to_char((uint8_t)(checksum >> 4));
    modbus_buffer[pos++] = hex_to_char((uint8_t)(checksum & 0x0F));
    modbus_buffer[pos++] = '\r';
    modbus_buffer[pos++] = '\n';
    
    callback_result = write(modbus_buffer, pos, MODBUS_PROTOCOL_BUS_TIMEOUT_MS);
    
    return MODBUS_CALLBACK_TO_PROTOCOL_RESULT(callback_result);
}

modbus_protocol_result_t modbus_protocol_ascii_answer_read(modbus_read_callback_t read, 
                                                           uint8_t * data, 
                                                           uint16_t data_length)
{
    modbus_callback_result_t callback_result;
    uint8_t modbus_buffer[MODBUS_PROTOCOL_BUFFER_SIZE] = { 0 };
    
    const uint16_t length = data_length * 2 + 5;
    if (length > MODBUS_PROTOCOL_BUFFER_SIZE)
    {
        return MODBUS_PROTOCOL_RESULT_NO_BUFFER_SPACE;
    }
    
    callback_result = read(modbus_buffer, length, MODBUS_PROTOCOL_BUS_TIMEOUT_MS);
    if (callback_result != MODBUS_CALLBACK_RESULT_SUCCESS)
    {
        return MODBUS_CALLBACK_TO_PROTOCOL_RESULT(callback_result);
    }
    
    uint16_t pos = 1, rcvpos = 0;
    if ((modbus_buffer[0] == ':') && (modbus_buffer[length - 2] == '\r') && (modbus_buffer[length - 1] == '\n'))
    {
        while (pos < length - 4)
        {
            data[rcvpos] = (uint8_t)(char_to_hex(modbus_buffer[pos++]) << 4);
            data[rcvpos++] |= char_to_hex(modbus_buffer[pos++]);
        }
        
        uint8_t checksum = (uint8_t)(char_to_hex(modbus_buffer[pos++]) << 4);
        checksum |= char_to_hex(modbus_buffer[pos++]);        
        const uint8_t checksum_calculated = checksum_calculate(data, data_length);
        
        if (checksum == checksum_calculated)
        {
            return MODBUS_PROTOCOL_RESULT_SUCCESS;
        }
    }
    
    return MODBUS_PROTOCOL_RESULT_CORRUPTED;
}
