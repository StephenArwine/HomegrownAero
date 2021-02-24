#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	
	long count = 0;
	
	while (1) {
		count = count + 1;
		
		if (count > 10000000)
		{
			count = 0;
		}
	}
}
