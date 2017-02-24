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

unsigned int cmd_arr_sandbox_motor0[100] = {0};
unsigned int cmd_arr_sandbox_motor1[100] = {0};
unsigned int *recipe_arr[NUMBER_OF_RECIPES] = {
    cmd_arr1, 
    cmd_arr2, 
    cmd_arr_sandbox_motor0, 
    cmd_arr_sandbox_motor1}; 

char recipes_description[NUMBER_OF_RECIPES][DESCRIPTION_LENGTH] = {
    "0 - ENTER DESCRIPTION HERE",
    "1 - ENTER DESCRIPTION HERE",
    "2 - Sandbox Recipe Motor 0",
    "3 - Sandbox Recipe Motor 1",
    "NOT CURRENTLY USED",
    "NOT CURRENTLY USED",
    "NOT CURRENTLY USED",
    "NOT CURRENTLY USED",
    "NOT CURRENTLY USED",
    "NOT CURRENTLY USED"
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
