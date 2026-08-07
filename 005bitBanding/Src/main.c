
#include <stdint.h>
#include<stdio.h>


#define aliasBase 0x22000000U
#define bitBandBase 0x20000000U
int main(void)
{
	uint8_t *ptr = (uint8_t*)0x20000200;
	*ptr  =0xff;

	//Normal Method
	*ptr &= ~(1<<7); //Clearing the 7th bit

	*ptr  =0xff;

	//Bit Banding Method
	//calculate the alias address
	uint8_t *aliasAddr =(uint8_t*)(aliasBase+(32*(0x20000200-bitBandBase))+7*4);

	*aliasAddr =0; //Clearing the 7th bit
    /* Loop forever */
	for(;;);
}
