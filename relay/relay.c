/*
 * relay.c
 *
 *  Created on: 21.04.2019
 *      Author: Akeman
 */

#include <ti/drivers/GPIO.h>

#include "board.h"

void relayOn(void)
{
    GPIO_write(Board_Relay, 0);
}

void relayOff(void)
{
    GPIO_write(Board_Relay, 1);
}


