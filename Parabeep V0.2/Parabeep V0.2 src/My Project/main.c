#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	volatile long count = 0;
	while (1) {
		count = count + 1;
	}
}
