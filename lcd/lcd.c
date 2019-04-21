/*
 * lcd.c
 *
 *  Created on: 20.04.2019
 *      Author: Akeman
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Mailbox.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

#include "Board.h"
#include "lcd.h"

bool i2cWrite(uint8_t* data, uint8_t len)
{
    bool success = false;
    I2C_Handle      i2c;
    I2C_Params      i2cParams;
    I2C_Transaction i2cTransaction;

    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
    i2c = I2C_open(Board_I2C0, &i2cParams);

    if (i2c == NULL)
    {
        System_abort("Error Initializing I2C\n");
    }
    else
    {
        System_printf("I2C Initialized!\n");
    }

    i2cTransaction.slaveAddress = 0x27;
    i2cTransaction.writeBuf = data;
    i2cTransaction.writeCount = len;
    i2cTransaction.readBuf = NULL;
    i2cTransaction.readCount = 0;

    if (I2C_transfer(i2c, &i2cTransaction))
    {
        System_printf("I2C OK\n");
        success = true;
    }
    else
    {
        System_printf("I2C Bus fault\n");
        success = false;
    }

    I2C_close(i2c);
    System_printf("I2C closed!\n");

    System_flush();

    return success;
}

void writeLcd(uint8_t* data, uint8_t len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        i2cWrite(&data[i], 1);
        Task_sleep(1);
        data[i] &= ~0x04;
        i2cWrite(&data[i], 1);
    }
}

void writeChar(char data)
{
    uint8_t txBuffer[2];

    txBuffer[0] = (data & 0xf0) | 0x0d;
    txBuffer[1] = ((data & 0x0f) << 4) | 0x0d;
    writeLcd(&txBuffer[0], 2);
    Task_sleep(1);
}

void writeString(char* data)
{
    int i;
    int length = strlen(data);

    for (i = 0; i < length; i++)
        writeChar(data[i]);
}

void writeLine(char* data)
{
    int i;

    for (i = 0; i < 20; i++)
    {
        if (data[i] == 0)
            writeChar(' ');
        else
            writeChar(data[i]);
    }
}

void initLcd(void)
{
    int i;
    uint8_t txBuffer[10];

    // INIT 3 times
    for( i = 0; i < 3; i++)
    {
        txBuffer[0] = 0x3c;
        writeLcd(&txBuffer[0], 1);
        Task_sleep(4);
    }

    // SET 4 BIT MODE
    txBuffer[0] = 0x2c;
    writeLcd(&txBuffer[0], 1);
    Task_sleep(1);

    // 4 Rows 5x8 dots
    txBuffer[0] = 0x2c;
    txBuffer[1] = 0x8c;
    writeLcd(&txBuffer[0], 2);
    Task_sleep(1);

    // Display off
    txBuffer[0] = 0x04;
    txBuffer[1] = 0x8c;
    writeLcd(&txBuffer[0], 2);
    Task_sleep(1);

    // Display clear
    txBuffer[0] = 0x04;
    txBuffer[1] = 0x14;
    writeLcd(&txBuffer[0], 2);
    Task_sleep(1);

    // Cursor right no shift
    txBuffer[0] = 0x04;
    txBuffer[1] = 0x64;
    writeLcd(&txBuffer[0], 2);
    Task_sleep(1);

    // Display on
    txBuffer[0] = 0x04;
    txBuffer[1] = 0xcc;
    writeLcd(&txBuffer[0], 2);
    Task_sleep(1);
}

void lcdTaskRun(void)
{
    lcdMailboxStruct msg;

    Task_sleep(15); // Wait for reset end of HD44780

    initLcd();

    //writeString("New Akeman ist cool!\n Hello World! Bachkippe!12345abcdefghijklmnopqr");

    while (1)
    {
        Mailbox_pend(lcdMailbox, &msg, BIOS_WAIT_FOREVER);

        writeLine(&msg.lcdData[0][0]);
        writeLine(&msg.lcdData[2][0]);
        writeLine(&msg.lcdData[1][0]);
        writeLine(&msg.lcdData[3][0]);
    }
}
