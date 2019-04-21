/*
 * tasks.h
 *
 *  Created on: 20.04.2019
 *      Author: Akeman
 */

#ifndef TASKS_TASKS_H_
#define TASKS_TASKS_H_

#include <ti/sysbios/knl/Task.h>

#define TASKSTACKSIZE 512
#define LCD_TASK_STACKSIZE 2048
#define TEMP_TASK_STACKSIZE 2048
#define DATA_TASK_STACKSIZE 2048
#define REFLOW_TASK_STACKSIZE 2048

#define LCD_TASK_PRIO 1
#define TEMP_TASK_PRIO 1
#define DATA_TASK_PRIO 1
#define REFLOW_TASK_PRIO 1

#define TCPPACKETSIZE 256
#define NUMTCPWORKERS 3

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

Task_Struct lcdTaskStruct;
Char lcdTaskStack[LCD_TASK_STACKSIZE];

Task_Struct tempTaskStruct;
Char tempTaskStack[TEMP_TASK_STACKSIZE];

Task_Struct dataTaskStruct;
Char dataTaskStack[DATA_TASK_STACKSIZE];

Task_Struct reflowTaskStruct;
Char reflowTaskStack[REFLOW_TASK_STACKSIZE];

void initTasks(void);

#endif /* TASKS_TASKS_H_ */
