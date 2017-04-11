#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#include <sys/neutrino.h>
#include <stdint.h>
#include <sys/mman.h>
#include <hw/inout.h>

#define QNX_BASE_ADDRESS (0x280)
#define CLOCK_PERIOD_10US (10000)
#define TIMER_PERIOD_NS (490000)
#define PWM_PERIOD (20000000)

uintptr_t port_a;
uintptr_t dir;

//Parameters of the timer
struct timespec timer_spec;
struct timespec t1;
timer_t timerID;

void *toggle() {
    static unsigned int val = 0;
    static unsigned int duty_cycle = 460000;

    timer_spec.tv_nsec = duty_cycle;

    while (1) {

    	//printf("In toggle\n");

    //if (val == 0) {
    //    // Define the oneshot of the timer
    //	timer_spec.tv_sec = 0;
    //	timer_spec.tv_nsec = duty_cycle;
    //} else {
    //    // Define the oneshot of the timer
    //	timer_spec.tv_sec = 0;
    //	timer_spec.tv_nsec = PWM_PERIOD - duty_cycle;
    //}

    // Start the timer
    //timer_settime(timerID, TIMER_ABSTIME, &timer_spec, NULL);

    // debug code used to verify accuract of timer
    	val ^= 1;
    	out8(port_a,val);

    	nanosleep(&timer_spec, NULL);
    }
}

void *bankDoor() {

	t1.tv_nsec = 100000000;

	unsigned int count = 0;

    while (1) {
        // While the bank is not closed
        //printf("Hello world\n");
    	nanosleep(&t1, NULL);
    	count += 1;
}}

int main(int argc, char *argv[]) {
	printf("Welcome to the QNX Momentics IDE\n");

    if ( ThreadCtl(_NTO_TCTL_IO, NULL) == -1)
    {

    	printf("Failed to get I/O access permission");
    }

    port_a = mmap_device_io(1, QNX_BASE_ADDRESS + 8);
    dir = mmap_device_io(1, QNX_BASE_ADDRESS + 11);
    // Set PortA as output
    out8(dir,0x00);

    // What to do when the timer expires
    struct sigevent event;
    struct sigaction action;

    	struct _clockperiod clkper;

    	// Set clock to a period of 10 us
    	clkper.nsec       = CLOCK_PERIOD_10US;
    	clkper.fract      = 0;
    	ClockPeriod ( CLOCK_REALTIME, &clkper, NULL, 0  );

        // Verify clock period has been set to 10 us
        struct timespec res;
        clock_getres( CLOCK_REALTIME, &res);
    	printf( "Resolution is %ld micro seconds.\n", res.tv_nsec / 1000 );

        // Define the oneshot of the timer
    	//timer_spec.it_value.tv_sec = 0;
    	//timer_spec.it_value.tv_nsec = TIMER_PERIOD_NS;

    	//Define the period of timer
    	//timer_spec.it_interval.tv_sec = 0;
    	//timer_spec.it_interval.tv_nsec = 0;

        // Setup signal handler which occurrs when timer fires
    	//action.sa_sigaction = toggle;
    	//action.sa_flags = SA_SIGINFO;
    	//sigaction( SIGUSR1, &action, NULL );
    	//SIGEV_SIGNAL_INIT(&event, SIGUSR1);

        // Start the timer
       // timer_create(CLOCK_REALTIME, &event, &timerID);
       // timer_settime(timerID, 0, &timer_spec, NULL);

       // unsigned int count = 0;

        pthread_t bankdoor_th;
       // pthread_t th;
        pthread_t br_th;
        pthread_create(&bankdoor_th, NULL, toggle, NULL);
       // pthread_create(&th, NULL, bankDoor, NULL);

        while(1) {
        	//usleep(500000);
        	//printf( "Test print %u.\n", count++);
        }

        printf( "EXITING PROGRAM\n");
	return EXIT_SUCCESS;
}
