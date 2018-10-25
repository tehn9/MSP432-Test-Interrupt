#include "msp.h"
#include <stdint.h>

volatile int deb_cnt;
volatile int deb_cnt_but;
int bits[4] = {BIT0, BIT1, BIT2, BIT3};
volatile int incr = -1;



void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P1SEL0 = 0x00;
	P1SEL1 = 0x00;

	P4OUT = 0x00;

	P1REN |=  BIT6; // set P1.1 resistor
	P1OUT |=  BIT6;
    P1REN |= BIT7; // set P1.1 resistor
    P1OUT |= BIT7;
    P1REN |=  BIT5; // set P1.1 resistor
    P1OUT |=  BIT5;
	P4DIR |= 1 << 0; // set P4.* output
	P4DIR |= 1 << 1;
	P4DIR |= 1 << 2;
	P4DIR |= 1 << 3;
	int var = 1 * SystemCoreClock;

//	P1IES |= 1 << 6;
	P1IFG  = 0x00;
	P1IE |= BIT6;
	P1IE |= BIT7;
	P1IE |= BIT5;
	NVIC->ISER[1] = 1 << (PORT1_IRQn & 31);
	 __enable_interrupt();

	while(1){
	}
}
void PORT1_IRQHandler ( void )
{
    if ( (P1IFG == BIT6 || P1IFG == BIT7) &&
         (P1IN & BIT6) && (P1IN & BIT7)) {
        P1IFG = 0x00;
        return;
    }
    switch (P1IFG)
    {
    case BIT6:
        if ( P1IN & BIT7) {
            ++incr;
        P4OUT |= 1 << incr;
        }
        break;
    case BIT7:
        if ( P1IN & BIT6) {
        P4OUT &= ~(1 << incr);
        --incr;
        }
        break;
    case BIT5:
        incr = -1;
        P4OUT = 0x00;
//        for(deb_cnt = 0; deb_cnt < 12000; deb_cnt++ );
        break;
    default:
        break;
    }
//    P4OUT = 0;

    for(deb_cnt = 0; deb_cnt < 1000; deb_cnt++ );
    P1IFG = 0x00;
}
