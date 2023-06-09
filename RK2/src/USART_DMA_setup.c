/*
 * USART_DMA_setup.c
 *
 *  Created on: 18 ��� 2023 �.
 *      Author: denlo
 */

#include "USART_DMA_setup.h"


void TIM6_DAC_IRQHandler(void) {
	TIM6->SR &= ~TIM_SR_UIF;
	if(DMA1_Channel3->CNDTR == RECEIVE_BUF_SIZE)
		return;
	if(program_state != RECEIVE_COMPLETE) {
		Clear_buffer(&Receive_buf);
		DMA1_Channel3->CCR &= ~DMA_CCR_EN;
		DMA1_Channel3->CNDTR = RECEIVE_BUF_SIZE;
		DMA1_Channel3->CCR |= DMA_CCR_EN;
	}
}

void USART1_IRQHandler(void) {
	if(USART1->ISR & USART_ISR_RXNE) {
		TIM6->CNT = 0;
	}
}

void DMA1_Channel2_3_IRQHandler(void) {
	if ((DMA1->ISR & DMA_ISR_TCIF3) == DMA_ISR_TCIF3) {
		DMA1->IFCR |= DMA_IFCR_CTCIF3;
		program_state = RECEIVE_COMPLETE;
		Receive_buf.amount_data_in_buf += RECEIVE_BUF_SIZE; //receive buf full of data
	}
}

void init_GPIO_for_USART(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//USART1_TX
	GPIOA->MODER |= GPIO_MODER_MODER9_1;
	//USART1_RX
	GPIOA->MODER |= GPIO_MODER_MODER10_1;
	//AF on PA9 and PA10
	GPIOA->AFR[1] |= 0x01 << GPIO_AFRH_AFRH1_Pos;
	GPIOA->AFR[1] |= 0x01 << GPIO_AFRH_AFRH2_Pos;
}

void init_USART1() {
	//RCC on
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//receiver on pa10
	USART1->CR1 |= USART_CR1_RE;
	//transmitter pa9
	USART1->CR1 |= USART_CR1_TE;
	USART1->BRR = SystemCoreClock / 115200;
	//init DMA for work with transmitter and receiver
	USART1->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
	//interrupt
	USART1->CR1 |= USART_CR1_RXNEIE;

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 7);

	//USART1 on
	USART1->CR1 |= USART_CR1_UE;
}

void init_DMA_to_work_with_USART(uint32_t receive_page) {
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	//memory increment mode
	DMA1_Channel3->CCR |= DMA_CCR_MINC;
	//dir of transfer data
	DMA1_Channel3->CCR &= ~DMA_CCR_DIR;
	//circular mode on
	DMA1_Channel3->CCR |= DMA_CCR_CIRC;
	//size of data in byte
	DMA1_Channel3->CNDTR = RECEIVE_BUF_SIZE;
	//address of periphery
	DMA1_Channel3->CPAR = (uint32_t)(&(USART1->RDR));
	//address of data
	DMA1_Channel3->CMAR = (uint32_t)(receive_page);
	//interrupt on
	DMA1_Channel3->CCR |= DMA_CCR_TCIE;
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 3);
}

void init_timer6_for_right_receive_check() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	TIM6->ARR = 2000;
	TIM6->PSC = 400;

	TIM6->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
	NVIC_SetPriority(TIM6_DAC_IRQn, 5);

	TIM6->CR1 |= TIM_CR1_CEN;
}

void start_receive_data_from_PC(){
	DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void setup_USART_DMA(uint32_t receive_page) {
	 init_timer6_for_right_receive_check();
	 init_GPIO_for_USART();
	 init_USART1();
	 init_DMA_to_work_with_USART(receive_page);
}
