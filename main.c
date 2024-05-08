#include <LPC21xx.H>
#include "timer.h"
#include "keyboard.h"
#include "interrupts.h"
#include "servo.h"


int main (){
	KeyboardInit();	
	ServoInit(100);
	
while(1){
		switch(eKeyboardRead()){
			case BUTTON_0:
				ServoCallib();
				break;			
			case BUTTON_1:
				ServoGoTo(12);
				break;
			case BUTTON_2:
				ServoGoTo(24);
				break;
			case BUTTON_3:
				ServoGoTo(36);
			break;
			case RELASED:
				break;
			default:{}
		}
	}
}
