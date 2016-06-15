#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include "gpio.h"

#define SET_BIT(reg, shift, value) (*reg = (((*reg) & ~(1 << (shift))) | ((value) << (shift))))

typedef struct {
	uint8_t bit;
	gpio_interrupt_callback *callback;
} gpio_interrupt_callback_table;

typedef struct {
	uint16_t *in;
    uint16_t *out;
    uint16_t *dir;
    uint16_t *ren;
    uint16_t *sel;
    uint16_t *sel2;
} gpio_registers;

typedef struct {
	uint16_t *ifg;
	uint16_t *ies;
	uint16_t *ie;
} gpio_interrupt_registers;

gpio_interrupt_callback_table P1_interrupt_callback[8];
gpio_interrupt_callback_table P2_interrupt_callback[8];

gpio_registers gpio_registers_map[] = {
	{ (uint16_t *) P1IN_ , (uint16_t *) P1OUT_ , (uint16_t *) P1DIR_ , (uint16_t *) P1REN_ , (uint16_t *) P1SEL_ , (uint16_t *) P1SEL2_ },
    { (uint16_t *) P2IN_ , (uint16_t *) P2OUT_ , (uint16_t *) P2DIR_ , (uint16_t *) P2REN_ , (uint16_t *) P2SEL_ , (uint16_t *) P2SEL2_ },
    { (uint16_t *) P3IN_ , (uint16_t *) P3OUT_ , (uint16_t *) P3DIR_ , (uint16_t *) P3REN_ , (uint16_t *) P3SEL_ , (uint16_t *) P3SEL2_ },
};

gpio_interrupt_registers gpio_interrupt_registers_map[] = {
	{ (uint16_t *) P1IFG_, (uint16_t *) P1IES_, (uint16_t *) P1IE_ },
	{ (uint16_t *) P2IFG_, (uint16_t *) P2IES_, (uint16_t *) P2IE_ },
};

uint8_t get_pin_number(char *pinNumber, char *port) {
	port[0] = pinNumber[0];
	port[1] = pinNumber[1];

	return pinNumber[3] - '0';
}

uint8_t set_pin_mode(char *pinMode, char *port, uint8_t pin) {
	if (strncmp(pinMode, "OUTPUT_MODE", 11) == 0) {
		SET_BIT(gpio_registers_map[(port[1] - '1')].out, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].dir, pin, 1);
		SET_BIT(gpio_registers_map[(port[1] - '1')].ren, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].sel, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].sel2, pin, 0);
		return 1;
	} else if (strncmp(pinMode, "INPUT_MODE_WITHOUT_PULLUP/PULLDOWN", 34) == 0) {
		SET_BIT(gpio_registers_map[(port[1] - '1')].out, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].dir, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].ren, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].sel, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].sel2, pin, 0);
		return 1;
	} else if (strncmp(pinMode, "INPUT_MODE_PULLUP", 17) == 0) {
		SET_BIT(gpio_registers_map[(port[1] - '1')].out, pin, 1);
		SET_BIT(gpio_registers_map[(port[1] - '1')].dir, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].ren, pin, 1);
		SET_BIT(gpio_registers_map[(port[1] - '1')].sel, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].sel2, pin, 0);
		return 1;
	} else if (strncmp(pinMode, "INPUT_MODE_PULLDOWN", 19) == 0) {
		SET_BIT(gpio_registers_map[(port[1] - '1')].out, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].dir, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].ren, pin, 1);
		SET_BIT(gpio_registers_map[(port[1] - '1')].sel, pin, 0);
		SET_BIT(gpio_registers_map[(port[1] - '1')].sel2, pin, 0);
		return 1;
	} else if (strncmp(pinMode, "INTERRUPT_LOW_TO_HIGH", 21) == 0) {
		SET_BIT(gpio_interrupt_registers_map[(port[1] - '1')].ie, pin, 1);
		SET_BIT(gpio_interrupt_registers_map[(port[1] - '1')].ies, pin, 0);
		SET_BIT(gpio_interrupt_registers_map[(port[1] - '1')].ifg, pin, 0);
		return 1;
	} else if (strncmp(pinMode, "INTERRUPT_HIGH_TO_LOW", 21) == 0) {
		SET_BIT(gpio_interrupt_registers_map[(port[1] - '1')].ie, pin, 1);
		SET_BIT(gpio_interrupt_registers_map[(port[1] - '1')].ies, pin, 1);
		SET_BIT(gpio_interrupt_registers_map[(port[1] - '1')].ifg, pin, 0);
		return 1;
	} else if (strncmp(pinMode, "GPIO_OFF", 8) == 0) {
		SET_BIT(gpio_registers_map[(port[1] - '1')].out, pin, 0);
		return 1;
	} else if (strncmp(pinMode, "GPIO_ON", 7) == 0) {
		SET_BIT(gpio_registers_map[(port[1] - '1')].out, pin, 1);
		return 1;
	} else if (strncmp(pinMode, "GPIO_TOGGLE", 11) == 0) {
		*gpio_registers_map[(port[1] - '1')].out ^= (0x01 << pin);
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
	char port[2];
	uint8_t pin = get_pin_number(pinNumber, port);
	return (*gpio_registers_map[(port[1] - '1')].in & (0x01 << pin)) >> pin;
}

uint8_t gpio_interrupt_enable(char *pinNumber, char *interruptMode, gpio_interrupt_callback *interruptCallBack) {
	char port[2];
	uint8_t pin = get_pin_number(pinNumber, port);
	switch (port[1] - '0') {
		case 1:
			P1_interrupt_callback[pin].bit = 1 << pin;
			P1_interrupt_callback[pin].callback = interruptCallBack;
			break;
		case 2:
			P2_interrupt_callback[pin].bit = 1 << pin;
			P2_interrupt_callback[pin].callback = interruptCallBack;
			break;
	}
	set_pin_mode(interruptMode, port, pin);
	return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) { 
	int i;
	for (i = 0; i < 8; i++) {
		if (P1IFG & P1_interrupt_callback[i].bit) {
			P1_interrupt_callback[i].callback();
			P1IFG &= ~P1_interrupt_callback[i].bit;
		}
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) { 
	int i;
	for (i = 0; i < 8; i++) {
		if (P2IFG & P2_interrupt_callback[i].bit) {
			P2_interrupt_callback[i].callback();
			P2IFG &= ~P2_interrupt_callback[i].bit;
		}
	}
}
