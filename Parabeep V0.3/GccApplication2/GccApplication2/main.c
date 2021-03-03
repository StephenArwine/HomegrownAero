/*
 * GccApplication2.c
 *
 * Created: 2/20/2021 3:24:06 PM
 * Author : Desktop
 */ 


#include "sam.h"


int main(void)
{
    /* Replace with your application code */
	
	long count = 0;
    while (1) 
    {
		count =  count + 1;
		
		
		if (count > 10000000)
		{
			count = 1;
		}
    }
}
