/**
 * @defgroup modbus_protocol Modbus communication protocol
 *
 * @brief A client/server data communications protocol between two devices.
 * Supports modbus RTU/ASCII protocols for RS-232/485 interfaces.
 *
 * @{
 */

#ifndef _MODBUS_PROTOCOL_H_
#define _MODBUS_PROTOCOL_H_

#include <stdbool.h>
#include <stdint.h>

#define MODBUS_PROTOCOL_BUS_TIMEOUT_MS    (100)
#define MODBUS_PROTOCOL_BUFFER_SIZE       (256)

/**@brief Modbus modes. */
typedef enum
{
    MODBUS_PROTOCOL_MODE_ASCII,
    MODBUS_PROTOCOL_MODE_RTU
} modbus_mode_t;

/**@brief Modbus callback results. */
typedef enum
{
    MODBUS_CALLBACK_RESULT_SUCCESS,                     /**< Callback operation successfully completed. */
    MODBUS_CALLBACK_RESULT_TIMEOUT,                     /**< Callback timed out. */
    MODBUS_CALLBACK_RESULT_IO_ERROR                     /**< I/O error occured in callback. */
} modbus_callback_result_t;

/**@brief Modbus protocol results. */
typedef enum
{
    MODBUS_PROTOCOL_RESULT_SUCCESS,                     /**< Protocol operation completed successfully. */
    MODBUS_PROTOCOL_RESULT_NO_BUFFER_SPACE,             /**< Insufficient protocol buffer space, MODBUS_PROTOCOL_BUFFER_SIZE increase required. */
    MODBUS_PROTOCOL_RESULT_TIMEOUT,                     /**< Protocol operation not completed during set timeout. */
    MODBUS_PROTOCOL_RESULT_CORRUPTED,                   /**< Received message is corrupted. */
    MODBUS_PROTOCOL_RESULT_IO_ERROR                     /**< I/O error occured during protocol operation. */
} modbus_protocol_result_t;

#define MODBUS_CALLBACK_TO_PROTOCOL_RESULT(СALLBACK_RESULT) \
    (СALLBACK_RESULT == MODBUS_CALLBACK_RESULT_TIMEOUT) ? MODBUS_PROTOCOL_RESULT_TIMEOUT : \
      (СALLBACK_RESULT == MODBUS_CALLBACK_RESULT_IO_ERROR) ? MODBUS_PROTOCOL_RESULT_IO_ERROR :  \
        MODBUS_PROTOCOL_RESULT_SUCCESS

/**@brief Bus write callback.
 *
 * @param[in] data        Pointer to data to write.
 * @param[in] data_length Length of the data in bytes.
 * @param[in] timeout_ms  Minimum waiting time for sending data.
 *
 * @retval MODBUS_CALLBACK_RESULT_SUCCESS  Data succesfully written to bus.
 * @retval MODBUS_CALLBACK_RESULT_TIMEOUT  No data was sent during set timeout.
 * @retval MODBUS_CALLBACK_RESULT_IO_ERROR If I/O error occured.
 */
typedef modbus_callback_result_t (*modbus_write_callback_t)(uint8_t * data, 
                                                            uint16_t data_length, 
                                                            uint32_t timeout_ms);

/**@brief Bus read callback.
 *
 * @param[out] data        Pointer to store read data.
 * @param[in]  data_length Length of the data in bytes.
 * @param[in]  timeout_ms  Minimum waiting time for receiving data.
 *
 * @retval MODBUS_CALLBACK_RESULT_SUCCESS  Data successfully read from bus.
 * @retval MODBUS_CALLBACK_RESULT_TIMEOUT  No data received during set timeout.
 * @retval MODBUS_CALLBACK_RESULT_IO_ERROR I/O error occured.
 */
typedef modbus_callback_result_t (*modbus_read_callback_t)(uint8_t * data, 
                                                           uint16_t data_length, 
                                                           uint32_t timeout_ms);

/**@brief Bus idle callback (for MODBUS_PROTOCOL_MODE_RTU). Callback must provide a time interval  
 *        on the bus with a duration of at least 3.5 bytes for a given baud rate.
 *
 * @param[in] data_length Length of the data in bytes.
 */
typedef void (*modbus_idle_callback_t)(uint16_t data_length);

/**@brief Modbus parameters. */
typedef struct
{
    const modbus_mode_t mode;                           /**< Modbus mode. */
    const modbus_read_callback_t write;                 /**< Pointer to a bus write callback. */
    const modbus_read_callback_t read;                  /**< Pointer to a bus read callback. */
    const modbus_idle_callback_t idle;                  /**< Pointer to a bus idle callback (required only for MODBUS_PROTOCOL_MODE_RTU). */
} modbus_params_t;

/**@brief Initialize modbus protocol.
 *
 * @param[in] params Pointer to the modbus parameters.
 */
void modbus_protocol_initialize(const modbus_params_t * params);

/**@brief Send request.
 *
 * @param[in] data        Pointer to request to send
 * @param[in] data_length Length of the request in bytes.
 *
 * @retval MODBUS_PROTOCOL_RESULT_SUCCESS Request successfully sent.
 * @retval protocol_result                Otherwise, @see modbus_protocol_result_t.
 */
modbus_protocol_result_t modbus_request_write(uint8_t * data, uint16_t data_length);

/**@brief Read answer.
 *
 * @param[out] data        Pointer to store read answer.
 * @param[in]  data_length Length of the answer in bytes.
 *
 * @retval MODBUS_PROTOCOL_RESULT_SUCCESS Answer successfully received.
 * @retval protocol_result                Otherwise, @see modbus_protocol_result_t.
 */
modbus_protocol_result_t modbus_answer_read(uint8_t * data, uint16_t data_length);

#endif

/** @} */
