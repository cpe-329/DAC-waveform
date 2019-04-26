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
#include "spi.h"

#define FREQ FREQ_24_MHZ


void main(void)
{
    // volitile int data = 2048;

	init(FREQ);
	led_on();
	delay_ms(500, FREQ);

	P1->SEL1 |= SPI_B0_CLK_PIN | 
				SPI_B0_MOSI_PIN | 
				SPI_B0_MISO_PIN; // SPI pins
	P1->SEL0 &= ~(P1_5 | P1_6 | P1_7);

	P3->SEL0 &= ~SPI_CS_PIN;
	P3->SEL1 &= ~SPI_CS_PIN;
	P3->DIR |= SPI_CS_PIN;

//	P2->DIR |= BIT0 | BIT1 | BIT2; // output to LEDS

	EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; //sets EUSCI state

	EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST |  // set reset
	                   EUSCI_B_CTLW0_MST |    // set as master
	                   EUSCI_B_CTLW0_SYNC |   // set as SPI
	                   EUSCI_B_CTLW0_CKPL |   // set CLK polarity high
	                   EUSCI_B_CTLW0_SSEL__SMCLK |  // use SMCLK
	                   EUSCI_B_CTLW0_MSB;     // set endianness
	// EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_MODE0 | // clear both mode bits
	//                      EUSCI_B_CTLW0_MODE1);  // set 3-pin mode
	EUSCI_B0 ->BRW = 0x01;

	//init state machine
	EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;
	EUSCI_B0->IE |= EUSCI_B_IE_RXIE;

	__enable_irq(); //Enable global interrupts

	NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);

	led_off();
	//polling loop
	while(1){


	     led_on();
         cs_low();
	    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0)){
	        led_on();
	        EUSCI_B0->TXBUF = dac_cmd(0x800);
	        // led_off();
	    }
	    cs_high();
	    delay_ms(1000, FREQ);
	    led_off();
        delay_ms(1000, FREQ);
	}
}

// Timer A0_0 interrupt service routine
void EUSCI_B0_IRQHandler(void) {
    rgb_set(RGB_RED);
    volatile uint8_t data;

	if(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0){
	    data = EUSCI_B0->RXBUF;
	    P2->OUT = data;
	}
}
