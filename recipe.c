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
MOVE | 2,
MOVE | 3,
WAIT | 31,
WAIT | 31,
WAIT | 31,
MOVE | 4,
RECIPE_END
};
