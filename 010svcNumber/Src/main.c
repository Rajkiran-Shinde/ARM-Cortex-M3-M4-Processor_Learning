#include <stdint.h>
#include <stdio.h>


void SVC_Handler_c(uint32_t *pBaseOfStackFrame){

	printf("In The SVC Handler\n");
	//__asm ("MRS R0,MSP");// We can't do that because that will currupt the SP value due to proluge
	uint8_t *pReturrnAddr  = (uint8_t*)pBaseOfStackFrame[6];
	//Decrement the address by 2
	pReturrnAddr -=2;
	uint8_t svc_no = *pReturrnAddr;
	printf ("SVC No is : %d\n", svc_no);

	//Increment the SVC no by 4 and sending back to thread mode

	//Adding
	svc_no +=4;
	//Save this navlue of the SVC no in the Stack frame in R0
	pBaseOfStackFrame[0] = svc_no;//THisi is the location of the the R0

}


int main(void)
{
	__asm ("SVC #5");
    /* Loop forever */
	//store the value of the R0 to the C variable

	//register uint32_t dataR0 __asm("r0");
	//here we are forcing compiler to create a variable in one of the register and storing the r0 by inline assembly inthe dataR0 C variable
	//We used the register keyword for that
	//Data is not created in stack it is created in the Register R0

	uint32_t dataR0;
	__asm volatile ("MOV %0,R0": "=r"(dataR0)::);


	printf("Data Add = %ld\n", dataR0);

	for(;;);
}

__attribute__((naked)) void SVC_Handler(void){
	__asm ("MRS R0,MSP");
	__asm ("B SVC_Handler_c"); //We are Branching to the SVC_Handler_c function using The 'B' inline asembly fnction
	//On the bracnhing the R0 will be copied in to the pBaseOfStackFrame


}
