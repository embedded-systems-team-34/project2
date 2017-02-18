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

void init_SM( struct fsm *state_machine_params, unsigned int delay, unsigned int channel, unsigned int *instruction_arr) {
    // Set current state to wait to wait for motors to reach home position
    state_machine_params->current_state = STATE_WAIT;
    state_machine_params->delay = delay;
    state_machine_params->channel = channel;
    state_machine_params->instruction_arr = instruction_arr;
    state_machine_params->cmd_index = 0;
}

void process_SM(struct fsm *state_machine_params) {
    
    static unsigned int  current_opcode = 0;
    static unsigned int current_argument = 0;
    
    switch(state_machine_params->current_state) {
        case STATE_PARSE:
            
            // Get the current opcode
            current_opcode = state_machine_params->instruction_arr[state_machine_params->cmd_index];
            // Get the current paramater
            current_argument = state_machine_params->instruction_arr[state_machine_params->cmd_index];
            current_opcode &= OPCODE_MASK;
            current_argument &= PARAMETER_MASK;
            
            // Parse the opcode and decide what to do!!
            switch(current_opcode) {
                case (MOVE):
                    state_machine_params->current_state = STATE_MOVE;
                    state_machine_params->cmd_index += 1;
                    break;
                case (WAIT):
                    // Don't go to wait for a wait of 0
                    if (current_argument == 0) {
                        state_machine_params->cmd_index += 1;
                    } else {
                        state_machine_params->delay = current_argument;
                        state_machine_params->current_state = STATE_WAIT;
                        state_machine_params->cmd_index += 1;
                    }
                    break;
                case (START_LOOP):
                    #warning NOT YET IMPLEMENTED
                    break;
                case (END_LOOP):
                    #warning NOT YET IMPLEMENTED
                    break;
                case (RECIPE_END):
                    #warning NOT YET IMPLEMENTED
                    state_machine_params->current_state = STATE_PARSE;
                    break;
                // Invalid opcode
                default:
                    ;
                    #warning NOT YET IMPLEMENTED
            // No break here is by design, we want to fall through and start parsing command 
            // on the same cycle as parsing so as not to add in an extra delay of 100 ms
                    
            } // end opcode parse
        case STATE_MOVE:
            state_machine_params->delay = setMotorPosition( state_machine_params->channel, current_argument);
            state_machine_params->current_state = STATE_WAIT;
            break;
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
