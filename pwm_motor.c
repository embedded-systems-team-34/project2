#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#include "pwm.h"
#include "motor.h"
#include "fsm.h"

uint8_t buffer[BufferSize];
struct fsm motor0_sm;
struct fsm motor1_sm;

// Thread active every 100 ms to update the state of the state machine
void systemTick(void) {

    struct timespec timer_spec;
    
    timer_spec.tv_sec = 0;
    timer_spec.tv_nsec = SYSTEM_TICK_PERIOD;

    while (1) {
        // Update the state of the two motors
        process_SM(&motor0_sm);
        process_SM(&motor1_sm);
        // Block for 100 ms
        nanosleep(&timer_spec, NULL);   
    }
}

void parseUserInput(char *string,unsigned int length) {
    
    unsigned int double_help = 0;
    
    // Invalid command length so do nothing
    if (length != 2) {
        return;
    }
    // Length must be 2 so this is a valid command to parse
    
    // convert entered commands to uppercase 
    string[0] = toupper(string[0]); 
    string[1] = toupper(string[1]);
    
    // If double help is requested by user only print out a single help message instead of reapeating same message twice
    if ((string[0] == 'H') && (string[1] == 'H')) {
        double_help = 1;
    } else {
        double_help = 0;
    }
    
    // Only process the command if no 'x' are detected
    if ((string[0] != 'X') && (string[1] != 'X')) {
        parseSerialCommand(&motor0_sm, string[0]);
        // If Double help prevent displaying help message twice, else process state machine 1
        if (double_help == 0) {
            parseSerialCommand(&motor1_sm, string[1]);
        }
    }
}

void getLine() {
    char userinput[100];
    
    printf("> ");
    // Wait until the user enters a carrige return
    while(gets(buff) != NULL) {
        parseUserInput(userinput,strlen(buff));
    }    
}

int main(int argc, char *argv[]) {

    unsigned int initial_motor_delay;   
	pthread_t systemtick_th;    

    printHelp();    
    
    initial_motor_delay = motorInit();  
    
    // Initalize the two state machine objects
    init_SM( &motor0_sm, initial_motor_delay, 0, getRecipeStartAddress(0), getRecipeStartAddress(3));
    init_SM( &motor1_sm, initial_motor_delay, 1, getRecipeStartAddress(1), getRecipeStartAddress(4));
    
    // Increase thread priority of system tick above that of general IO
    pthread_setschedprio( systemtick_th, 11 );
    pthread_create(&systemtick_th, NULL, systemTick, NULL);
    

	while(1) {
        getLine();
	}

	printf("EXITING PROGRAM\n");
	return EXIT_SUCCESS;
}
