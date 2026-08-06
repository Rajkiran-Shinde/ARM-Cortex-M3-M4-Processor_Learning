
#include <stdint.h>
#include <stdio.h>

int main(void)
{
	//Enable The config Faults
	uint32_t *pSHCSR = (uint32_t*) 0xE000ED24;

	*pSHCSR |= (1<<16); //Mem Management
	*pSHCSR |= (1<<17); // Bus Fault
	*pSHCSR |= (1<<18); //Usage Fault


	uint32_t *pSRAM =(uint32_t*)0x20001000;
	*pSRAM = 0xFFFFFFFF;

	void (*some_address)(void);

	some_address =(void*)0x2000101;

	some_address();


    /* Loop forever */
	for(;;);
}

//Implement the Fault handlers
 void HardFault_Handler (void){
	 printf("Exception : HardFault\n");
	 while (1);
 }

 void MemManage_Handler (void){
	 printf("Exception : MemManage\n");
	 while (1);
 }

 void BusFault_Handler (void){
	 printf("Exception : BusFault\n");
	 while (1);
 }

 void UsageFault_Handler(void){
	 printf("Exception : UsageFault\n");
	 while (1);
 }
