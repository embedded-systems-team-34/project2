/******************************************************************************
* FILENAME : PWM.c
*
* DESCRIPTION :
*     PWM setup and interface code.  Setup PWM for a period of 20 ms on using PortA
*     pin 0 and 1
*
* AUTHOR:
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu
*
******************************************************************************/

#include "pwm.h"

unsigned int channel_mask[2] = {MASK_CHANNEL_0, MASK_CHANNEL_1};
unsigned int duty_cycle[2] = {0 , 0};


// PWM thread to drive the two servo motors
void pwm_channel() {
    struct timespec timer_spec;
    unsigned int max;

    while (1) {

    	// Set both PWM channels high
    	out8(port_a,channel_mask[1] | channel_mask[0]);


    	// nanospin used in order to get an accurate duty cycle to reduce motor jitter

    	// Get minimum PWM On time
    	if (duty_cycle[0] < duty_cycle[1]) {
            timer_spec.tv_sec = 0;
            // Get the duty cycle ON time
            timer_spec.tv_nsec = duty_cycle[0];
            nanospin(&timer_spec);
            out8(port_a, channel_mask[1] | !(channel_mask[0]));
            timer_spec.tv_sec = 0;
            // Calculate the delta left for the high channel
            timer_spec.tv_nsec = duty_cycle[1] - duty_cycle[0];
            nanospin(&timer_spec);
            out8(port_a, !(channel_mask[1]) | !(channel_mask[0]));
            max = duty_cycle[1];
    	} else {
            timer_spec.tv_sec = 0;
            timer_spec.tv_nsec = duty_cycle[1];
            nanospin(&timer_spec);
            out8(port_a, !(channel_mask[1]) | (channel_mask[0]));
            timer_spec.tv_sec = 0;
            // Calculate the delta left for the high channel
            timer_spec.tv_nsec = duty_cycle[0] - duty_cycle[1];
            nanospin(&timer_spec);
            out8(port_a, !(channel_mask[1]) | !(channel_mask[0]));
            max = duty_cycle[0];
    	}
    	// Set both outputs low for the remainder of the PWM period
        timer_spec.tv_sec = 0;
        timer_spec.tv_nsec = DUTY_CYCLE_PERIOD - max;

        // Blocking call for the remainder of PWM period
        nanosleep(&timer_spec, NULL);

    }

}

// Initalize the PWM hardware - get hardware permission, and create pwm thread
void pwmInit() {

	pthread_t pwm_th;

	// Get access to hardware IO
    if ( ThreadCtl(_NTO_TCTL_IO, NULL) == -1)
    {

    	printf("Failed to get I/O access permission");
    }

    // PORTA is located at QNX_BASE_ADDRESS + 8 (register 0x288)
    port_a = mmap_device_io(1, QNX_BASE_ADDRESS + 8);
    dir = mmap_device_io(1, QNX_BASE_ADDRESS + 11);
    // Set PortA as output
    out8(dir,0x00);

    // Increase thread priority so PWM signal will always be stable and can
    // run over other threads
    pthread_setschedprio( pwm_th, 12 );
    pthread_create(&pwm_th, NULL, pwm_channel, NULL);

}

// Set the PWM duty cycle for a given channel
void setPWMDuty( unsigned int channel_num, unsigned int set_duty_cycle) {
    // Write new Duty cycle value for channel 1
    if (channel_num == 0) {
        duty_cycle[0] = set_duty_cycle;
    }
    // Write new duty cycle value for channel 2
    else {
        duty_cycle[1] = set_duty_cycle;
    }
}
