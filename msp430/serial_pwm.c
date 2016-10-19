#include <msp430.h>

#define RLED BIT0
#define GLED BIT6

#define RXPIN BIT1
#define TXPIN BIT2
#define MAX 255

#define PWM_OUT GLED
#define CYCLE 0xFF // PWM freq = SMCLK / CYCLE

void setup_uart();
void setup_timer();
void update_duty_cycle(char);

int main() {
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	
	setup_uart();
	setup_timer();
	
	while(1) {
		IE2 |= UCA0RXIE; // Enable interruption via UART RX
		_BIS_SR(LPM0_bits+GIE);
	}
	
	return 0;
}

// UART RX interruption controller
void USCI0RXInt(void) __attribute__((interrupt(USCIAB0RX_VECTOR)));
void USCI0RXInt(void) {
	IE2 &= ~UCA0RXIE; // Disable interruption via UART RX
	update_duty_cycle(UCA0RXBUF);
	LPM0_EXIT;
}

// Update output PWM duty cycle
void update_duty_cycle(char value) {
	if(value >= 'a' && value <= 'z')
		TACCR1 = CYCLE * (value - 'a') / 25;
}

// Initialize UART
void setup_uart() {
	// Setup pins
	P1SEL |= RXPIN + TXPIN;
	P1SEL2 |= RXPIN + TXPIN;
	P1DIR |= RXPIN + TXPIN;

	// LSB first - 8-bit data - no parity - 1 stop bit - UART - Async
	UCA0CTL0 = 0;	
	// SMCLK
	UCA0CTL1 = UCSSEL_2;
	// baud rate 9600 -> 16 MHz
	UCA0BR0 = 103;
	UCA0BR1 = 6;
	// First modulation stage select
	UCA0MCTL = UCBRS2+UCBRS1;
	// Disable interruption via UART RX
	IE2 &= ~UCA0RXIE;
}

// Initialize TimerA
void setup_timer() {
	// Setup output LED
	P1SEL |= PWM_OUT;
	P1DIR |= PWM_OUT;
	P1OUT &= ~PWM_OUT;
	
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

