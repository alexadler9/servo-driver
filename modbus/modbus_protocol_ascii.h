/**
 * @ingroup modbus_protocol
 *
 * @defgroup modbus_protocol_ascii Modbus ASCII protocol
 *
 * @brief Modbus ASCII makes use of ASCII characters for protocol communication. 
 *
 * The ASCII format uses a longitudinal redundancy check checksum. 
 * Modbus ASCII messages are framed by a leading colon (":") and trailing newline (CR/LF).
 *
 * This protocol can be used on lines with high delays and devices with less accurate timers.
 *
 * @{
 */

#ifndef _MODBUS_PROTOCOL_ASCII_H_
#define _MODBUS_PROTOCOL_ASCII_H_

#include <stdbool.h>
#include <stdint.h>
#include "modbus_protocol.h"

/**@brief Send request via Modbus ASCII protocol.
 *
 * @param[in] write       Write callback.
 * @param[in] data        Pointer to request to write.
 * @param[in] data_length Length of the request in bytes.
 *
 * @retval MODBUS_PROTOCOL_RESULT_SUCCESS Request successfully sent.
 * @retval protocol_result                Otherwise, @see modbus_protocol_result_t.
 */
modbus_protocol_result_t modbus_protocol_ascii_request_write(modbus_write_callback_t write, 
                                                             uint8_t * data, 
                                                             uint16_t data_length);

/**@brief Read answer via Modbus ASCII protocol.
 *
 * @param[in]  read        Read callback.
 * @param[out] data        Pointer to store read answer.
 * @param[in]  data_length Length of the answer in bytes.
 *
 * @retval MODBUS_PROTOCOL_RESULT_SUCCESS Answer successfully received.
 * @retval protocol_result                Otherwise, @see modbus_protocol_result_t.
 */
modbus_protocol_result_t modbus_protocol_ascii_answer_read(modbus_read_callback_t read, 
                                                           uint8_t * data, 
                                                           uint16_t data_length);

#endif

/** @} */