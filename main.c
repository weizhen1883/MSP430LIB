#include <msp430g2553.h>
#include <stdint.h>
#include "gpio.h"

void interrupt_callback_P1_3();
void interrupt_callback_P2_0();

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	BCSCTL1 = CALBC1_1MHZ; 		// Set oscillator to 1MHz
	DCOCTL = CALDCO_1MHZ;  		// Set oscillator to 1MHz

	gpio_init("P1.0", "OUTPUT_MODE");
	gpio_init("P1.6", "OUTPUT_MODE");
	gpio_init("P1.3", "INPUT_MODE_PULLUP");
	gpio_interrupt_enable("P1.3", "INTERRUPT_HIGH_TO_LOW", interrupt_callback_P1_3);
	gpio_init("P2.0", "INPUT_MODE_WITHOUT_PULLUP/PULLDOWN");
	gpio_interrupt_enable("P2.0", "INTERRUPT_LOW_TO_HIGH", interrupt_callback_P2_0);
	
	__bis_SR_register(GIE + LPM0_bits); 
	// __bis_SR_register(GIE); 

	// gpio_output("P1.0", "ON");
	// while (1) {
	// 	if (gpio_input("P1.3") == 0) {
	// 		gpio_output("P1.0", "TOGGLE");
	// 	}
	// 	if (gpio_input("P2.0") == 1) gpio_output("P1.6", "TOGGLE");
	// }

	return 0;
}

void interrupt_callback_P1_3() {
	gpio_output("P1.0", "TOGGLE");
	gpio_output("P1.6", "TOGGLE");
}

void interrupt_callback_P2_0() {
	gpio_output("P1.6", "TOGGLE");
}