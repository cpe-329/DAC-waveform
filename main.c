/*
 * main.c
 * 
 * Danica Fujiwara & Spencer Shaw
 * 
 * CPE 329-17/18 Spring 2019
 *  
 *  MCLK = = DCO ~24MHz
 *  SMCLK = MCLK / 8   
 *  TIMER_A0 utilize SMCLK 3MHz
 *  TIMER_A0 CCR0 and CCR1 to create 2 timing events
 */

#include <stdint.h>
#include "msp.h"
#include "my_msp.h"
#include "delay.h"
#include "led.h"

#define FREQ FREQ_24_MHZ

void main(void)
{
    uint8_t data;

	init(FREQ_1_5_MHZ);

	P1->SEL0 |= BIT5 | BIT6 |BIT7; // SPI pins
	P2->DIR |= BIT0 | BIT1 | BIT2; // output to LEDS

	EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; //sets EUSCI state

	EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST |  // set reset
	                   EUSCI_B_CTLW0_MST |    // set as master
	                   EUSCI_B_CTLW0_SYNC |   // set as SPI
	                   EUSCI_B_CTLW0_CKPL |   // set CLK polarity high
	                   EUSCI_B_CTLW0_UCSSEL_2 |  // use SMCLK
	                   EUSCI_B_CTLW0_MSB;     // set endianness
	EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_MODE0 | // clear both mode bits
	                     EUSCI_B_CTLW0_MODE1);  // set 3-pin mode
	EUSCI_B0 ->BRW = 0x01;

	//init state machine
	EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;
	EUSCI_B0->IE |= EUSCI_B_IE_RXIE;

	__enable_irq(); //Enable global interrupt

	NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);
	//polling loop
	while(1){
	    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)){
	        EUSCI_B0->TXBUF = data;
	    }
	    delay_ms(2000, FREQ);
	}
}

// Timer A0_0 interrupt service routine
void EUSCI_B0_IRQHandler(void) {
    volatile uint8_t data;
	if(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0){
	    data = EUSCI_B_RXBUF_RXBUF_MASK ;
	    P2->OUT = data;
	}
}
