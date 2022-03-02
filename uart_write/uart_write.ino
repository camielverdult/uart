#define F_CPU 16000000UL // Here the frequency of the micro controller is defined as unsigned long (the frequency of the ATmega328 is 16 MHz).

#define BAUD 9600

// Calculate baud ubrr value 
#define UBRR F_CPU/16/BAUD-1

#define MASK_MSB 0b11110000 // This is your bit mask this will be applied on the value of the ADCH register.
#define NIBBLE 4 // This is the value of a nibble. It is used to switch the bits to your left.

#include <avr/io.h> // Include the 'avr/io' header file, to use the facilities provided by this header file.
#include <avr/interrupt.h> // Include the 'avr/interrupt' header file, to use the facilities provided by this header file.

// These will be used in our interrupt for when usart tx/rx is ready
uint8_t uart_tx_buf = 0;

void USART_Init( unsigned int ubrr)
{
  /*Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  
  /* Enable transmitter and transmitter interrupt */
  UCSR0B = (1 << TXEN0) | (1 << TXCIE0);
  
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void ADC_Init(void) {
  cli(); // disable all interrupt

  DIDR0 |= (1 << ADC0D); // Here you have to disable the digital input of a specific pin to use it as an ADC. In this case, the digital input on 'ADC0D' is disabled.

  ADMUX |= (1 << REFS0) | (1 << ADLAR); // Here we configure ADC Multiplexer Selection register

  // ADCSRA – ADC Control and Status Register A
  // Bit 7 – ADEN: ADC Enable
  // Bit 5 – ADATE: ADC Auto Trigger Enable - When this bit is written to one, Auto Triggering of the ADC is enabled. The ADC will start a conversion on a 
  // positive edge of the selected trigger signal. The trigger source is selected by setting the ADC Trigger Select bits, ADTS in ADCSRB
  // Bit 4 – ADIF: ADC Interrupt Flag - The ADC Conversion Complete Interrupt is executed if the ADIE bit and the I-bit in SREG are set.
  // Bit 3 – ADIE: ADC Interrupt Enable - When this bit is written to one and the I-bit in SREG is set, the ADC Conversion Complete Interrupt is activated.
  // Bits 2:0 – ADPS[2:0]: ADC Prescaler Select Bits - These bits determine the division factor between the system clock frequency and the input clock to the ADC.
  // ADSP2, ADPS1 and ADPS0 set to 1 configures the division factor between the system clock frequency and the input clock to the ADC to be 128 
  ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
      
  ADCSRB |= (0 << ADPS2) | (0 << ADPS1) | (0 << ADPS0); // Here the ADC is in free running mode (see the corresponding table in the data sheet). You can also write 'ADCRSB = 0b0' 
  
  sei(); // Re-allow interrupts

  ADCSRA |= (1 << ADSC); // begin the first conversion from potentiometer to LEDs, it will keep running after thi
  
}

//ISR(USART_TX_vect) {
//  /* Send buffer (again) */
//  UDR0 = uart_tx_buf;
//}

// This is the function associated with the interrupt service routine when an ADC conversion is complete. The vector within this ISR is 'ADC_vect'.
// Within this function the statements are given that will write the MSB value of the ADCH register (after a small modification) to PORTB.
ISR(ADC_vect)
{
//    uart_tx_buf = ((MASK_MSB & ADCH) >> NIBBLE); // Read the ADCH register. In the left nibble contains your MSB values. Apply a bit-mask, and shift the result to the right. Then you can display the results on your LEDs.
  uart_tx_buf = ADCH;
}

void USART_Transmit(unsigned char data) {
  while ( ! ( UCSR0A & (1 << UDRE0) ) );

  UDR0 = data;
}

int main( void )
{
  // Initialise USART with our calculated value 
  USART_Init(UBRR);

  ADC_Init();

  // This is your while loop (infinite loop).
  while (1) 
  {    
      // Nothing in this while loop.
      USART_Transmit(uart_tx_buf);
  }

  return 0;  
}
