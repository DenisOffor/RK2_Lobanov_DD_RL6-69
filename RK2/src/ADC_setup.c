/*
 * ADC_setup.c
 *
 *  Created on: 27 мая 2023 г.
 *      Author: denlo
 */

#include "ADC_setup.h"

void DMA1_Channel1_IRQHandler() {
	DMA1->IFCR |= DMA_IFCR_CTCIF1;
	if(!(DMA1_Channel1->CCR & DMA_CCR_CIRC)) //if it's not infinite data transfer
		DMA1_Channel1->CCR &= ~ DMA_CCR_EN;  //turn off DMA channel when sufficient amount of data is transfered
}


void init_ADC() {
	//analog fucn on PA0
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER0;
	//turn on RCC ADC and osc on 14 MGhz
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->CR2 |= RCC_CR2_HSI14ON;
	//sampling time selection
	ADC1->SMPR |= ADC_SMPR1_SMPR;
	//channel 1 selection
	ADC1->CHSELR |= ADC_CHSELR_CHSEL0;
	//work with DMA enable:
	ADC1->CFGR1 |= ADC_CFGR1_DMACFG;
	ADC1->CFGR1 |= ADC_CFGR1_DMAEN;
	//external trigger selection
	ADC1->CFGR1 &= ~ADC_CFGR1_CONT;
	ADC1->CFGR1 |= ADC_CFGR1_DISCEN;
	ADC1->CFGR1 |= ADC_CFGR1_EXTEN_0;
	ADC1->CFGR1 |= ADC_CFGR1_EXTSEL_2;
	//ADC enable
	ADC1->CR |= ADC_CR_ADEN;
	while( (ADC1->ISR & ADC_ISR_ADRDY) != ADC_ISR_ADRDY );
}


void init_DMA_for_ADC() {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	//memory size -> 16 bits
	DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;
	//periphery size -> 16 bits
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;
	//from per to smth
	DMA1_Channel1->CCR &= ~DMA_CCR_DIR;
	//size of memory in bytes
	DMA1_Channel1->CNDTR = SIZE_OF_TRANSMIT_DATA;
	//periphery address (from)
	DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
	//memory address (to)
	DMA1_Channel1->CMAR = (uint32_t) (&USART1->TDR);
	//interrupt on
	DMA1_Channel1->CCR |= DMA_CCR_TCIE;
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	NVIC_SetPriority(DMA1_Channel1_IRQn, 5);
}



void init_tim15_as_TRGO() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
	//initially TRG0 10 ms
	TIM15->ARR = 400;
	TIM15->PSC = 200;
	//Master mode selection -> update
	TIM15->CR2 |= TIM_CR2_MMS_1;
	//timer enable
	TIM15->CR1 |= TIM_CR1_CEN;
}

void Setup_periphery_for_ADC() {
	init_tim15_as_TRGO();
	init_DMA_for_ADC();
	init_ADC();
}

void ADC_start_conversation() {
	ADC1->CR |= ADC_CR_ADSTART;
}

void ADC_stop_conversation() {
	ADC1->CR |= ADC_CR_ADSTP;
}









