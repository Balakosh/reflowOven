/*
 * heartbeat.c
 *
 *  Created on: 20.04.2019
 *      Author: Akeman
 */

#include <stdint.h>

#include <ti/drivers/GPIO.h>
#include <ti/sysbios/knl/Task.h>

#include "Board.h"

void heartBeatFxn(UArg arg0, UArg arg1)
{
    while (1) {
        Task_sleep((unsigned int)arg0);
        GPIO_toggle(Board_LED0);
    }
}

