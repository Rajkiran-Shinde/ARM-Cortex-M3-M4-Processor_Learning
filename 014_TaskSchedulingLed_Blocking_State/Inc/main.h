/*
 * main.h
 *
 *  Created on: Aug 14, 2026
 *      Author: Raj
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAX_TASK 5
//Stack Memory Calculation
#define Size_Stack 1024U
#define Size_Scheduler 1024U

#define SRAM_Start 0x20000000U
#define Size_SRAM ((128)*(1024))
#define SRAM_END ((SRAM_Start) + (Size_SRAM))

#define T1_Stack_Start 		SRAM_END
#define T2_Stack_Start 		((SRAM_END)- (Size_Stack))
#define T3_Stack_Start 		((SRAM_END)- (2*Size_Stack))
#define T4_Stack_Start 		((SRAM_END)- (3*Size_Stack))
#define IDEAL_Stack_Start 	((SRAM_END)- (4*Size_Stack))
#define Sch_Stack_Start 	((SRAM_END)- (5*Size_Stack))

//Systick Timer config
#define TICK_HZ 1000U
#define SYSTICK_TM 180000000U //PLL CLOCK

#define DUMMY_XPSR 0x01000000U

#define TASK_READY_STATE 0x00
#define TASK_BLOCKED_STATE 0xFF

#endif /* MAIN_H_ */
