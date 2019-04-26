///*
// * main.c
// *
// * Danica Fujiwara & Spencer Shaw
// *
// * CPE 329-17/18 Spring 2019
// *
// *  MCLK = = DCO ~24MHz
// *  SMCLK = MCLK / 8
// *  TIMER_A0 utilize SMCLK 3MHz
// *  TIMER_A0 CCR0 and CCR1 to create 2 timing events
// */
//
//#include <stdint.h>
//#include "msp.h"
//
#include "my_msp.h"
#include "delay.h"
#include "led.h"
#include "spi.h"
//
#define FREQ FREQ_24_MHZ
//
//
//void main(void)
//{
//    // volitile int data = 2048;
//
//	led_on();
//	delay_ms(500, FREQ);
//
//	P1->SEL1 |= SPI_B0_CLK_PIN |
//				SPI_B0_MOSI_PIN |
//				SPI_B0_MISO_PIN; // SPI pins
//	P1->SEL0 &= ~(P1_5 | P1_6 | P1_7);
//
//	P3->SEL0 &= ~SPI_CS_PIN;
//	P3->SEL1 &= ~SPI_CS_PIN;
//	P3->DIR |= SPI_CS_PIN;
//
////	P2->DIR |= BIT0 | BIT1 | BIT2; // output to LEDS
//
//	EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; //sets EUSCI state
//
//	EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Put eUSCI state machine in reset
//
//    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST  | // keep eUSCI in reset
//                      EUSCI_B_CTLW0_MST    | // Set as SPI master
//                      EUSCI_B_CTLW0_SYNC   | // Set as synchronous mode
//                      EUSCI_B_CTLW0_CKPL   | // Set clock polarity high
//                      EUSCI_B_CTLW0_SSEL__SMCLK | // SMCLK
//                      EUSCI_B_CTLW0_MSB;     // MSB first
//
//	EUSCI_B0->BRW = 0x01;               // no div - fBitClock = fBRCLK/(UCBRx)
//
//    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;  // Initialize USCI state machine
//    EUSCI_B0->IE |= EUSCI_B_IE_RXIE;          // Enable RX interrupt
//
//    // Enable global interrupt
//    __enable_irq();
//
//    // Enable eUSCI_B0 interrupt in NVIC module
//    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);
//
//
//	led_off();
//	//polling loop
//	while(1){
//
//	    led_on();
//	    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0)){}
//		led_off();
//		EUSCI_B0->TXBUF = dac_cmd(0x800);
//		// led_off();
//
//	    // cs_high();
//	    delay_ms(500, FREQ);
//	    led_off();
//        delay_ms(500, FREQ);
//	}
//}
//
//// Timer A0_0 interrupt service routine
//void EUSCI_B0_IRQHandler(void) {
//    rgb_set(RGB_RED);
//    volatile uint8_t data;
//
//	if(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0){
//	    data = EUSCI_B0->RXBUF;
//	    P2->OUT = data;
//	}
//}
/*
 *  SPI Example using eUSCI_B0 for SPI
 *  P1.5  UCB0CLK   SCLK
 *  P1.6  UCB0SIMO  MOSI
 *  P1.7  UCB0SOMI  MISO
 *
 *  write numbers 1-10 to SPI using polling the IFG flag to wait until the
 *  TXBUF is clear before writing the next number
 *  Interrupts are enabled on receiving from SPI. The ISR will read
 *  the RXBUF and set the value to multicolor LED (P2.0-2)
 */

#include "msp.h"
#include <stdint.h>

int main(void)
{
    volatile uint16_t data;
    volatile uint32_t i;


    init(FREQ);
    P1->SEL0 |= BIT5 | BIT6 | BIT7;     // Set P1.5, P1.6, and P1.7 as
                                        // SPI pins functionality

    P2->DIR |= BIT0 | BIT1 | BIT2;      // set as output for LED

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Put eUSCI state machine in reset

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST  | // keep eUSCI in reset
                      EUSCI_B_CTLW0_MST    | // Set as SPI master
                      EUSCI_B_CTLW0_CKPH |
                      EUSCI_B_CTLW0_SYNC   | // Set as synchronous mode
//                      EUSCI_B_CTLW0_CKPL   | // Set clock polarity high
                      EUSCI_B_CTLW0_SSEL__SMCLK | // SMCLK
                      EUSCI_B_CTLW0_MSB;     // MSB first

    EUSCI_B0->BRW = 0x01;               // no div - fBitClock = fBRCLK/(UCBRx)

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;  // Initialize USCI state machine
    EUSCI_B0->IE |= EUSCI_B_IE_RXIE;          // Enable RX interrupt

    // Enable global interrupt
    __enable_irq();

    // Enable eUSCI_B0 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);

    while(1)
    {
        #define MAX 1300
        // write numbers 0-7 to SPI. Use TXIFG to verify TXBUF is empty
        for(data = 0; data < MAX; data += 2){
            dac_set(data);
            for (i = 0; i< 2; i++){}
        }
        for(data = 0; data < MAX; data += 2){
            dac_set(MAX - data);
            for (i = 0; i< 2; i++){}
        }
    }
}

// SPI interrupt service routine
// Read from SPI and set P2 multicolor LED

void EUSCIB0_IRQHandler(void)
{
    volatile uint8_t RXData;

    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG)  // verify RX interrupt
    {
        RXData = EUSCI_B0->RXBUF;

        P2->OUT &= ~(BIT0 | BIT1 | BIT2); // reset to 0
        P2->OUT |= (RXData & 0b0111);                // set data to LEDs
    }
}

