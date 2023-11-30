/**
 * @defgroup servo_driver Servo EPS-B1 series driver
 *
 * @brief Basic driver for communicating with EPS-B1 series servo using Modbus protocol
 *
 * It is intended only to demonstrate interaction with the servo. To make this example work:
 * - add implementation of the read/write bus callbacks (see functions bus_read() and bus_write());
 * - if Modbus RTU mode is used, then add implementation of the idle bus callback (bus_idle).
 *
 * You can change the Modbus mode using the MODBUS_MODE_RTU flag (true - RTU mode is used, otherwise - ASCII).
 *
 * @{
 */

#ifndef _SERVO_DRIVER_H_
#define _SERVO_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MODBUS_MODE_RTU   (false)

/**@brief Initialize servo driver.
*/
void servo_initialize(void);

/**@brief Read N words from servo.
 *
 * @param[in]  axis      Communication address (1-127).
 * @param[in]  address   Starting address.
 * @param[out] words     Pointer to read words.
 * @param[in]  words_num Words number (0-29).
 *
 * @retval true if successful, otherwise false.
 */
bool servo_nwords_read(uint8_t axis, uint16_t address, uint16_t * words, uint16_t words_num);

/**@brief Write N words to servo.
 *
 * @param[in] axis      Communication address (1-127).
 * @param[in] address   Starting address.
 * @param[in] words     Pointer to write words.
 * @param[in] words_num Words number (0-29).
 *
 * @retval true if successful, otherwise false.
 */
bool servo_nwords_write(uint8_t axis, uint16_t address, uint16_t * words, uint16_t words_num);

/**@brief Write 1 word to servo.
 *
 * @param[in] axis    Communication address (1-127).
 * @param[in] address Starting address.
 * @param[in] word    Write word.
 * 
 * @retval true if successful, otherwise false.
 */
bool servo_oneword_write(uint8_t axis, uint16_t address, uint16_t word);

#endif

/** @} */
