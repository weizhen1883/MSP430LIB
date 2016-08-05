#ifndef _USCI_H_
#define _USCI_H_

typedef void usci_interrupt_callback(char *buf);
typedef void write_char(char c);
typedef void write_string(char *buf);

typedef struct {
	write_char *writec;
	write_string *write;
} usci;

usci uart, spi_a, spi_b;

void usci_init(char *type, char *mode, usci_interrupt_callback *receive_interrupt_callback);

#endif