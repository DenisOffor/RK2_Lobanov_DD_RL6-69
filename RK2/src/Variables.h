/*
 * Variables.h
 *
 *  Created on: 27 мая 2023 г.
 *      Author: denlo
 */

#ifndef VARIABLES_H_
#define VARIABLES_H_

#include "buffer.h"

#define RECEIVE_BUF_SIZE 3
#define SIZE_OF_TRANSMIT_DATA 255
#define SYNC_BYTE 0xAB00

typedef enum Commands {
	WAITING = 0,
	ADJUST_TRGO = 0x01,
	TURN_ON_OFF_ADC = 0x02,
	TRANSFER_CERTAIN_AMOUNT_OF_DATA_TO_PC = 0x03,
	TRANSFER_INFINITE_AMOUNT_OF_DATA_TO_PC = 0x04,
	RECEIVE_COMPLETE
} Commands;


buf Receive_buf;
uint8_t rec_arr[RECEIVE_BUF_SIZE];
Commands program_state;


#endif /* VARIABLES_H_ */
