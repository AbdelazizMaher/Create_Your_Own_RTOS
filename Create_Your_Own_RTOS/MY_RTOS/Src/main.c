/*
 * Session07_SVC_PendSV Exceptions.c
 *
 *  Created on: Mar 24, 2023
 *      Author: Abdelaziz Maher
 */

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


#include"Stm32f103x6.h"
#include "OS_Scheduler.h"

sOS_Task_ControlBlock TCB_Task1 , TCB_Task2 , TCB_Task3 , TCB_Task4;
unsigned char Task1_Led , Task2_Led , Task3_Led , Task4_Led;
sOS_Mutex_ControlBlock Mutex1;
unsigned char payload1[3] = {1,2,3};

sOS_Mutex_ControlBlock Mutex2;
unsigned char payload2[3] = {1,2,3};

void Task1()
{
	static int count = 0;
	while(1)
	{
		Task1_Led ^= 1;
		//Task1 Code
		count++;
		if( count == 500 )
		{
			MYRTOS_AcquireMutex(&Mutex1 ,&TCB_Task1);
			MYRTOS_ActivateTask(&TCB_Task4);
			MYRTOS_AcquireMutex(&Mutex2 ,&TCB_Task1);
		}
		if( count == 800 )
		{
			MYRTOS_ReleaseMutex(&Mutex1);
			MYRTOS_ReleaseMutex(&Mutex2);
			count = 0;
		}
	}
}

void Task2()
{
	static int count = 0;
	while(1)
	{
		Task2_Led ^= 1;
		//Task2 Code
		count++;
		if( count == 100 )
		{
			MYRTOS_ActivateTask(&TCB_Task3);
		}
		if( count == 200 )
		{
			MYRTOS_TerminateTask(&TCB_Task2);
			count = 0;
		}
	}
}

void Task3()
{
	static int count = 0;
	while(1)
	{
		Task3_Led ^= 1;
		//Task3 Code
		count++;
		if( count == 100 )
		{
			MYRTOS_ActivateTask(&TCB_Task4);
		}
		if( count == 200 )
		{
			MYRTOS_TerminateTask(&TCB_Task3);
			count = 0;
		}
	}
}

void Task4()
{
	static int count = 0;
	while(1)
	{
		Task4_Led ^= 1;
		//Task4 Code
		count++;
		if( count == 100 )
		{
			MYRTOS_AcquireMutex(&Mutex2 ,&TCB_Task4);
			MYRTOS_AcquireMutex(&Mutex1 ,&TCB_Task4);
		}
		if( count == 200 )
		{
			MYRTOS_ReleaseMutex(&Mutex2);
			MYRTOS_ReleaseMutex(&Mutex1);
			MYRTOS_TerminateTask(&TCB_Task4);
			count = 0;
		}
	}
}

int main(void)
{
	eOS_ErrorState err = NOERROR;

	// HW_Init (Initialize clock tree , Reset controller)
	HW_Init();

	// Init OS
	if( MYRTOS_Init() != NOERROR )
		while(1);

	//Configure Mutex1
	strcpy( Mutex1.MutexName , "MUTEX1" );
	Mutex1.PayloadBuffer_Size = 3;
	Mutex1.pPayloadBuffer = payload1;
	//Configure Mutex2
	strcpy( Mutex2.MutexName , "MUTEX2" );
	Mutex2.PayloadBuffer_Size = 3;
	Mutex2.pPayloadBuffer = payload2;

	//Configure Task1
	strcpy( TCB_Task1.TaskName , "Task1" );
	TCB_Task1.Priority = 4;
	TCB_Task1.pTaskEntry = Task1;
	TCB_Task1.Stack_Size = 500;

	//Configure Task2
	strcpy( TCB_Task2.TaskName , "Task2" );
	TCB_Task2.Priority = 3;
	TCB_Task2.pTaskEntry = Task2;
	TCB_Task2.Stack_Size = 500;

	//Configure Task3
	strcpy( TCB_Task3.TaskName , "Task3" );
	TCB_Task3.Priority = 2;
	TCB_Task3.pTaskEntry = Task3;
	TCB_Task3.Stack_Size = 500;

	//Configure Task4
	strcpy( TCB_Task4.TaskName , "Task4" );
	TCB_Task4.Priority = 1;
	TCB_Task4.pTaskEntry = Task4;
	TCB_Task4.Stack_Size = 500;

	err += MYRTOS_CreateTask(&TCB_Task1);
	err += MYRTOS_CreateTask(&TCB_Task2);
	err += MYRTOS_CreateTask(&TCB_Task3);
	err += MYRTOS_CreateTask(&TCB_Task4);

	err += MYRTOS_ActivateTask(&TCB_Task1);



	MYRTOS_StartOS();

	while(1)
	{

	}
	return 0;
}


