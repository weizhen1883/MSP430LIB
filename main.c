#include <msp430g2553.h>
#include <stdint.h>
#include "gpio.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	BCSCTL1 = CALBC1_1MHZ; 		// Set oscillator to 1MHz
	DCOCTL = CALDCO_1MHZ;  		// Set oscillator to 1MHz

	gpio_init("P1.0", "OUTPUT_MODE");
	gpio_init("P1.6", "OUTPUT_MODE");
	gpio_init("P1.3", "INPUT_MODE_PULLUP");
	gpio_interrupt_enable("P1.3", "INTERRUPT_LOW_TO_HIGH");
	gpio_init("P2.0", "INPUT_MODE_WITHOUT_PULLUP/PULLDOWN");
	gpio_interrupt_enable("P2.0", "INTERRUPT_LOW_TO_HIGH");
	
	__bis_SR_register(GIE); 

	gpio_output("P1.0", "ON");
	while (1) {
		if (gpio_interrupt_check("P1.3") == 1) {
			gpio_output("P1.0", "TOGGLE");
		}
		if (gpio_interrupt_check("P2.0") == 1) gpio_output("P1.6", "TOGGLE");
	}

	return 0;
}