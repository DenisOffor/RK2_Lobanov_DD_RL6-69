/*
 * process_cmd.c
 *
 *  Created on: 1 θών. 2023 γ.
 *      Author: denlo
 */

#include "process_cmd.h"
void Adjust_TRGO(uint8_t* data) {
	TIM15->CR1 &= ~TIM_CR1_CEN;
	TIM15->PSC = 200 + 200*(*data); //adjust frequency of TFGO
	TIM15->CR1 |= TIM_CR1_CEN;      //by random law
}

void Turn_on_off_ADC(uint8_t* data) {
	if(*data == 1)
		ADC_start_conversation();   //if in data 1 -> start work ADC
	if(*data == 0)
		ADC_stop_conversation();    //if in data 0 -> stop work ADC
}

void Transfer_amount_of_data_to_PC(uint8_t* data) {
	DMA1_Channel1->CNDTR = *data;      //write in CNDTR amount of data needed in PC
	DMA1_Channel1->CCR |= DMA_CCR_EN;  //and start transfer
}

void Transfer_infinite_data_to_PC(uint8_t* data) {
	if(*data == 1) {
		//turn on circular mode for "infinite" transfer of data
		DMA1_Channel1->CCR |= DMA_CCR_CIRC;
		DMA1_Channel1->CNDTR = SIZE_OF_TRANSMIT_DATA;
		DMA1_Channel1->CCR |= DMA_CCR_EN;			  //turn on DMA
	}
	if(*data == 0) {
		DMA1_Channel1->CCR &= ~ DMA_CCR_EN;
		DMA1_Channel1->CNDTR = SIZE_OF_TRANSMIT_DATA;
		DMA1_Channel1->CCR &= ~DMA_CCR_CIRC;
	}
}

void check_command() {
	if(Buffer_empty(&Receive_buf)) //if buffer with command empty -> return
		return;
    //get command by reading 2 first byte and using XOR, for instance 0xAB01 ^ 0xAB00(SYNC_BYTE) = 0x01
	uint16_t command = ((Buffer_get_from_front(&Receive_buf) << 8) | Buffer_get_from_front(&Receive_buf)) ^ SYNC_BYTE;
	uint8_t data = Buffer_get_from_front(&Receive_buf); //get received data for implement some sort of actions

	switch(command) {
		case ADJUST_TRGO:
			Adjust_TRGO(&data);
			break;
		case TURN_ON_OFF_ADC:
			Turn_on_off_ADC(&data);
			break;
		case TRANSFER_CERTAIN_AMOUNT_OF_DATA_TO_PC:
			Transfer_amount_of_data_to_PC(&data);
			break;
		case TRANSFER_INFINITE_AMOUNT_OF_DATA_TO_PC:
			Transfer_infinite_data_to_PC(&data);
			break;
	}

	Clear_buffer(&Receive_buf);  //clear buffer just in case to get new correct command
	program_state = WAITING;
}
