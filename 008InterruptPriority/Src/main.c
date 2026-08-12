#include <stdint.h>
#include <stdio.h>

#define IRQNO_TIMER2 28
#define IRQNO_T2C1 31

uint32_t *pNVIC_IPRbase =(uint32_t*)0xE000E400;
uint32_t *pNVIC_ISERbase =(uint32_t*)0xE000E100;
uint32_t *pNVIC_ISPRbase =(uint32_t*)0XE000E200;

void config_priority_IRQ(uint8_t irqNo, uint8_t priorityValue){
	//find IPRx
	uint8_t iprx = irqNo /4;
	uint32_t *ipr = pNVIC_IPRbase+iprx;

	//Find Position
	uint8_t pos = (irqNo%4)*8;

	//config priority
	*ipr &= ~(0XFF<<pos);
	*ipr |= (priorityValue<<pos);
}

int main(void)
{
	config_priority_IRQ(IRQNO_TIMER2, 0x80);
	config_priority_IRQ(IRQNO_T2C1, 0x70);

	//set the interrupt pending bit
	*pNVIC_ISPRbase |=(1<<IRQNO_TIMER2);

	//Enable the IRQ in the NVIC ISER
	*pNVIC_ISERbase |=(1<<IRQNO_T2C1);
	*pNVIC_ISERbase |=(1<<IRQNO_TIMER2);

    /* Loop forever */
	for(;;);
}

void TIM2_IRQHandler (void){
	printf("Timer 2 Interrupt\n");
	*pNVIC_ISPRbase |=(1<<IRQNO_T2C1);
	while(1);
}

void I2C1_EV_IRQHandler(void){
	printf("I2C 2\n");
}
