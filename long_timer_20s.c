// /*
//  * main.c
//  * 
//  * Danica Fujiwara & Spencer Shaw
//  * 
//  * CPE 329-17/18 Spring 2019
//  *  
//  *  MCLK = = DCO ~24MHz
//  *  SMCLK = MCLK / 8   
//  *  TIMER_A0 utilize SMCLK 3MHz
//  *  TIMER_A0 CCR0 and CCR1 to create 2 timing events
//  */

// #include <stdint.h>
// #include "msp.h"
// #include "my_msp.h"
// #include "delay.h"
// #include "led.h"

// volatile uint8_t counter_100ms = 0;

// void main(void)
// {
//     init(FREQ_1_5_MHZ);
// 	P4->SEL0 |= P4_3;
// 	P4->SEL1 &= ~P4_3;
// 	P4->DIR |= P4_3;

// 	// setup TIMER_A0
//     TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt
//     // TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG; // clear interrupt

//     TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
//     // TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE; // TACCR1 interrupt enabled

//     TIMER_A0->CCR[0] = 18880;   // set CCR0 count
//     // TIMER_A0->CCR[1] = 8;   // set CCR1 count

//     TIMER_A0->CTL = TIMER_A_CTL_TASSEL_2 | // SMCLK,
//                     TIMER_A_CTL_MC_1;  // UP mode, count up to CCR[0]

//     NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);   // set NVIC interrupt
//     NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);   // TA0_0 and TA0_N

//     __enable_irq();     // Enable global interrupt

//     while(1){
// 		if (counter_100ms > 200){
// 			counter_100ms = 0;
// 		} else if (counter_100ms < 100){
// 			led_on();
// 		} else{
// 			led_off();
// 		}
//     }
// }

// // Timer A0_0 interrupt service routine
// void TA0_0_IRQHandler(void) {
    
// 	P2->OUT |= BIT0;
	
// 	TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // Clear the CCR0 interrupt
	
// 	counter_100ms++;
	

// 	P2->OUT &= ~BIT0;
// }
