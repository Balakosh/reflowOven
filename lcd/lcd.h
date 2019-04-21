/*
 * lcd.h
 *
 *  Created on: 20.04.2019
 *      Author: Akeman
 */

#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#include <ti/sysbios/knl/Mailbox.h>

typedef struct MsgObj {
    Int id;
    char lcdData[4][20];
} lcdMailboxStruct, *Msg;

Mailbox_Handle lcdMailbox;

void lcdTaskRun(void);

#endif /* LCD_LCD_H_ */
