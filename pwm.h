/******************************************************************************
* FILENAME : PWM.h
*
* DESCRIPTION :
*     Function prototypes for PWM
*
* AUTHOR:
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu
*
******************************************************************************/
#ifndef PWM_H
#define PWM_H

#include <time.h>
#include <pthread.h>

#include <unistd.h>

#include <sys/neutrino.h>
#include <stdint.h>
#include <sys/mman.h>
#include <hw/inout.h>

// 20 ms period in ns
#define DUTY_CYCLE_PERIOD (20000000)
#define MASK_CHANNEL_0 (0x1)
#define MASK_CHANNEL_1 (0x2)

#define QNX_BASE_ADDRESS (0x280)

uintptr_t port_a;
uintptr_t dir;

// Function Prototypes
void pwmInit(void);
void setPWMDuty( unsigned int channel_num, unsigned int duty_cycle);
void pwm_channel();

#endif
