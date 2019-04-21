/*
 * data.c
 *
 *  Created on: 21.04.2019
 *      Author: Akeman
 */
#include <string.h>
#include <stdio.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

#include "lcd/lcd.h"
#include "temp/temp.h"

void dataTaskFxn(void)
{
    lcdMailboxStruct msg;

    while (1)
    {
        Task_sleep(1000);

        memset(&msg, ' ', sizeof(lcdMailboxStruct));

        snprintf(msg.lcdData[0], sizeof(msg.lcdData[0]), "Ramp to soak 1/5");
        snprintf(msg.lcdData[1], sizeof(msg.lcdData[1]), "%5.2fC / 150C", temperature);

        Mailbox_post(lcdMailbox, &msg, BIOS_WAIT_FOREVER);
    }
}


