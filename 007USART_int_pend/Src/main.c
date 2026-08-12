#include <stdint.h>
#include <stdio.h>

#define addISPR1 0XE000E204U
#define addISER1 0xE000E104U
#define USART3_IRQNO 39

int main(void)
{
	//Manually pend the pending bit using pending reg
	//to Calculate the the Interrupt set pending register we have to use the moduluse operator
	uint8_t *pISPR1 = (uint8_t*)addISPR1;
	*pISPR1 |= (1<<(USART3_IRQNO % 32));


	//Enable the USART3 IRQ number in NVIC
	uint32_t *pISER1 = (uint32_t*)addISER1;
	*pISER1 |= (1<<(USART3_IRQNO % 32));
    /* Loop forever */
	for(;;);
}

void USART3_IRQHandler(void){
	printf("USART Interrupt\n");
}
