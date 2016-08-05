#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include "usci.h"

#define SET_BIT(reg, shift, value) (*reg = (((*reg) & ~(1 << (shift))) | ((value) << (shift))))

typedef struct {
    uint16_t *sel;
    uint16_t *sel2;
} usci_registers;

usci_registers usci_registers_map[] = {
    { (uint16_t *) P1SEL_ , (uint16_t *) P1SEL2_ },
};

usci_interrupt_callback *uart_rx_callback;

char cmdbuf[20];
uint8_t cmd_index = 0;

/*****  uart  *****/
void uart_init(uint8_t mode) {
    P1SEL |= BIT1 | BIT2;
    P1SEL2 |= BIT1 | BIT2;

    UCA0CTL1 |= UCSSEL_2; // Use SMCLK = 8MHz

    //Set the baud rate
    switch (mode) {
        case 0://9600
            UCA0BR1 = 3;
            UCA0BR0 = 65;
            UCA0MCTL = UCBRS_2;
            break;
        case 1://19200
            UCA0BR1 = 1;
            UCA0BR0 = 160;
            UCA0MCTL = UCBRS_6;
            break;
        case 2://38400
            UCA0BR1 = 0;
            UCA0BR0 = 208;
            UCA0MCTL = UCBRS_3;
            break;
        case 3://56000
            UCA0BR1 = 0;
            UCA0BR0 = 142;
            UCA0MCTL = UCBRS_7;
            break;
        case 4://115200
            UCA0BR1 = 0;
            UCA0BR0 = 69;
            UCA0MCTL = UCBRS_4;
            break;
        case 5://128000
            UCA0BR1 = 0;
            UCA0BR0 = 62;
            UCA0MCTL = UCBRS_4;
            break;
        case 6://256000
            UCA0BR1 = 0;
            UCA0BR0 = 31;
            UCA0MCTL = UCBRS_2;
            break;
        default:
            UCA0BR1 = 3;
            UCA0BR0 = 65;
            UCA0MCTL = UCBRS_2;
            break;
    }
    
    UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
    IE2 |= UCA0RXIE; // enable RX interrupt
}

void uart_write_char(char c) {
    while (!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = c;
}

void uart_write_string(char *buf) {
    while (*buf) uart_write_char(*buf++);
}

/*****  spi  *****/
/* spi_a:chennal=0 spi_b:chennal=1, master:mode1=0 slave:mode1=1, 3-wire:mode2=0 4-wire:mode2=1 */
void spi_init(uint8_t chennal, uint8_t mode1, uint8_t mode2) {

}

void spi_write_char(char c, uint8_t chennal) {
    switch (chennal) {
        case 0:
            while (!(IFG2 & UCA0TXIFG));
            UCA0TXBUF = c;
            break;
        case 1:
            while (!(IFG2 & UCB0TXIFG));
            UCB0TXBUF = c;
            break;
    }
}

void spi_write_string(char *buf, uint8_t chennal) {
    while (*buf) uart_putc(*buf++, chennal);
}

void usci_init(char *type, char *mode, usci_interrupt_callback *receive_interrupt_callback) {
    if (strncmp(type, "UART", 4) == 0) {
        if (strncmp(mode, "9600", 4) == 0) uart_init(0);
        else if (strncmp(mode, "19200", 5) == 0) uart_init(1);
        else if (strncmp(mode, "38400", 5) == 0) uart_init(2);
        else if (strncmp(mode, "56000", 5) == 0) uart_init(3);
        else if (strncmp(mode, "115200", 6) == 0) uart_init(4);
        else if (strncmp(mode, "128000", 6) == 0) uart_init(5);
        else if (strncmp(mode, "256000", 6) == 0) uart_init(6);
        uart.writec = uart_write_char;
        uart.write = uart_write_string;
        uart_rx_callback = receive_interrupt_callback;
    } else if (strncmp(type, "SPI_A", 5) == 0) {
    } else if (strncmp(type, "SPI_B", 5) == 0) {
    } else if (strncmp(type, "I2C", 3) == 0) {
    }
}

/*****  usci interrupt  *****/
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void) {

}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    char rx_char;

    if (IFG2 & UCA0RXIFG) {
        rx_char = UCA0RXBUF;
        if (rx_char == '\r') {
            cmdbuf[cmd_index] = '\0';
            uart_rx_callback(cmdbuf);
            cmd_index = 0;
        } else {
            cmdbuf[cmd_index] = rx_char;
            cmd_index++;
        }
    }
}