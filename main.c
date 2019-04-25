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

volatile uint8_t counter_2bit = 0;

void main(void)
{
	init(FREQ_1_5_MHZ);

	P4->SEL0 |= P4_3;
	P4->SEL1 &= ~P4_3;
	P4->DIR |= P4_3;

	// setup TIMER_A0
	TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt
	TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt

	TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
	TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; // TACCR1 interrupt enabled

	TIMER_A0->CCR[0] = 94;   // set CCR0 count
	TIMER_A0->CCR[1] = 47;   // set CCR1 count

	TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | // SMCLK,
									TIMER_A_CTL_MC_1;  // UP mode, count up to CCR[0]

	NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);   // set NVIC interrupt
	NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);   // TA0_0 and TA0_N

	__enable_irq();     // Enable global interrupt

	while(1){}
}

// Timer A0_0 interrupt service routine
void TA0_0_IRQHandler(void) {
	TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // Clear the CCR0 interrupt
	P2->OUT ^= RGB_RED;
	P1->OUT ^= LED1_PIN;
}

// Timer A0_N interrupt service routine for CCR1 - CCR4
void TA0_N_IRQHandler(void)
{
   if(TIMER_A0->CCTL[1]&TIMER_A_CCTLN_CCIFG)   // check for CCR1 interrupt
   {
			TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clear CCR1 interrupt
			P1->OUT ^= LED1_PIN;
   }
}
