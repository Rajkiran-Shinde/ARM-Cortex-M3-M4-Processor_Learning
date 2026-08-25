#include<stdint.h>
#include<stdio.h>
#include"main.h"
#include "led.h"


//Prototypes of the Functions
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void init_systick_timer(uint32_t tick_hz);
__attribute ((naked)) void init_scheduler_stack(uint32_t scheduler_top_stack);

__attribute ((naked)) void SysTick_Handler(void);

__attribute ((naked)) void switch_sp_psp(void);

void delay(uint32_t count);

uint32_t get_psp(void);

void init_task_stack(void);

void enable_fault(void);

uint32_t current_task = 1; // Task one is running
uint32_t global_tick_count =0;

//Task Delay, Here we are going to block the task for the given number of tasks

void task_delay (uint32_t tick_count);

typedef struct {
	uint32_t psp_value;
	uint32_t block_count;
	uint32_t current_state;
	void (*task_handler)(void);
}TCP_t; //Task control block

TCP_t user_tasks [MAX_TASK];

int main(void)
{
	enable_fault ();
	init_scheduler_stack(Sch_Stack_Start);

	led_init_all();


	init_task_stack();//Dummy values to sf1 sf 2

	init_systick_timer(TICK_HZ);

	switch_sp_psp();

	task1_handler();
    /* Loop forever */
	for(;;);
}

void ideal_task (void){
	while(1);
}
void task1_handler(void){
	while(1){
		led_on(LED_GREEN);

		task_delay(1000);

		led_off(LED_GREEN);

		task_delay(1000);
	}
}

void task2_handler(void){
	while(1){
        led_on(LED_ORANGE);

		task_delay(500);
        led_off(LED_ORANGE);

		task_delay(500);
	}
}

void task3_handler(void){
	while(1){
        led_on(LED_BLUE);

		task_delay(250);

        led_off(LED_BLUE);

		task_delay(250);
	}
}

void task4_handler(void){
	while(1){
        led_on(LED_RED);

		task_delay(125);

        led_off(LED_RED);

		task_delay(125);
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

	user_tasks [0]. current_state = TASK_READY_STATE;
	user_tasks [1]. current_state = TASK_READY_STATE;
	user_tasks [2]. current_state = TASK_READY_STATE;
	user_tasks [3]. current_state = TASK_READY_STATE;
	user_tasks [4]. current_state = TASK_READY_STATE;

	user_tasks [0]. psp_value = IDEAL_Stack_Start;
	user_tasks [1]. psp_value = T1_Stack_Start;
	user_tasks [2]. psp_value = T2_Stack_Start;
	user_tasks [3]. psp_value = T3_Stack_Start;
	user_tasks [4]. psp_value = T4_Stack_Start;

	user_tasks[0]. task_handler = ideal_task;
	user_tasks[1]. task_handler = task1_handler;
	user_tasks[2]. task_handler = task2_handler;
	user_tasks[3]. task_handler = task3_handler;
	user_tasks[4]. task_handler = task4_handler;


	uint32_t *pPSP;
	for (int i =0; i< MAX_TASK; i++){
		pPSP= (uint32_t*) user_tasks[i].psp_value;
		pPSP--;
		*pPSP = DUMMY_XPSR; //0x01000000

		pPSP--; //PC
		*pPSP = (uint32_t)user_tasks[i].task_handler;

		pPSP--;
		*pPSP = 0xFFFFFFFD;

		for (int j=0;j<13; j++){
			pPSP --;
			*pPSP = 0;
		}

	user_tasks[i].psp_value = (uint32_t) pPSP;

	}
}

//to get the psp value of current stack
uint32_t get_psp(void){

	return user_tasks[current_task].psp_value;
}

void save_psp_value (uint32_t current_psp_value){
	user_tasks[current_task].psp_value=current_psp_value;
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



void update_next_task (void){
	int state =TASK_BLOCKED_STATE;
	for (int i=0;i<(MAX_TASK) ; i++){
		current_task ++;
		current_task %= MAX_TASK;
		state = user_tasks[current_task].current_state;
		if((state==TASK_READY_STATE)&& (current_task != 0))
			break;

	}

	if (state != TASK_READY_STATE)
		current_task =0;
}

__attribute ((naked)) void PendSV_Handler(void){
	//Save the context of the current value

		//Get the Running task's psp value
		__asm volatile ("MRS R0, PSP");

		// Store the sf2 usng psp value
		__asm volatile ("STMDB R0!,  {R4 -R11}"); // Store Multiple and decrement before

//Save the value of lr
		__asm volatile ("PUSH {LR}");

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

		__asm volatile ("POP {LR}");

		//EXIT
		__asm volatile ("BX LR");

}
void update_global_tick_count(void){
	global_tick_count++;
}

void unblock_tasks(void){
	for (int i=1;i<MAX_TASK;i++)
	{
		if (user_tasks [i].current_state != TASK_READY_STATE)
		{
			if (user_tasks [i].block_count == global_tick_count)
			{
				user_tasks[i].current_state = TASK_READY_STATE ;
			}
		}
	}
}

__attribute ((naked)) void SysTick_Handler(void){
	uint32_t *pICSR =(uint32_t*) 0xE000ED04;
	update_global_tick_count();
	unblock_tasks();

	*pICSR |= (1<<28);
}

void enable_fault(void){
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24; //this is the part of control register where it is system handler and control register

		//Enable all the configurable faults.
		*pSHCSR |= (1<<16); // Mem Fault
		*pSHCSR |= (1<<17); //Bus Fault
		*pSHCSR |= (1<<18); //Usages Fault

}

void schedule(void){
	uint32_t *pICSR =(uint32_t*) 0xE000ED04;
	*pICSR |= (1<<28);
}

void task_delay (uint32_t tick_count){
	if (current_task)
	{
		user_tasks[current_task].block_count = global_tick_count + tick_count;
		user_tasks[current_task].current_state = TASK_BLOCKED_STATE;
		schedule();

	}

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



