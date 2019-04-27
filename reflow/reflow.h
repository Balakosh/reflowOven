/*
 * reflow.h
 *
 *  Created on: 21.04.2019
 *      Author: Akeman
 */

#ifndef REFLOW_REFLOW_H_
#define REFLOW_REFLOW_H_

#include <ti/sysbios/knl/Semaphore.h>

typedef enum ReflowStates {
    Waiting = 0,
    Ramp_to_soak,
    Soak,
    Ramp_to_peak,
    Reflow,
    Cooling,
    Finished,

    ReflowStateCount
} ReflowStates;

ReflowStates reflowState;
Semaphore_Handle reflowStartSemaphore;
Semaphore_Handle reflowSemaphore;

void reflowTaskFxn(void);

#endif /* REFLOW_REFLOW_H_ */
