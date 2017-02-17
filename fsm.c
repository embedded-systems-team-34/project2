/******************************************************************************
* FILENAME : fsm.c          
*
* DESCRIPTION : 
*     Finite state machine for executing recipes 
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/

#include "fsm.h"

void init_SM( struct fsm *state_machine_params, unsigned int delay, unsigned int channel, unsigned char *instruction_arr) {
    // Set current state to wait to wait for motors to reach home position
    state_machine_params->current_state = STATE_WAIT;
    state_machine_params->delay = delay;
    state_machine_params->channel = channel;
    state_machine_params->instruction_arr = instruction_arr;
}

void process_SM(struct fsm *state_machine_params) {
    
    static unsigned int  current_opcode;
    static unsigned int current_argument = 0;
    
    switch(state_machine_params->current_state) {
        case STATE_PARSE:
            #warning RECIPE PARSING CURRENTLY NOT IMPLEMENTED
            current_opcode = 0;
            current_argument = (current_argument + 1) % 6;
            state_machine_params->current_state = STATE_MOVE;
        case STATE_MOVE:
            state_machine_params->delay = setMotorPosition( state_machine_params->delay, current_argument);
            state_machine_params->current_state = STATE_WAIT;
        case STATE_WAIT:
            // Decrement the delay by 100 ms
            state_machine_params->delay -= 1;
            // If we are done waiting then go parse the next command
            if (state_machine_params->delay == 0) {
                state_machine_params->current_state = STATE_PARSE;
            }
            break;
        default:
            break;
    } // end switch
}
