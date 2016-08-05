#include <msp430g2553.h>
#include <stdint.h>
#include "gpio.h"
// #include "usci.h"

void interrupt_callback_P1_3();
// void interrupt_callback_P2_0();
// void uart_interrupt_callback(char *buf);

// void configure_clocks(void);

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// configure_clocks();
	BCSCTL1 = CALBC1_8MHZ; 		// Set oscillator to 1MHz
	DCOCTL = CALDCO_8MHZ;  		// Set oscillator to 1MHz

	gpio_init("P1.0", "OUTPUT_MODE");
	gpio_init("P1.6", "OUTPUT_MODE");
	gpio_init("P1.3", "INPUT_MODE_PULLUP");
	gpio_interrupt_enable("P1.3", "INTERRUPT_HIGH_TO_LOW", interrupt_callback_P1_3);
	// gpio_init("P2.0", "INPUT_MODE_WITHOUT_PULLUP/PULLDOWN");
	// gpio_interrupt_enable("P2.0", "INTERRUPT_LOW_TO_HIGH", interrupt_callback_P2_0);

    // usci_init("UART", "115200", uart_interrupt_callback);
	
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
	gpio_output("P1.6", "TOGGLE");

    gpio_output("P1.0", "ON");
    __delay_cycles(1000000);
    gpio_output("P1.0", "OFF");

}

// void interrupt_callback_P2_0() {
// 	gpio_output("P1.6", "TOGGLE");
// }

// void uart_interrupt_callback(char *buf) {
//     uart.write("The message is:");
//     uart.write(buf);
//     uart.write("\r\n");
// }

// void configure_clocks(void) {
//     uint8_t dcoctl = 0x00;
//     uint8_t bcsctl1 = 0x00;
//     uint8_t bcsctl2 = 0x00;

//     /*
//      * DCO - internal digitally controlled oscillator
//      * use calibrated values in Information Memory Segment A
//      * to set a precise clock speed of 16 MHz
//      */
//     dcoctl = CALDCO_16MHZ;
//     bcsctl1 = CALBC1_16MHZ;

    
//      * SYSTEM Auxiliary Clock
//      * use VLOCLK (internal very low power clock)
//      * ~12 kHz, but wide tolerance part-to-part
//      * use 1 prescale keeping ~12 kHz
     
//     bcsctl1 |= 0; /* DIVA = 0 */

//     /*
//      * SYSTEM Master Clock
//      * use DCO w/ 1 prescale
//      * making MCLK = 16 MHz
//      */
//     bcsctl2 |= 0; /* SELM = 0 */
//     bcsctl2 |= 0; /* DIVM = 0 */

//     /*
//      * SYSTEM Sub-Main Clock
//      * use DCO w/ 2 prescale
//      * making SMCLK = 8 MHz
//      */
//     bcsctl2 |= 0; /* SELS = 0 */
//     bcsctl2 |= DIVS0; /* DIVS = 0b01 */

//     DCOCTL = dcoctl;
//     BCSCTL1 = bcsctl1;
//     BCSCTL2 = bcsctl2;
// }