/*
 * GccApplication4.c
 *
 * Created: 2/5/2021 7:59:19 PM
 * Author : Desktop
 */ 


#include "sam.h"
uint32_t count = 0;

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		count = count + 1;
		if (count > 100000)
		{
			count = count + 2;
			count = 0;
		}
		
		
    }
}
