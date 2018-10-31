/*
 * ece263_lab4.c
 * Lab 4: Digital Music Keyboard
 * Created: 4/1/2018 8:57:30 PM
 * Authors : Jacob Aubertine, Michael Benker
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
 
 
struct whichButton{
   	uint8_t column;
   	uint8_t row;
};
 
void Timer0Setup();
void Timer2Setup();
void ButtonPressed(struct whichButton *ptr);
void ButtonToTimer0(struct whichButton button);
void ButtonToTimer2(struct whichButton button);
 
int main(void)
{
   	/* I/O pin setup */
   	DDRC &= 0xF0; // sets bits 0-3 of Port C as inputs
   	DDRD |= 0x0F; // sets bits 0-3 of Port D as outputs
   	PORTC |= 0x0F;   	// enables pull-up resistors
   	DDRD |= (1 << PORTD6); 	// sets OC0A as output (PD6)
   	DDRB |= (1 << PORTB3); 	// sets OC2A as output (PB3)
   	
   	PORTD &= 0xF0;   	// sets bits 0-3 of Port D to 0 to detect button press
/* 	
   	// pin change interrupt control register, set PCIE1 to enable PCMSK1 scan
   	PCICR |= (1 << PCIE1);
   	PCMSK1 = 0x0F;         // sets PCINT8-11 to trigger an interrupt on state change
   	
   	//sei();               	// turn on interrupts globally
*/
   	struct whichButton button;
   	struct whichButton *ptrButton = &button;
   	
   	Timer0Setup();
   	Timer2Setup();
   	
	while (1)  
	{
          	ButtonPressed(ptrButton);
          	// PORTD &= 0xF0;	// sets bits 0-3 of Port D to 0 to detect button press
          	ButtonToTimer0(button);
          	ButtonToTimer2(button);
          	_delay_ms(20);
          	
          	if ( (button.column != 0) && (button.row != 0) ){
                 	TCCR0B |= (0b11 << CS00);     	// sets timer0 to %64 prescaler
                 	TCCR2B |= (0b11 << CS20);     	// sets timer2 to %32 prescaler
          	}
          	
          	else{
                 	TCCR0B &= ~(0b11 << CS00);    	// turns timer0 clock source off
                 	TCCR2B &= ~(0b11 << CS20);    	// turns timer2 clock source off
          	}
	}
}
 
void Timer0Setup(){
   	TCCR0A = (0b01 << COM0A0) | (0b10 << WGM00); // toggles on compare match, CTC mode
   	TCCR0B = (0 << WGM02) | (0b000 << CS00); // no prescaler, timer0 stopped initially
}
 
void Timer2Setup(){
   	TCCR2A = (0b01 << COM2A0) | (0b10 << WGM20);	// toggles on compare match, CTC mode
   	TCCR2B = (0 << WGM22) | (0b000 << CS20);    	// no prescaler, timer2 stopped initially
}
 
void ButtonPressed(struct whichButton *ptr){
   	PORTD &= 0xF0;   	// sets bits 0-3 of Port D to 0 to detect button press
   	if( (PINC & (1 << PINC3)) == 0 ){           	// column 1
          	ptr->column = 1;
          	PORTD = ~(1 << PORTD3);
          	if( (PINC & (1 << PINC3)) == 0 )            	// row 1
                 	ptr->row = 1;
          	PORTD = ~(1 << PORTD2);
          	if( (PINC & (1 << PINC3)) == 0 )            	// row 2
                 	ptr->row = 2;
          	PORTD = ~(1 << PORTD1);
          	if( (PINC & (1 << PINC3)) == 0 )            	// row 3
                 	ptr->row = 3;
          	PORTD = ~(1 << PORTD0);
          	if( (PINC & (1 << PINC3)) == 0 )            	// row 4
                 	ptr->row = 4;
   	}
   	else if( (PINC & (1 << PINC2)) == 0 ){      	// column 2
          	ptr->column = 2;
          	PORTD = ~(1 << PORTD3);
          	if( (PINC & (1 << PINC2)) == 0 )            	// row 1
                 	ptr->row = 1;
          	PORTD = ~(1 << PORTD2);
          	if( (PINC & (1 << PINC2)) == 0 )            	// row 2
                 	ptr->row = 2;
          	PORTD = ~(1 << PORTD1);
          	if( (PINC & (1 << PINC2)) == 0 )            	// row 3
                 	ptr->row = 3;
          	PORTD = ~(1 << PORTD0);
          	if( (PINC & (1 << PINC2)) == 0 )            	// row 4
                 	ptr->row = 4;
   	}
   	else if( (PINC & (1 << PINC1)) == 0 ){      	// column 3
          	ptr->column = 3;
          	PORTD = ~(1 << PORTD3);
          	if( (PINC & (1 << PINC1)) == 0 )            	// row 1
                 	ptr->row = 1;
          	PORTD = ~(1 << PORTD2);
          	if( (PINC & (1 << PINC1)) == 0 )            	// row 2
                 	ptr->row = 2;
          	PORTD = ~(1 << PORTD1);
          	if( (PINC & (1 << PINC1)) == 0 )            	// row 3
                 	ptr->row = 3;
          	PORTD = ~(1 << PORTD0);
          	if( (PINC & (1 << PINC1)) == 0 )            	// row 4
                 	ptr->row = 4;
   	}
   	else if( (PINC & (1 << PINC0)) == 0 ){      	// column 4
          	ptr->column = 4;
          	PORTD = ~(1 << PORTD3);
          	if( (PINC & (1 << PINC0)) == 0 )            	// row 1
                 	ptr->row = 1;
          	PORTD = ~(1 << PORTD2);
          	if( (PINC & (1 << PINC0)) == 0 )            	// row 2
                 	ptr->row = 2;
          	PORTD = ~(1 << PORTD1);
          	if( (PINC & (1 << PINC0)) == 0 )            	// row 3
                 	ptr->row = 3;
          	PORTD = ~(1 << PORTD0);
          	if( (PINC & (1 << PINC0)) == 0 )            	// row 4
                 	ptr->row = 4;
   	}
   	else{
          	ptr->column = 0;
          	ptr->row = 0;
   	}
}
 
void ButtonToTimer0(struct whichButton button){
   	switch(button.row) {
          	case 1 :
                 	OCR0A = 178;     	// 697 Hz
                 	break;
          	case 2 :
                 	OCR0A = 161;     	// 770 Hz
                 	break;
          	case 3 :
                 	OCR0A = 146;     	// 852 Hz
                 	break;
          	case 4 :
                 	OCR0A = 132;     	// 941 Hz
                 	break;
          	default :
                 	break;
   	}
   	
}
 
void ButtonToTimer2(struct whichButton button){
   	switch(button.column) {
          	case 1 :
                 	OCR2A = 206;     	// 1209 Hz
                 	break;
          	case 2 :
                 	OCR2A = 186;     	// 1336 Hz
                 	break;
          	case 3 :
                 	OCR2A = 168;     	// 1477 Hz
                 	break;
          	case 4 :
                 	OCR2A = 152;     	// 1633 Hz
                 	break;
          	default :
                 	break;
   	}
}
