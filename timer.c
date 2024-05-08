#include <LPC21xx.H>
#include "timer.h"

#define COUNTER_ENABLE 1<<0
#define COUNTER_RESET	 1<<1
#define INTERRUPT_ON_MR0 1<<0
#define RESET_ON_MR0 1<<1
#define MR0_INTERRUPT 1<<0

void InitTimer0Match0(unsigned int uiDelayTime){
	T0MCR = RESET_ON_MR0 | INTERRUPT_ON_MR0;
	T0MR0=uiDelayTime*15;
	T0TCR |= COUNTER_RESET;
	T0TCR &= ~COUNTER_RESET;
	T0TCR |= COUNTER_ENABLE;
}

void WaitOnTimer0Match0(void){
	while((T0IR & MR0_INTERRUPT) == 0) {}
	T0IR = MR0_INTERRUPT;
}
