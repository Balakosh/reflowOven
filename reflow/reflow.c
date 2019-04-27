/*
 * reflow.c
 *
 *  Created on: 21.04.2019
 *      Author: Akeman
 */
#include <stdio.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/knl/Task.h>

#include "reflow.h"
#include "relay/relay.h"
#include "temp/temp.h"

#define RAMP_TO_SOAK_TIME 120
#define SOAK_TEMP 150
#define REFLOW_TIME 60
#define PEAK_TEMP 217

extern ReflowStates reflowState;
ReflowStates oldReflowState;
int subState;
Timer_Handle reflowTimerHandle;

void reflowTimer(void)
{
    Semaphore_post(reflowSemaphore);
}

void initReflowTimer(int period)
{
    Error_Block eb;
    Timer_Params timerParams;

    Error_init(&eb);
    Timer_Params_init(&timerParams);

    timerParams.period = period;
    timerParams.runMode = Timer_RunMode_ONESHOT;
    timerParams.startMode = Timer_StartMode_USER;

    reflowTimerHandle = Timer_create(Timer_ANY, (Timer_FuncPtr)&reflowTimer, &timerParams, &eb);
}

void reflowTaskFxn(void)
{
    reflowState = Waiting;
    subState = 0;

    while (1)
    {
        oldReflowState = reflowState;

        switch (reflowState)
        {
            case Waiting:
            {
                Semaphore_pend(reflowStartSemaphore, BIOS_WAIT_FOREVER);

                reflowState = Ramp_to_soak;
                break;
            }
            case Ramp_to_soak:
            {
                switch (subState)
                {
                    case 0:
                    {
                        relayOn();
                        subState++;
                        break;
                    }
                    case 1:
                    {
                        Semaphore_pend(temperatureUpdateSemaphore, BIOS_WAIT_FOREVER);

                        if (temperature > 150)
                            reflowState = Ramp_to_soak;
                        break;
                    }
                }
                break;
            }
            case Soak:
            {
                switch (subState)
                {
                    case 0:
                    {
                        initReflowTimer(RAMP_TO_SOAK_TIME * 1000);
                        Timer_start(reflowTimerHandle);
                        subState++;
                        break;
                    }
                    case 1:
                    {
                        Semaphore_pend(temperatureUpdateSemaphore, BIOS_WAIT_FOREVER);

                        if (temperature > (SOAK_TEMP * 1.05))
                            relayOff();

                        if (temperature < SOAK_TEMP)
                            relayOn();

                        if (Semaphore_pend(reflowSemaphore, BIOS_NO_WAIT))
                            reflowState = Ramp_to_peak;
                        break;
                    }
                }

                break;
            }
            case Ramp_to_peak:
            {
                switch (subState)
                {
                    case 0:
                    {
                        relayOn();
                        subState++;
                        break;
                    }
                    case 1:
                    {
                        Semaphore_pend(temperatureUpdateSemaphore, BIOS_WAIT_FOREVER);

                        if (temperature >= PEAK_TEMP)
                            reflowState = Reflow;
                        break;
                    }
                }

                break;
            }
            case Reflow:
            {
                switch (subState)
                {
                    case 0:
                    {
                        initReflowTimer(REFLOW_TIME * 1000);
                        Timer_start(reflowTimerHandle);
                        subState++;
                        break;
                    }
                    case 1:
                    {
                        Semaphore_pend(temperatureUpdateSemaphore, BIOS_WAIT_FOREVER);

                        if (temperature > (PEAK_TEMP * 1.05))
                            relayOff();

                        if (temperature < PEAK_TEMP)
                            relayOn();

                        if (Semaphore_pend(reflowSemaphore, BIOS_NO_WAIT))
                            reflowState = Cooling;

                        break;
                    }
                }
                break;
            }
            case Cooling:
            {
                switch (subState)
                {
                    case 0:
                    {
                        relayOff();
                        subState++;
                        break;
                    }
                    case 1:
                    {
                        Semaphore_pend(temperatureUpdateSemaphore, BIOS_WAIT_FOREVER);

                        if (temperature < 50)
                            reflowState = Finished;
                        break;
                    }
                }
                break;
            }
            case Finished:
            {
                Task_sleep(1000);
                break;
            }
        }

        if (oldReflowState != reflowState)
        {
            char buffer[64];
            subState = 0;
            snprintf(buffer, sizeof(buffer), "State=%d\n", reflowState);
            System_printf(buffer);
        }
    }
}
