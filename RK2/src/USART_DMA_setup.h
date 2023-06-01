/*
 * USART_DMA_setup.h
 *
 *  Created on: 18 мая 2023 г.
 *      Author: denlo
 */

#ifndef USART_DMA_SETUP_H_
#define USART_DMA_SETUP_H_


#include "stm32f0xx.h"
#include "Variables.h"
void TIM6_DAC_IRQHandler(void);
void USART1_IRQHandler(void);
void DMA1_Channel2_3_IRQHandler(void);
void init_GPIO_for_USART();
void init_USART1();
void init_DMA_to_work_with_USART(uint32_t receive_page);
void setup_USART_DMA(uint32_t receive_page);
void start_receive_data_from_PC();
void init_timer6_for_right_receive_check();

#endif /* USART_DMA_SETUP_H_ */
