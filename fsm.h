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
    STATE_INCREMENTAL_MOVE,
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
    // 1 means that recipe execution is paused
    // 0 means that recipe execution is live (not paused)
    unsigned int isPaused;
    // Direction to move on an incremental move +1 is right, -1 is left
    int incrementalMoveDirection;
    // 1 indicates sandbox mode is active
    // 0 indicates sandbox is not active
    unsigned int isSandboxMode;
    unsigned int *sandbox_arr;
    unsigned int sandbox_cmd_index;
    
};

// function prototypes
void init_SM( struct fsm *state_machine_params, unsigned int delay, unsigned int channel, unsigned int *instruction_arr, unsigned int *sandbox_arr);
void process_SM(struct fsm *state_machine_params);
void parseSerialCommand(struct fsm *state_machine_params, char command);
unsigned int isAnInt(char command);
void printHelp(void);

#endif
