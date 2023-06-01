

/* Includes */
#include "process_cmd.h"

void setup_periphery() {
	InitBuffer(&Receive_buf, &rec_arr[0], RECEIVE_BUF_SIZE);
	setup_USART_DMA((uint32_t)(&Receive_buf.buf[0]));
	Setup_periphery_for_ADC();
}

int main(void)
{
	setup_periphery();
	start_receive_data_from_PC();
	while (1)
	{
		check_command();
	}
}



















