#ifndef _GPIO_H_
#define _GPIO_H_

typedef void gpio_interrupt_callback(void);

uint8_t gpio_init(char *pinNumber, char *pinMode);
uint8_t gpio_output(char *pinNumber, char *status);
uint8_t gpio_input(char *pinNumber);
uint8_t gpio_interrupt_enable(char *pinNumber, char *interruptMode, gpio_interrupt_callback *callback);

#endif
