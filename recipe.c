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
RECIPE_END
};
