/******************************************************************************
* FILENAME : recipe.c          
*
* DESCRIPTION : 
*     Functions for manupulating recipes
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/

#include "recipe.h"

unsigned int cmd_arr1[100] = { 
MOVE | 5,
MOVE | 4,
MOVE | 3,
MOVE | 2,
MOVE | 1,
MOVE | 0,
START_LOOP | 3,
MOVE | 5,
MOVE | 3,
MOVE | 0,
END_LOOP,
WAIT | 30,
RECIPE_END
};

unsigned int cmd_arr2[100] = { 
MOVE | 5,
WAIT | 30,
MOVE | 4,
WAIT | 30,
MOVE | 3,
WAIT | 30,
MOVE | 2,
WAIT | 30,
MOVE | 1,
WAIT | 30,
MOVE | 0,
WAIT | 30,
RECIPE_END
};

unsigned int cmd_sandbox_creation_motor0[100] = {
MOVE | 5,
WAIT | 30,
MOVE | 4,
WAIT | 30,
MOVE | 3,
WAIT | 30,
MOVE | 2,
WAIT | 30,
MOVE | 1,
SANDBOX,
MOVE | 5,
RECIPE_END
};

unsigned int cmd_pyramid[100] = {
START_LOOP | 3,
MOVE | 5,
MOVE | 4,
MOVE | 5,
MOVE | 3,
MOVE | 5,
MOVE | 2,
MOVE | 5,
MOVE | 1,
MOVE | 5,
MOVE | 0,
MOVE | 5,
MOVE | 0,
MOVE | 1,
MOVE | 0,
MOVE | 2,
MOVE | 0,
MOVE | 3,
MOVE | 0,
MOVE | 4,
MOVE | 0,
MOVE | 5,
END_LOOP,
WAIT | 1,
RECIPE_END
};

unsigned int cmd_provided[100] = {
MOVE | 0,
MOVE | 5,
MOVE | 0,
MOVE | 3,
START_LOOP | 0,
MOVE | 1,
MOVE | 4,
END_LOOP,
MOVE | 0,
MOVE | 2,
WAIT | 0,
MOVE | 3,
WAIT | 0,
MOVE | 2,
MOVE | 3,
WAIT | 31,
WAIT | 31,
WAIT | 31,
MOVE | 4,
RECIPE_END      
};

unsigned int cmd_loop_err[100] = {
MOVE | 0,
MOVE | 1,
MOVE | 2,
MOVE | 3,
START_LOOP,
MOVE | 5,
MOVE | 3,
START_LOOP,
MOVE | 4,
END_LOOP,
END_LOOP,
MOVE | 5,
RECIPE_END      
};

unsigned int cmd_invalid_op[100] = {
MOVE | 0,
MOVE | 1,
MOVE | 2,
MOVE | 3,
0x60,
RECIPE_END      
};

unsigned int cmd_invalid_param[100] = {
MOVE | 0,
MOVE | 1,
MOVE | 2,
MOVE | 3,
MOVE | 10,
RECIPE_END      
};

unsigned int cmd_arr_sandbox_motor0[100] = {0};
unsigned int cmd_arr_sandbox_motor1[100] = {0};
unsigned int *recipe_arr[NUMBER_OF_RECIPES] = {
    cmd_arr1, 
    cmd_arr2, 
    cmd_sandbox_creation_motor0,
    cmd_arr_sandbox_motor0, 
    cmd_arr_sandbox_motor1,
    cmd_pyramid,
    cmd_provided,
    cmd_loop_err,
    cmd_invalid_op,
    cmd_invalid_param}; 

char recipes_description[NUMBER_OF_RECIPES][DESCRIPTION_LENGTH] = {
    "0 - Test Recipe 1",
    "1 - Test Recipe 2",
    "2 - Recipe plus sandbox Creation Recipe",
    "3 - Sandbox Recipe Motor 1 - DO NOT REMOVE",
    "4 - Sandbox Recipe Motor 2- DO NOT REMOVE",
    "5 - Pyramids",
    "6 - Provided Recipe (9.3 second wait)",
    "7 - Nested Loop Error",
    "8 - Invalid OP-code",
    "9 - Invalid Move Paramater"
};

void printRecipes() {
	int		n ;
    unsigned int i = 0;
    uint8_t buffer[100];
    
    for (i = 0; i < NUMBER_OF_RECIPES; i++) {
        n = sprintf((char *)buffer, "%s\r\n", recipes_description[i]);
        USART_Write(USART2, buffer, n);	
    }
}

unsigned int * getRecipeStartAddress(unsigned int recipe_index) {
    return recipe_arr[recipe_index];
}
