/**
 * @ingroup modbus_protocol
 *
 * @defgroup modbus_protocol_rtu Modbus RTU protocol
 *
 * @brief Modbus RTU (Remote Terminal Unit), makes use of a compact, binary representation 
 * of the data for protocol communication. 
 *
 * The RTU format uses CRC-16-MODBUS whose polynomial is x16 + x15 + x2 + 1.
 * Modbus RTU message must be transmitted continuously without inter-character hesitations. 
 * Modbus messages are framed (separated) by idle (silent) periods. 
 *
 * This protocol is critical to delays, but the overhead of data transmission is less than in Modbus ASCII.
 *
 * @{
 */

#ifndef _MODBUS_PROTOCOL_RTU_H_
#define _MODBUS_PROTOCOL_RTU_H_

#include <stdbool.h>
#include <stdint.h>
#include "modbus_protocol.h"

/**@brief Send request via Modbus RTU protocol.
 *
 * @param[in] write       Write callback.
 * @param[in] idle        Idle callback.
 * @param[in] data        Pointer to request to write.
 * @param[in] data_length Length of the request in bytes.
 *
 * @retval MODBUS_PROTOCOL_RESULT_SUCCESS Request successfully sent.
 * @retval protocol_result                Otherwise, @see modbus_protocol_result_t.
 */
modbus_protocol_result_t modbus_protocol_rtu_request_write(modbus_write_callback_t write, 
                                                           modbus_idle_callback_t idle, 
                                                           uint8_t * data, 
                                                           uint16_t data_length);

/**@brief Read answer via Modbus RTU protocol.
 *
 * @param[in]  read        Read callback.
 * @param[out] data        Pointer to store read answer.
 * @param[in]  data_length Length of the answer in bytes.
 *
 * @retval MODBUS_PROTOCOL_RESULT_SUCCESS Answer successfully received.
 * @retval protocol_result                Otherwise, @see modbus_protocol_result_t.
 */
modbus_protocol_result_t modbus_protocol_rtu_answer_read(modbus_read_callback_t read, 
                                                         uint8_t * data, 
                                                         uint16_t data_length);

#endif

/** @} */