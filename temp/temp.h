/*
 * temp.h
 *
 *  Created on: 20.04.2019
 *      Author: Akeman
 */

#ifndef TEMP_TEMP_H_
#define TEMP_TEMP_H_

#include <ti/sysbios/knl/Semaphore.h>

float temperature;
Semaphore_Handle temperatureUpdateSemaphore;

void tempTaskFxn(void);

#endif /* TEMP_TEMP_H_ */
