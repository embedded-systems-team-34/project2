/******************************************************************************
* FILENAME : fsm.h          
*
* DESCRIPTION : 
*     Function prototypes for finite state machine 
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/
#ifndef FSM_H
#define FSM_H 

#include "motor.h"
#include "recipe.h"

typedef enum {
    STATE_PARSE,
    STATE_WAIT,
    STATE_ERROR
} state_t;

struct fsm {
    state_t current_state;
    unsigned int delay;
    unsigned int channel;
    unsigned int *instruction_arr;
    unsigned int cmd_index;
    // Index pointing to the start of a loop 
    unsigned int loop_start_index;
    // 0 means we are not currently in a loop
    // 1 means we are currently in a loop
    unsigned int inLoopFlag;
    // Count of number of iterations to loop over
    unsigned int iterationsLeftToLoop;
    
};

// function prototypes
void init_SM( struct fsm *state_machine_params, unsigned int delay, unsigned int channel, unsigned int *instruction_arr);
void process_SM(struct fsm *state_machine_params);

#endif
