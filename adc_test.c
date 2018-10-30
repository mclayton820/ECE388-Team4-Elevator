/*
 * ECE388_v1-0.c
 *
 * Created: 10/15/2018 7:56:02 PM
 * Author : Jacob Aubertine
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>




void ADC_Pot2Intensity();

int main(void)
{
	// ADC setup
	DDRC &= 11101111;  // Make PORTC4 (ADC4) for the selected ADC channel an input pin.	
	DDRB = 0xFF;
	DDRD = 0xFF;
	ADCSRA = (1<<ADEN)|(0b111<<ADPS0);	// enables ADC and sets prescaler to %128
	ADMUX = (0b00<<REFS0)|(0<<ADLAR)|(0b0100<<MUX0);	// voltage reference is AREF, ADC data register right adjusted, ADC4 as input
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);	// starts conversion
		while( (ADCSRA&(1<<ADIF)) == 0);	// waits for conversion to complete
		PORTD = ADCL;
		PORTB = ADCH;		// copies ADCH register to variable
	}
}

void ADC_Pot2Intensity(){
	ADCSRA |= (1<<ADSC);	// starts conversion
	while( (ADCSRA&(1<<ADIF)) == 0);	// waits for conversion to complete
	//MAX7221_Intensity(ADCH>>4);		// looks at 4 highest bits for intensity
}