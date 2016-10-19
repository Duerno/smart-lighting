#include <msp430.h>

#define RLED BIT0
#define GLED BIT6

#define AD_IN (BIT4+BIT5)
#define AD_CH INCH_5
unsigned int samples[2];

void setup_p1();
void setup_adc();
void update_leds();

int main() {
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	
	setup_adc();
	setup_p1();
	
	while(1) {
		ADC10SA = (unsigned int)&samples[0]; // ADC conversions start address
		ADC10CTL0 |= ADC10SC + ENC; // Start conversions
		_BIS_SR(LPM0_bits+GIE);
		update_leds();
	}
	
	return 0;
}

// ADC10 interruption routine
void ADCInt(void) __attribute__((interrupt(ADC10_VECTOR)));
void ADCInt(void) {
	ADC10CTL0 &= ~(ADC10SC+ENC);
	LPM0_EXIT;
}

// Update LEDs
void update_leds() {
	// Red LED
	if(samples[0] > MAX/2) {
		P1OUT |= RLED;
	} else {
		P1OUT &= ~RLED;
	}
	
	// Green LED
	if(samples[1] > MAX/2) {
		P1OUT |= GLED;
	} else {
		P1OUT &= ~GLED;
	}
}

// Initialize ADC10
void setup_adc() {	
	samples[0] = samples[1] = 0; // Initialize 'samples'
	ADC10CTL0 &= ~ENC;// Turn ADC10 off

	//          Vcc/GND  s&h=16clks   I.E.      ADC ON    Start next conversion automatically
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10IE + ADC10ON + MSC;
	//          INCH     /1           SMCLK         repeat-seq-of-conversion
	ADC10CTL1 = AD_CH +  ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_3;
	// Two conversions
	ADC10DTC1 = 0X04;
	// Inputs
	ADC10AE0 = AD_IN;
	// Turn ADC10 on
	ADC10CTL0 |= ENC;
}

