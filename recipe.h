/******************************************************************************
* FILENAME : recipe.h          
*
* DESCRIPTION : 
*     Store the recipe arrays
*
* AUTHOR: 
*     Donald MacIntyre - djm4912@rit.edu
*     Madison Smith    - ms8565@rit.edu  
*
******************************************************************************/
#ifndef RECIPE_H
#define RECIPE_H 

#include "UART.h"
#include <stdio.h>

#define OPCODE_MASK (0xE0)
#define PARAMETER_MASK (0x1F)
#define NUMBER_OF_RECIPES (10)
#define DESCRIPTION_LENGTH (50)


// OPCODES
#define SANDBOX (0xE0)
#define MOVE (0x20)
#define WAIT (0x40)
#define START_LOOP (0x80)
#define END_LOOP (0xA0)
#define RECIPE_END (0)

extern unsigned int *recipe_arr[10];

void printRecipes(void);
unsigned int * getRecipeStartAddress(unsigned int recipe_index);

#endif
