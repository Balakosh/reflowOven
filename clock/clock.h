/*
 * clock.h
 *
 *  Created on: 21.04.2019
 *      Author: Akeman
 */

#ifndef CLOCK_CLOCK_H_
#define CLOCK_CLOCK_H_

#include <ti/sysbios/hal/Timer.h>

Timer_Handle usTimerHandle;
unsigned long long int usTicks;

void initTimer(void);

#endif /* CLOCK_CLOCK_H_ */
