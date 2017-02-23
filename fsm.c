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

void parseSerialCommand(struct fsm *state_machine_params, char command) {
    
    unsigned int motorPos = 0;
    unsigned int cmd = 0;
    
    //Check if the char value is between 30-39
    //Parse it to an int
    //Process by switching, putting command value into recipe array
    
    // Parse the command - valid commands are as follows:
    // P - pause
    // C - continue
    // R - move motor right
    // L - move motor left
    // B - restart
    // N - Do nothing
    if (command == 'P') {
        // Tell the state machine we are paused, so finish any queued commands
        // but do not parse any new commands
        state_machine_params->isPaused = 1;
    } else if (command == 'C') {
        // Take the state machine out of pause allowing parsing of commands to resume
        state_machine_params->isPaused = 0;
    }     
    else if (command == 'H') {
        //Print all of the recipes
    }   
    // If the user commands to move the motor left or right this will take 
    // priority over recipes. Execution will break out of current command being parsed
    // to do a motor move.
    else if (command == 'R') {
        state_machine_params->current_state = STATE_INCREMENTAL_MOVE;
        state_machine_params->incrementalMoveDirection = 1;
    } else if (command == 'L') {
        state_machine_params->current_state = STATE_INCREMENTAL_MOVE;
        state_machine_params->incrementalMoveDirection = -1;
    } else if (command == 'B') {
        // This opcode not only restarts a recipe but also begins execution immediately 
        // taking the state machine out of pause
        state_machine_params->cmd_index = 0;
        state_machine_params->isPaused = 0;
    } 
    
    // Parse Special Sandbox only commands
    else if (state_machine_params->isSandboxMode == 1) {
        else if (command == 'S') {
            // User has requested to save the position so read the motor position and write a move command with the current position
            motorPos = getMotorPosition( state_machine_params->channel)
            cmd = MOVE | motorPos;
            state_machine_params->sandbox_arr[sandbox_cmd_index] = cmd;
            state_machine_params->sandbox_cmd_index += 1;
            
        } else if (command == 'Q') {
            // Add a RECIPE_END command when user indicates done
            cmd = RECIPE_END;
            state_machine_params->sandbox_arr[sandbox_cmd_index] = cmd;
            // Reset the sandbox command index so next time recipe is executed pointer into sandbox array will be a first element
            state_machine_params->sandbox_cmd_index = 0;
            
            // break out of sandbox, move onto the next opcode
            // This is incrementing the counter in the CURRENT recipe, NOT the sandbox reciepe
            state_machine_params->cmd_index += 1;
            
        }
    }
    
    // Invalid command or a no-op so do nothing 
    else {
        
    }
}

void init_SM( struct fsm *state_machine_params, unsigned int delay, unsigned int channel, unsigned int *instruction_arr, unsigned int *sandbox_arr) {
    // Set current state to wait to wait for motors to reach home position
    state_machine_params->current_state = STATE_WAIT;
    state_machine_params->delay = delay;
    state_machine_params->channel = channel;
    state_machine_params->instruction_arr = instruction_arr;
    state_machine_params->cmd_index = 0;
    state_machine_params->loop_start_index = 0;
    state_machine_params->inLoopFlag = 0;
    state_machine_params->iterationsLeftToLoop = 0;
    // Pause the recipe on startup
    state_machine_params->isPaused = 1;
    state_machine_params->isSandboxMode = 0;
    state_machine_params->sandbox_arr = sandbox_arr;
    state_machine_params->sandbox_cmd_index = 0;
    
}

void process_SM(struct fsm *state_machine_params) {
    
    unsigned int  current_opcode = 0;
    unsigned int current_argument = 0;
    
    switch(state_machine_params->current_state) {
        case STATE_PARSE:
            
            // Do not parse the current command unless we are not paused
            if (state_machine_params->isPaused == 0) {
            
                // Get the current opcode
                current_opcode = state_machine_params->instruction_arr[state_machine_params->cmd_index];
                // Get the current paramater
                current_argument = state_machine_params->instruction_arr[state_machine_params->cmd_index];
                current_opcode &= OPCODE_MASK;
                current_argument &= PARAMETER_MASK;
                
                // Parse the opcode and decide what to do!!
                switch(current_opcode) {
                    case(SANDBOX):
                        state_machine_params->current_state = STATE_PARSE;
                        break;
                    
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
            }
            break;
                
        case STATE_INCREMENTAL_MOVE:
            state_machine_params->delay = incrementalMotorMove( state_machine_params->channel, state_machine_params->incrementalMoveDirection);
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
        case STATE_ERROR:
        default:
            // Invalid opcode or a nested loop 
            state_machine_params->current_state = STATE_ERROR;
            break;
    } // end switch
}
