/*
 * ADC_setup.h
 *
 *  Created on: 27 мая 2023 г.
 *      Author: denlo
 */

#ifndef ADC_SETUP_H_
#define ADC_SETUP_H_

#include "stm32f0xx.h"
#include "Variables.h"


void init_ADC();
void init_DMA_for_ADC();
void init_tim15_as_TRGO();
void Setup_periphery_for_ADC();
void ADC_start_conversation();
void ADC_stop_conversation();

#endif /* ADC_SETUP_H_ */
