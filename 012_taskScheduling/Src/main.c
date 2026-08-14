#include<stdint.h>
#include<stdio.h>
#include"main.h"

//Prototypes of the Functions
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void init_systick_timer(uint32_t tick_hz);
__attribute ((naked)) void init_scheduler_stack(uint32_t scheduler_top_stack);

__attribute ((naked)) void switch_sp_psp(void);

uint32_t get_psp(void);

void init_task_stack(void);

void enable_fault(void);

uint32_t pspOfTasks [MAX_TASK]= {T1_Stack_Start, T2_Stack_Start, T3_Stack_Start, T4_Stack_Start};

uint32_t task_handlers [MAX_TASK];

uint32_t current_task = 0; // Task one is running

int main(void)
{
	enable_fault ();
	init_scheduler_stack(Sch_Stack_Start);

	task_handlers[0] = (uint32_t) task1_handler;
	task_handlers[1] = (uint32_t) task2_handler;
	task_handlers[2] = (uint32_t) task3_handler;
	task_handlers[3] = (uint32_t) task4_handler;


	init_task_stack();//Dummy values to sf1 sf 2

	init_systick_timer(TICK_HZ);

	switch_sp_psp();

	task1_handler();
    /* Loop forever */
	for(;;);
}

void task1_handler(void){
	while(1){
		printf("Task1");
	}
}

void task2_handler(void){
	while(1){
		printf("Task2");
	}
}

void task3_handler(void){
	while(1){
		printf("Task3");
	}
}

void task4_handler(void){
	while(1){
		printf("Task4");
	}
}



void init_systick_timer(uint32_t tick_hz){
	uint32_t *pSCSR = (uint32_t*)0xE000E010;//Systick control and status register to enable the systic and setting
	uint32_t *pSRVR = (uint32_t*)0xE000E014;// Relode value Systick relode value register
	uint32_t countValue = ((SYSTICK_TM/tick_hz)-1);
	//Clear the value of svr
	*pSRVR &= ~(0x00FFFFFFFF);
	//load the valu in SVR
	*pSRVR |= countValue;

	//Some Settings
	*pSCSR |= (1<<1); //Enable TICKINT as Enables SysTick exception request
	*pSCSR |= (1<<2);// SET The bit CLKSOURCE as a processor clock

	//Enable the Systick
	*pSCSR |= (1<<0); // Set the bit to enable the systick timer

}

__attribute ((naked)) void init_scheduler_stack(uint32_t scheduler_top_stack){
	__asm volatile ("MSR MSP, %0" : : "r"(scheduler_top_stack));
	__asm volatile ("BX LR"); //As we are using the naked function so there is no apolouge or prolouge hence the value is not returend
	//to do that we are using the branch indirect to LR Return from Function call to return value.
}

void init_task_stack(void){
	uint32_t *pPSP;
	for (int i =0; i< MAX_TASK; i++){
		pPSP= (uint32_t*) pspOfTasks [i];
		pPSP--;
		*pPSP = DUMMY_XPSR; //0x01000000

		pPSP--; //PC
		*pPSP = task_handlers [i];

		pPSP--;
		*pPSP = 0xFFFFFFFD;

		for (int j=0;j<13; j++){
			pPSP --;
			*pPSP = 0;
		}

	pspOfTasks [i]= (uint32_t) pPSP;

	}
}

//to get the psp value of current stack
uint32_t get_psp(void){

	return pspOfTasks [current_task];
}


__attribute ((naked)) void switch_sp_psp(void){

	//initilise the psp with task 1 stck start address

	//get the value of psp
	__asm volatile ("PUSH {LR}"); //The LR should not get currepted hence we are pushing it
	__asm volatile ("BL get_psp"); //Branch with link
	__asm volatile ("MSR PSP, R0"); //Init PSP
	__asm volatile ("POP {LR}");//Pops back lr value

	//Change the SP to PSP
	__asm volatile ("MOV R0, #0x02");
	__asm volatile ("MSR CONTROL, R0");
	__asm volatile ("BX LR");


}

void save_psp_value (uint32_t stack_addr){
	pspOfTasks[current_task]=stack_addr;
}

void update_next_task (void){
	current_task ++;
	current_task = current_task % MAX_TASK;
}

void SysTick_Handler(void){

	//Save the context of the current value

	//Get the Running task's psp value
	__asm volatile ("MRS R0, PSP");

	// Store the sf2 usng psp value
	__asm volatile ("STMDB R0!,  {R4 -R11}"); // Store Multiple and decrement before
	// Save the current value of psp
	__asm volatile ("BL save_psp_value");

	//Retrive the Context of the next task

	//Decide a New task
	__asm volatile ("BL update_next_task");

	//Get it's past psp value
	__asm volatile ("BL get_psp");

	//Retrive SF2
	__asm volatile ("LDMIA R0!, {R4-R11}");//Load multiple registers and increment after

	//Update PSP
	__asm volatile ("MSR PSP, R0");

	//EXIT
}

void enable_fault(void){
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24; //this is the part of control register where it is system handler and control register

		//Enable all the configurable faults.
		*pSHCSR |= (1<<16); // Mem Fault
		*pSHCSR |= (1<<17); //Bus Fault
		*pSHCSR |= (1<<18); //Usages Fault

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



