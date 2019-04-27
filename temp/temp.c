/*
 * temp.c
 *
 *  Created on: 20.04.2019
 *      Author: Akeman
 */
#include <string.h>
#include <stdio.h>

#include <ti/drivers/SPI.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include "Board.h"
#include "lcd/lcd.h"
#include "temp.h"
#include "relay/relay.h"

#define SPI_MSG_LENGTH 4

unsigned char masterRxBuffer[SPI_MSG_LENGTH];
unsigned char masterTxBuffer[SPI_MSG_LENGTH];

void tempTaskFxn(void)
{
    SPI_Handle masterSpi;
    SPI_Transaction masterTransaction;
    bool transferOK;
    SPI_Params params;

    temperature = 0;

    SPI_Params_init(&params);

    params.frameFormat = SPI_POL0_PHA1; //SPI_POL0_PHA1;

    params.bitRate = 1000000;
    params.dataSize = 8;
    params.mode = SPI_MASTER;
    params.transferMode = SPI_MODE_BLOCKING;
    params.transferTimeout = SPI_WAIT_FOREVER;
    params.transferCallbackFxn = NULL;

    masterSpi = SPI_open(Board_SPI0, &params);

    if (masterSpi == NULL)
    {
//        System_abort("Error initializing SPI\n");
    }
    else
    {
//        System_printf("SPI initialized\n");
    }

    /* Initialize master SPI transaction structure */
    masterTransaction.count = SPI_MSG_LENGTH;
    masterTransaction.txBuf = (Ptr)masterTxBuffer;
    masterTransaction.rxBuf = (Ptr)masterRxBuffer;

    memset((Ptr)masterRxBuffer, 0, SPI_MSG_LENGTH);

    while (1)
    {
        transferOK = SPI_transfer(masterSpi, &masterTransaction);

        uint16_t temp = ((masterRxBuffer[0] << 8 | masterRxBuffer[1]) >> 3);

        temperature = 0.25 * temp;

        if(transferOK)
        {
//            char buffer[100];
//
//            snprintf(buffer, sizeof(buffer), "TempF=%f 0=%x 1=%x\n",
//                     temperature, masterRxBuffer[0], masterRxBuffer[1]);
//            System_printf(buffer);

            Semaphore_post(temperatureUpdateSemaphore);
        }
        else
        {
//            System_printf("Unsuccessful master SPI transfer");
        }

//        System_flush();

        if (temperature > 28)
            relayOn();
        else
            relayOff();

        Task_sleep(1000);
    }
//    SPI_close(masterSpi);
//
//    System_printf("Done\n");
//
//    System_flush();
}
