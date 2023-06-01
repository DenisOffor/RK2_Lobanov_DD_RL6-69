/*
 * process_cmd.h
 *
 *  Created on: 1 θών. 2023 γ.
 *      Author: denlo
 */

#ifndef PROCESS_CMD_H_
#define PROCESS_CMD_H_

#include "stm32f0xx.h"
#include "Variables.h"
#include "USART_DMA_setup.h"
#include "ADC_setup.h"

void Adjust_TRGO(uint8_t* data);
void Turn_on_off_ADC(uint8_t* data);
void Transfer_amount_of_data_to_PC(uint8_t* data);
void Transfer_infinite_data_to_PC(uint8_t* data);
void check_command();

#endif /* PROCESS_CMD_H_ */
