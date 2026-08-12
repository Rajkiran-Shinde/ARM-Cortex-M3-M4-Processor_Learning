#include <stdint.h>
#include <stdio.h>

int main(void)
{
	//1. Enableing all the configurable Fault exceptons
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24; //this is the part of control register where it is system handler and control register

	//Enable all the configurable faults.
	*pSHCSR |= (1<<16); // Mem Fault
	*pSHCSR |= (1<<17); //Bus Fault
	*pSHCSR |= (1<<18); //Usages Fault

	//Some Undefined instructions

	uint32_t *pSRAM = (uint32_t*)0x20010000;
	*pSRAM = 0xffffffff;

	void (*some_adsress)(void);
	//some_adsress= (void*)0x20010001; //making the t bit 1
	some_adsress= (void*)0x20010000; // Making the t Bit 0 changing to arm State which is LIIIGLE ha ha No police is coming
	some_adsress();

    /* Loop forever */
	for(;;);
}

//implementing the Fault Handlers
void HardFault_Handler(void){
	printf("HardFault_Handler\n");
	while(1);
}

void MemManage_Handler (void){
	printf("MemManage_Handler\n");
	while(1);
}

void BusFault_Handler (void){
	printf("BusFault_Handler\n");
	while(1);
}

void UsageFault_Handler (void){
	uint32_t *pUFSR = (uint32_t*)0xE000ED2A;
	//UFSR is usage fault status register is used to define which type of fault it is
	//as now we are having the undefined instreuction so it have to print the 1 that means the undefined instruction
	//this info is available in arm cortex datashtted and refrence manual
	printf("UsageFault_Handler\n");
	printf("UFSR =%lx\n",*pUFSR)&0xFFFF;
	while(1);
}




