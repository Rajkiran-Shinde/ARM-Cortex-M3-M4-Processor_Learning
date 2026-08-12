#include <stdint.h>
#include <stdio.h>


void SVC_Handler_c(uint32_t *pBaseOfStackFrame){

	int32_t arg0, arg1, result;

	printf("In The SVC Handler\n");
	//__asm ("MRS R0,MSP");// We can't do that because that will currupt the SP value due to proluge
	uint8_t *pReturrnAddr  = (uint8_t*)pBaseOfStackFrame[6];
	//Decrement the address by 2
	pReturrnAddr -=2;
	uint8_t svc_no = *pReturrnAddr;
	printf ("SVC No is : %d\n", svc_no);

	arg0 = pBaseOfStackFrame[0];
	arg1 = pBaseOfStackFrame[1];
	switch (svc_no){
	case 36:
		result=arg0+arg1;
		break;
	case 37:
		result = arg0-arg1;
		break;
	case 38:
		result = arg0*arg1;
		break;
	case 39:
		result = arg0/arg1;
		break;
	default:
		printf("Invalid SVC Instruction");

	}


	pBaseOfStackFrame[0]=result;
}


int32_t add_number (int32_t x, int32_t y){
	int32_t result;
	__asm volatile ("SVC #36");
	__asm volatile ("MOV %0,R0": "=r"(result)::);
	return result;

}

int32_t Sub_number (int32_t x, int32_t y){
	int32_t result;
	__asm volatile ("SVC #37");
	__asm volatile ("MOV %0,R0": "=r"(result)::);
	return result;

}

int32_t Mul_number (int32_t x, int32_t y){
	int32_t result;
	__asm volatile ("SVC #38");
	__asm volatile ("MOV %0,R0": "=r"(result)::);
	return result;

}

int32_t Div_number (int32_t x, int32_t y){
	int32_t result;
	__asm volatile ("SVC #39");
	__asm volatile ("MOV %0,R0": "=r"(result)::);
	return result;
}



int main(void)
{
	int32_t result;
	result = add_number(40, 90);
	printf("Add Result = %ld\n", result);

	result = Sub_number(50, 90);
	printf("Sub Result = %ld\n", result);

	result = Mul_number(10, 17);
	printf("Mul Result = %ld\n", result);

	result = Div_number(180, 26);
	printf("Div Result = %ld\n", result);

	for(;;);
}

__attribute__((naked)) void SVC_Handler(void){
	__asm ("MRS R0,MSP");
	__asm ("B SVC_Handler_c"); //We are Branching to the SVC_Handler_c function using The 'B' inline asembly fnction
	//On the bracnhing the R0 will be copied in to the pBaseOfStackFrame
}
