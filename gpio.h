#ifndef _GPIO_H_
#define _GPIO_H_

uint8_t gpio_init(char *pinNumber, char *pinMode);
uint8_t gpio_output(char *pinNumber, char *status);
uint8_t gpio_input(char *pinNumber);
uint8_t gpio_interrupt_enable(char *pinNumber, char *interruptMode);
uint8_t gpio_interrupt_check(char *pinNumber);

#endif
