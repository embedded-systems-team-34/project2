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
#ifndef fsm_H
#define fsm_H 

#include "motor.h"

typedef enum {
    STATE_PARSE,
    STATE_MOVE,
    STATE_WAIT
} state_t;

struct fsm {
    state_t current_state;
    unsigned int delay;
    unsigned int channel;
    unsigned char *instruction_arr;
};

// function prototypes
void init_SM( struct fsm *state_machine_params, unsigned int delay, unsigned int channel, unsigned char *instruction_arr);
void process_SM(struct fsm *state_machine_params);

#endif
