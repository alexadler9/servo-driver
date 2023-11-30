#include "modbus_protocol.h"
#include "modbus_protocol_ascii.h"
#include "modbus_protocol_rtu.h"

static const modbus_params_t * modbus_params;

void modbus_protocol_initialize(const modbus_params_t * params)
{
    modbus_params = params;
}

modbus_protocol_result_t modbus_request_write(uint8_t * data, uint16_t data_length)
{
    return (modbus_params->mode == MODBUS_PROTOCOL_MODE_RTU) ? 
            modbus_protocol_rtu_request_write(modbus_params->write, modbus_params->idle, data, data_length) :
            modbus_protocol_ascii_request_write(modbus_params->write, data, data_length);
}

modbus_protocol_result_t modbus_answer_read(uint8_t * data, uint16_t data_length)
{
    return (modbus_params->mode == MODBUS_PROTOCOL_MODE_RTU) ? 
            modbus_protocol_rtu_answer_read(modbus_params->read, data, data_length) :
            modbus_protocol_ascii_answer_read(modbus_params->read, data, data_length);
}
