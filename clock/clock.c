/*
 * clock.c
 *
 *  Created on: 21.04.2019
 *      Author: Akeman
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

#include "clock.h"

void clockIsr(void)
{
    usTicks++;
}

void initTimer(void)
{
    Error_Block eb;
    Timer_Params timerParams;

    usTicks = 0;

    Error_init(&eb);
    Timer_Params_init(&timerParams);

    timerParams.period = 1;
    timerParams.runMode = Timer_RunMode_CONTINUOUS;
    timerParams.startMode = Timer_StartMode_AUTO;

    usTimerHandle = Timer_create(Timer_ANY, (Timer_FuncPtr)&clockIsr, &timerParams, &eb);
}




