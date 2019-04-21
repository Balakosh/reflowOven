/*
 * tasks.c
 *
 *  Created on: 20.04.2019
 *      Author: Akeman
 */

#include "tasks.h"
#include "heartbeat.h"
#include "lcd/lcd.h"
#include "temp/temp.h"
#include "data/data.h"
#include "reflow/reflow.h"

void initTasks(void)
{
    Task_Params taskParams;

    /* Construct heartBeat Task thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)heartBeatFxn, &taskParams, NULL);

    /* Construct lcd Task thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = NULL;
    taskParams.stackSize = LCD_TASK_STACKSIZE;
    taskParams.stack = &lcdTaskStack;
    taskParams.priority = LCD_TASK_PRIO;
    Task_construct(&lcdTaskStruct, (Task_FuncPtr)lcdTaskRun, &taskParams, NULL);

    /* Construct temp Task thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = NULL;
    taskParams.stackSize = TEMP_TASK_STACKSIZE;
    taskParams.stack = &tempTaskStack;
    taskParams.priority = TEMP_TASK_PRIO;
    Task_construct(&tempTaskStruct, (Task_FuncPtr)tempTaskFxn, &taskParams, NULL);

    /* Construct data Task thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = NULL;
    taskParams.stackSize = DATA_TASK_STACKSIZE;
    taskParams.stack = &dataTaskStack;
    taskParams.priority = DATA_TASK_PRIO;
    Task_construct(&dataTaskStruct, (Task_FuncPtr)dataTaskFxn, &taskParams, NULL);

    /* Construct reflow Task thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = NULL;
    taskParams.stackSize = REFLOW_TASK_STACKSIZE;
    taskParams.stack = &reflowTaskStack;
    taskParams.priority = REFLOW_TASK_PRIO;
    Task_construct(&reflowTaskStruct, (Task_FuncPtr)reflowTaskFxn, &taskParams, NULL);
}


