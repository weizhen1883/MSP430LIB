#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include "gpio.h"

uint8_t gpio_interrupt_flag[] = {0x00, 0x00};

uint8_t get_pin_number(char *pinNumber, char *port) {
	port[0] = pinNumber[0];
	port[1] = pinNumber[1];

	return pinNumber[3] - '0';
}

uint8_t set_pin_mode(char *pinMode, char *port, uint8_t pin) {
	if (strncmp(pinMode, "OUTPUT_MODE", 11) == 0) {
		switch (port[1] - '0') {
			case 1:
				P1OUT &= ~(0x01 << pin);
				P1DIR |= (0x01 << pin);
				P1REN &= ~(0x01 << pin);
				P1SEL &= ~(0x01 << pin);
				P1SEL2 &= ~(0x01 << pin);
				break;
			case 2:
				P2OUT &= ~(0x01 << pin);
				P2DIR |= (0x01 << pin);
				P2REN &= ~(0x01 << pin);
				P2SEL &= ~(0x01 << pin);
				P2SEL2 &= ~(0x01 << pin);
				break;
			case 3:
				P3OUT &= ~(0x01 << pin);
				P3DIR |= (0x01 << pin);
				P3REN &= ~(0x01 << pin);
				P3SEL &= ~(0x01 << pin);
				P3SEL2 &= ~(0x01 << pin);
				break;
		}
		return 1;
	} else if (strncmp(pinMode, "INPUT_MODE_WITHOUT_PULLUP/PULLDOWN", 34) == 0) {
		switch (port[1] - '0') {
			case 1:
				P1OUT &= ~(0x01 << pin);
				P1DIR &= ~(0x01 << pin);
				P1REN &= ~(0x01 << pin);
				P1SEL &= ~(0x01 << pin);
				P1SEL2 &= ~(0x01 << pin);
				break;
			case 2:
				P2OUT &= ~(0x01 << pin);
				P2DIR &= ~(0x01 << pin);
				P2REN &= ~(0x01 << pin);
				P2SEL &= ~(0x01 << pin);
				P2SEL2 &= ~(0x01 << pin);
				break;
			case 3:
				P3OUT &= ~(0x01 << pin);
				P3DIR &= ~(0x01 << pin);
				P3REN &= ~(0x01 << pin);
				P3SEL &= ~(0x01 << pin);
				P3SEL2 &= ~(0x01 << pin);
				break;
		}
		return 1;
	} else if (strncmp(pinMode, "INPUT_MODE_PULLUP", 17) == 0) {
		switch (port[1] - '0') {
			case 1:
				P1OUT |= (0x01 << pin);
				P1DIR &= ~(0x01 << pin);
				P1REN |= (0x01 << pin);
				P1SEL &= ~(0x01 << pin);
				P1SEL2 &= ~(0x01 << pin);
				break;
			case 2:
				P2OUT |= (0x01 << pin);
				P2DIR &= ~(0x01 << pin);
				P2REN |= (0x01 << pin);
				P2SEL &= ~(0x01 << pin);
				P2SEL2 &= ~(0x01 << pin);
				break;
			case 3:
				P3OUT |= (0x01 << pin);
				P3DIR &= ~(0x01 << pin);
				P3REN |= (0x01 << pin);
				P3SEL &= ~(0x01 << pin);
				P3SEL2 &= ~(0x01 << pin);
				break;
		}
		return 1;
	} else if (strncmp(pinMode, "INPUT_MODE_PULLDOWN", 19) == 0) {
		switch (port[1] - '0') {
			case 1:
				P1OUT &= ~(0x01 << pin);
				P1DIR &= ~(0x01 << pin);
				P1REN |= (0x01 << pin);
				P1SEL &= ~(0x01 << pin);
				P1SEL2 &= ~(0x01 << pin);
				break;
			case 2:
				P2OUT &= ~(0x01 << pin);
				P2DIR &= ~(0x01 << pin);
				P2REN |= (0x01 << pin);
				P2SEL &= ~(0x01 << pin);
				P2SEL2 &= ~(0x01 << pin);
				break;
			case 3:
				P3OUT &= ~(0x01 << pin);
				P3DIR &= ~(0x01 << pin);
				P3REN |= (0x01 << pin);
				P3SEL &= ~(0x01 << pin);
				P3SEL2 &= ~(0x01 << pin);
				break;
		}
		return 1;
	} else if (strncmp(pinMode, "INTERRUPT_LOW_TO_HIGH", 21) == 0) {
		switch (port[1] - '0') {
			case 1:
				P1IE |= (0x01 << pin);
				P1IES &= ~(0x01 << pin);
				P1IFG &= ~(0x01 << pin);
				break;
			case 2:
				P2IE |= (0x01 << pin);
				P2IES &= ~(0x01 << pin);
				P2IFG &= ~(0x01 << pin);
				break;
		}
		return 1;
	} else if (strncmp(pinMode, "INTERRUPT_HIGH_TO_LOW", 21) == 0) {
		switch (port[1] - '0') {
			case 1:
				P1IE |= (0x01 << pin);
				P1IES |= (0x01 << pin);
				P1IFG &= ~(0x01 << pin);
				break;
			case 2:
				P2IE |= (0x01 << pin);
				P2IES |= (0x01 << pin);
				P2IFG &= ~(0x01 << pin);
				break;
		}
		return 1;
	} else if (strncmp(pinMode, "GPIO_OFF", 8) == 0) {
		switch (port[1] - '0') {
			case 1:
				P1OUT &= ~(0x01 << pin);
				break;
			case 2:
				P2OUT &= ~(0x01 << pin);
				break;
			case 3:
				P3OUT &= ~(0x01 << pin);
				break;
		}
		return 1;
	} else if (strncmp(pinMode, "GPIO_ON", 7) == 0) {
		switch (port[1] - '0') {
			case 1:
				P1OUT |= (0x01 << pin);
				break;
			case 2:
				P2OUT |= (0x01 << pin);
				break;
			case 3:
				P3OUT |= (0x01 << pin);
				break;
		}
		return 1;
	} else if (strncmp(pinMode, "GPIO_TOGGLE", 11) == 0) {
		switch (port[1] - '0') {
			case 1:
				P1OUT ^= (0x01 << pin);
				break;
			case 2:
				P2OUT ^= (0x01 << pin);
				break;
			case 3:
				P3OUT ^= (0x01 << pin);
				break;
		}
		return 1;
	}
	return 0;
}

uint8_t gpio_init(char *pinNumber, char *pinMode) {
	char port[2];
	uint8_t pin = get_pin_number(pinNumber, port);
	set_pin_mode(pinMode, port, pin);
	return 0;
}

uint8_t gpio_output(char *pinNumber, char *status) {
	char port[2];
	uint8_t pin = get_pin_number(pinNumber, port);
	if (strncmp(status, "ON", 2) == 0) {
		set_pin_mode("GPIO_ON", port, pin);
	} else if (strncmp(status, "TOGGLE", 6) == 0) {
		set_pin_mode("GPIO_TOGGLE", port, pin);
	} else {
		set_pin_mode("GPIO_OFF", port, pin);
	} 
	return 0;
}

uint8_t gpio_input(char *pinNumber) {
	uint8_t result = 0;
	char port[2];
	uint8_t pin = get_pin_number(pinNumber, port);
	switch (port[1] - '0') {
		case 1:
			result = (P1IN & (0x01 << pin)) >> pin;
			break;
		case 2:
			result = (P2IN & (0x01 << pin)) >> pin;
			break;
		case 3:
			result = (P3IN & (0x01 << pin)) >> pin;
			break;
	}
	return result;
}

uint8_t gpio_interrupt_enable(char *pinNumber, char *interruptMode) {
	char port[2];
	uint8_t pin = get_pin_number(pinNumber, port);
	set_pin_mode(interruptMode, port, pin);
	return 0;
}

uint8_t gpio_interrupt_check(char *pinNumber) {
	uint8_t result = 0;
	char port[2];
	uint8_t pin = get_pin_number(pinNumber, port);
	switch (port[1] - '0') {
		case 1:
			result = (gpio_interrupt_flag[0] & (0x01 << pin)) >> pin;
			gpio_interrupt_flag[0] &= ~(0x01 << pin);
			break;
		case 2:
			result = (gpio_interrupt_flag[1] & (0x01 << pin)) >> pin;
			gpio_interrupt_flag[1] &= ~(0x01 << pin);
			break;
	}
	return result;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) { 
	if (P1IFG & BIT0) {
		gpio_interrupt_flag[0] |= (0x01 << 0);
		P1IFG &= ~BIT0;
	} else if (P1IFG & BIT1) {
		gpio_interrupt_flag[0] |= (0x01 << 1);
		P1IFG &= ~BIT1;
	} else if (P1IFG & BIT2) {
		gpio_interrupt_flag[0] |= (0x01 << 2);
		P1IFG &= ~BIT2;
	} else if (P1IFG & BIT3) {
		gpio_interrupt_flag[0] = (0x01 << 3);
		P1IFG &= ~BIT3;
	} else if (P1IFG & BIT4) {
		gpio_interrupt_flag[0] |= (0x01 << 4);
		P1IFG &= ~BIT4;
	} else if (P1IFG & BIT5) {
		gpio_interrupt_flag[0] |= (0x01 << 5);
		P1IFG &= ~BIT5;
	} else if (P1IFG & BIT6) {
		gpio_interrupt_flag[0] |= (0x01 << 6);
		P1IFG &= ~BIT6;
	} else if (P1IFG & BIT7) {
		gpio_interrupt_flag[0] |= (0x01 << 7);
		P1IFG &= ~BIT7;
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) { 
	if (P2IFG & BIT0) {
		gpio_interrupt_flag[1] |= (0x01 << 0);
		P2IFG &= ~BIT0;
	} else if (P2IFG & BIT1) {
		gpio_interrupt_flag[1] |= (0x01 << 1);
		P2IFG &= ~BIT1;
	} else if (P2IFG & BIT2) {
		gpio_interrupt_flag[1] |= (0x01 << 2);
		P2IFG &= ~BIT2;
	} else if (P2IFG & BIT3) {
		gpio_interrupt_flag[1] = (0x01 << 3);
		P2IFG &= ~BIT3;
	} else if (P2IFG & BIT4) {
		gpio_interrupt_flag[1] |= (0x01 << 4);
		P2IFG &= ~BIT4;
	} else if (P2IFG & BIT5) {
		gpio_interrupt_flag[1] |= (0x01 << 5);
		P2IFG &= ~BIT5;
	} else if (P2IFG & BIT6) {
		gpio_interrupt_flag[1] |= (0x01 << 6);
		P2IFG &= ~BIT6;
	} else if (P2IFG & BIT7) {
		gpio_interrupt_flag[1] |= (0x01 << 7);
		P2IFG &= ~BIT7;
	}
}
