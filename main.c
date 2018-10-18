/*
 * ECE388_v1-0.c
 *
 * Created: 10/15/2018 7:56:02 PM
 * Author : Jacob Aubertine
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>



struct whichButton{
	uint8_t column;
	uint8_t row;
};

void ButtonSetup();
void ButtonPressed(struct whichButton *ptr);
void Timer0Setup();
void ButtonPressed(struct whichButton *ptr);
void ButtonToNum(struct whichButton *ptr, uint8_t *buttonnum);
void ADC_Pot2Intensity();

int main(void)
{
    ButtonSetup();
	struct whichButton button;
	struct whichButton *ptrButton = &button;
	
	// ADC setup
	// Make the pin for the selected ADC channel an input pin.	
	ADCSRA = (1<<ADEN)|(0b111<<ADPS0);	// enables ADC and sets prescaler to %128
	ADMUX = (0b00<<REFS0)|(1<<ADLAR)|(4<<MUX0);	// voltage reference is AREF, ADC data register left adjusted, ADC4 as input

	DDRB |= 6; //Set pin PB1 & PB2 as output for OC1A & OC1B
	// Timer 1 setup pwm
	TCCR1A = 0xA2; // Set OC1A & OC1B; WMG1[1]. Table 18-8 in Datasheet. & Table 18-9.
	TCCR1B = 0x18; // Set WMG1[2] & WMG1[3]; tABLE 18-9
	TCNT1 = 0; // clear counter
	ICR1 = 40000; // count of 40000 for a 20ms period or 50 Hz cycle
	OCR1A = 2900; // start in the middle between 900 & 4900
	OCR1B = 2900; //
	TCCR1B |= 2; //set clock to divide by 8 and start TABLE

	uint8_t adcvals;
	uint8_t row;
	uint8_t column;
	
	static uint16_t posit[2]={1500,5000}; //position of servo
	
    while (1) 
    {
		/*
		ADCSRA |= (1<<ADSC);	// starts conversion
		while( (ADCSRA&(1<<ADIF)) == 0);	// waits for conversion to complete
		adcvals = ADCH;		// copies ADCH register to adcvals variable
		*/
		
		/*
		PORTD &= 0;
		_delay_ms(500);
		PORTD |= 0x0F;
		_delay_ms(500);
		*/
		
		for (uint16_t ii = 0;ii < 2;ii++)
		{
			OCR1A = posit[ii];
			_delay_ms(2000);
		}
    }
}

void ButtonSetup(){
	DDRC &= 0b1110000;	// sets bits 0-4 of Port C as inputs (PC4 is ADC4)
	DDRD |= 0x0F;	// sets bits 0-3 of Port D as outputs
	PORTC |= 0x0F;	// enables pull-up resistors
	DDRD |= (1 << PORTD6);	// sets OC0A as output (PD6)
	
	PORTD &= 0xF0;	// sets bits 0-3 of Port D to 0 to detect button press	
}

/*
void Timer0Setup(){
	TCCR0A = (0b01 << COM0A0) | (0b10 << WGM00);	// toggles on compare match, CTC mode
	TCCR0B = (0 << WGM02) | (0b000 << CS00);		// no prescaler, timer0 stopped initially
	OCR0A = 70;
}
*/

void ButtonPressed(struct whichButton *ptr){
	PORTD &= 0xF0;	// sets bits 0-3 of Port D to 0 to detect button press
	if( (PINC & (1 << PINC3)) == 0 ){			// column 1
			ptr->column = 1;
		PORTD = ~(1 << PORTD3);
		if( (PINC & (1 << PINC3)) == 0 )			// row 1
			ptr->row = 1;
		PORTD = ~(1 << PORTD2);
		if( (PINC & (1 << PINC3)) == 0 )			// row 2
			ptr->row = 2;
		PORTD = ~(1 << PORTD1);
		if( (PINC & (1 << PINC3)) == 0 )			// row 3
			ptr->row = 3;
		PORTD = ~(1 << PORTD0);
		if( (PINC & (1 << PINC3)) == 0 )			// row 4
			ptr->row = 4;
	}
	else if( (PINC & (1 << PINC2)) == 0 ){		// column 2
			ptr->column = 2;
		PORTD = ~(1 << PORTD3);
		if( (PINC & (1 << PINC2)) == 0 )			// row 1
			ptr->row = 1;
		PORTD = ~(1 << PORTD2);
		if( (PINC & (1 << PINC2)) == 0 )			// row 2
			ptr->row = 2;
		PORTD = ~(1 << PORTD1);
		if( (PINC & (1 << PINC2)) == 0 )			// row 3
			ptr->row = 3;
		PORTD = ~(1 << PORTD0);
		if( (PINC & (1 << PINC2)) == 0 )			// row 4
			ptr->row = 4;
	}
	else if( (PINC & (1 << PINC1)) == 0 ){		// column 3
			ptr->column = 3;
		PORTD = ~(1 << PORTD3);
		if( (PINC & (1 << PINC1)) == 0 )			// row 1
			ptr->row = 1;
		PORTD = ~(1 << PORTD2);
		if( (PINC & (1 << PINC1)) == 0 )			// row 2
			ptr->row = 2;
		PORTD = ~(1 << PORTD1);
		if( (PINC & (1 << PINC1)) == 0 )			// row 3
			ptr->row = 3;
		PORTD = ~(1 << PORTD0);
		if( (PINC & (1 << PINC1)) == 0 )			// row 4
			ptr->row = 4;
	}
	else if( (PINC & (1 << PINC0)) == 0 ){		// column 4
			ptr->column = 4;
		PORTD = ~(1 << PORTD3);
		if( (PINC & (1 << PINC0)) == 0 )			// row 1
			ptr->row = 1;
		PORTD = ~(1 << PORTD2);
		if( (PINC & (1 << PINC0)) == 0 )			// row 2
			ptr->row = 2;
		PORTD = ~(1 << PORTD1);
		if( (PINC & (1 << PINC0)) == 0 )			// row 3
			ptr->row = 3;
		PORTD = ~(1 << PORTD0);
		if( (PINC & (1 << PINC0)) == 0 )			// row 4
			ptr->row = 4;
	}
	else{
		ptr->column = 0;
		ptr->row = 0;
	}
	
	_delay_ms(20);
}

void ButtonToNum(struct whichButton *ptr, uint8_t *buttonnum){
	if ( ptr->column == 2 && ptr->row == 4 )
		*buttonnum = 0;
	else if ( ptr->column == 1 && ptr->row == 3 )
		*buttonnum = 1;
	else if ( ptr->column == 2 && ptr->row == 3 )
		*buttonnum = 2;
	else if ( ptr->column == 3 && ptr->row == 3 )
		*buttonnum = 3;
	else if ( ptr->column == 1 && ptr->row == 2 )
		*buttonnum = 4;
	else if ( ptr->column == 2 && ptr->row == 2 )
		*buttonnum = 5;
	else if ( ptr->column == 3 && ptr->row == 2 )
		*buttonnum = 6;
	else if ( ptr->column == 1 && ptr->row == 1 )
		*buttonnum = 7;
	else if ( ptr->column == 2 && ptr->row == 1 )
		*buttonnum = 8;
	else if ( ptr->column == 3 && ptr->row == 1 )
		*buttonnum = 9;
}

void ADC_Pot2Intensity(){
	ADCSRA |= (1<<ADSC);	// starts conversion
	while( (ADCSRA&(1<<ADIF)) == 0);	// waits for conversion to complete
	MAX7221_Intensity(ADCH>>4);		// looks at 4 highest bits for intensity
}

void Door(){
	  
}