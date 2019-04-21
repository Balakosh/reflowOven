#include <string.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

/* TI-RTOS Header files */
// #include <ti/drivers/EMAC.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/USBMSCHFatFs.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

#include "Board.h"
#include "tasks/tasks.h"
#include "lcd/lcd.h"

void initMailbox(void)
{
    Error_Block eb;
    Mailbox_Params params;

    Error_init(&eb);
    Mailbox_Params_init(&params);

    lcdMailbox = Mailbox_create(sizeof(lcdMailboxStruct), 10, &params, &eb);
}

int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initEMAC();
    Board_initGPIO();
    Board_initI2C();
    // Board_initSDSPI();
    Board_initSPI();
    // Board_initUART();
    // Board_initUSB(Board_USBDEVICE);
    // Board_initUSBMSCHFatFs();
    // Board_initWatchdog();
    // Board_initWiFi();

    initMailbox();
    initTasks();

     /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    BIOS_start();

    return (0);
}
