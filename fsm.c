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
    state_machine_params->loop_start_index = 0;
    state_machine_params->inLoopFlag = 0;
    state_machine_params->iterationsLeftToLoop = 0;
}

void process_SM(struct fsm *state_machine_params) {
    
    unsigned int  current_opcode = 0;
    unsigned int current_argument = 0;
    
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
                    state_machine_params->delay = setMotorPosition( state_machine_params->channel, current_argument);
                    state_machine_params->current_state = STATE_WAIT;
                    state_machine_params->cmd_index += 1;
                    break;
                case (WAIT):
                    // Don't go to wait for a wait of 0 just increment to the next command in the recipe
                    if (current_argument == 0) {
                        state_machine_params->cmd_index += 1;
                    } else {
                        state_machine_params->delay = current_argument;
                        state_machine_params->current_state = STATE_WAIT;
                        state_machine_params->cmd_index += 1;
                    }
                    break;
                case (START_LOOP):
                    // If we are aleady in a loop and attempt to process another that is a nested loop go to error state
                    if (state_machine_params->inLoopFlag == 1) {
                        state_machine_params->current_state = STATE_ERROR;
                    }
                    // set in loop flag to indicate that we are in a loop
                    state_machine_params->inLoopFlag = 1;
                    // Current index is a start loop command -> therefore the first instruction of the loop is located at current instruction + 1
                    state_machine_params->loop_start_index = (state_machine_params->cmd_index) + 1;
                    // Set the number of iterations to remain in this loop
                    state_machine_params->iterationsLeftToLoop = current_argument;
                    
                    // We are now all setup to loop, increment to the next command and start executing loop body
                    state_machine_params->cmd_index += 1;
                    state_machine_params->current_state = STATE_PARSE;
                    break;
                case (END_LOOP):
                    // Have reached the end of the loop body 
                    
                    // If the loop is done executing 
                    if (state_machine_params->iterationsLeftToLoop == 0) {
                        state_machine_params->current_state = STATE_PARSE;
                        state_machine_params->cmd_index += 1;
                        state_machine_params->inLoopFlag = 0;
                    }
                    // else the loop is not done executing so keep on looping
                    else {
                        // decrement the number of times left to loop
                        // It may seem strange to decrement here and not before checking but requirements specifically state
                        // that loops are executed once plus n more times, so checking here allows for this behavior
                        state_machine_params->iterationsLeftToLoop -= 1;
                        state_machine_params->current_state = STATE_PARSE;
                        state_machine_params->cmd_index = state_machine_params->loop_start_index;
                    }
                    break;
                case (RECIPE_END):
                    // Don't need to do anything here, the recipe is over so just remain here
                    state_machine_params->current_state = STATE_PARSE;
                    break;
                // Invalid opcode
                default:
                    state_machine_params->current_state = STATE_ERROR;
            } // end opcode parse
						break;
        case STATE_WAIT:
            // Decrement the delay by 100 ms
            state_machine_params->delay -= 1;
            // If we are done waiting then go parse the next command
            if (state_machine_params->delay == 0) {
                state_machine_params->current_state = STATE_PARSE;
            }
            break;
        case STATE_ERROR:
        default:
            // Invalid opcode or a nested loop 
            state_machine_params->current_state = STATE_ERROR;
            break;
    } // end switch
}
