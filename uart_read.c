/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/*
 * main.c
 *
 * Created: 2/27/2022 12:14:45 PM
 *  Author: Camiel Verdult
 */ 

#include <avr/io.h> // Include the 'avr/io' header file, to use the facilities provided by this header file.
#include <avr/interrupt.h> // Include the 'avr/interrupt' header file, to use the facilities provided by this header file.

#define F_CPU 16000000UL // Here the frequency of the micro controller is defined as unsigned long (the frequency of the ATmega328 is 16 MHz).

#define BAUD 9600

// Calculate baud ubrr value
#define UBRR F_CPU/16/BAUD-1 /* This will be around 130 */

void USART_Init(unsigned int ubrr)
{
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	// Enable receiver and receive interrupt
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
	
	// Set USART frame format: 8data, 2stop bits
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void PORTD_Init(void) {
	cli(); // disable all interrupt

	// Set the pins 'PB3', 'PB2', 'PB1' and 'PB0' as an output.
	// Pin 11, 10, 9, 8 on Arduino Uno
	DDRB |= (1 << PB3) | (1 << PB2) | (1 << PB1) | (1 << PB0);

	sei(); // Re-allow interrupts
}

// Write received byte to PORTD
ISR(USART_RX_vect) {
	PORTB = UDR0;
}

int main(void)
{
	// Initialize USART with our calculated value
	USART_Init(UBRR);

	// Initialize PORTD (PB0-3) for LED output
	PORTD_Init();
	
	while (1)
	{
		// Nothing in this while loop.
		// All the write data
	}

	return 0;
}
