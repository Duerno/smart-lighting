#include <msp430.h>

#define RLED BIT0
#define GLED BIT6

#define PWM_OUT GLED
#define CYCLE 0xF // PWM freq = SMCLK / CYCLE

#define AD_IN BIT5
#define AD_CH INCH_5
#define MAX 1023 // ADC10 max value
unsigned int samples[1];

void setup_p1();
void setup_adc();
void setup_timer();

int main() {
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	
	setup_adc();
	setup_p1();
	setup_timer();
	
	while(1) {
		ADC10SA = (unsigned int)&samples[0]; // ADC conversions start address
		ADC10CTL0 |= ADC10SC + ENC; // Start conversions
		_BIS_SR(LPM0_bits+GIE);
	}
	
	return 0;
}

// ADC10 interruption routine
void ADCInt(void) __attribute__((interrupt(ADC10_VECTOR)));
void ADCInt(void) {
	ADC10CTL0 &= ~(ADC10SC+ENC);
	TACCR1 = CYCLE * samples[0] / MAX;
	LPM0_EXIT;
}

// Initialize ADC10
void setup_adc() {	
	samples[0] = 0; // Initialize 'samples'
	ADC10CTL0 &= ~ENC;// Turn ADC10 off

	//          Vcc/GND  s&h=16clks   I.E.      ADC ON    Start next conversion automatically
	ADC10CTL0 = SREF_0 + ADC10SHT_2 + ADC10IE + ADC10ON + MSC;
	//          INCH     /1           SMCLK         repeat-seq-of-conversion
	ADC10CTL1 = AD_CH +  ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_3;
	// Two conversions
	ADC10DTC1 = 0x01;
	// Inputs
	ADC10AE0 = AD_IN;
	// Turn ADC10 on
	ADC10CTL0 |= ENC;
}

// Initialize P1
void setup_p1() {
	P1SEL = GLED;
	P1DIR = GLED + RLED;
	P1OUT &= ~(GLED + RLED);
}

// Initialize TimerA
void setup_timer() {
	// Turn TimerA off
	TACTL = MC_0;
	// Period
	TACCR0 = CYCLE-1;
	// Duty cycle
	TACCR1 = CYCLE/4;
	// Output mode 7: reset/set
	TACCTL1 = OUTMOD_7;
	//      SMCLK      /1     upmode
	TACTL = TASSEL_2 + ID_0 + MC_1;
}

