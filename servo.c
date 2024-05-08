#include "servo.h"
#include <LPC21xx.H>
#include "led.h"
#include "interrupts.h"

#define DETECTOR_bm (1<<10)

enum ServoState {CALLIB, IDLE, IN_PROGRESS};

struct Servo{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
}; struct Servo sServo;

enum DetectorState {ACTIVE, INACTIVE};
void DetectorInit(void){

	IO0DIR=IO0DIR &(~DETECTOR_bm);
}

enum DetectorState eReadDetector (void){

	if((IO0PIN&DETECTOR_bm) == 0){
		return ACTIVE;
	}else{
		return INACTIVE;
	}
}

void ServoCallib(void){
	sServo.eState = CALLIB;
	while(eReadDetector()==INACTIVE);
}

void ServoGoTo(unsigned int uiPosition){
	sServo.eState = IN_PROGRESS;
	sServo.uiDesiredPosition = uiPosition;
}

void ServoAutomat(){
	switch(sServo.eState){
		case IDLE:
			if(sServo.uiCurrentPosition != sServo.uiDesiredPosition){
				sServo.eState = IN_PROGRESS;}
			else{
				sServo.eState = IDLE;}
			break;
		case IN_PROGRESS: 
			if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){
				LedStepLeft();
				sServo.eState = IN_PROGRESS;
				sServo.uiCurrentPosition--;
			}else if(sServo.uiCurrentPosition < sServo.uiDesiredPosition){
				sServo.eState = IN_PROGRESS;
				LedStepRight();
				sServo.uiCurrentPosition++;
			}else{
				sServo.eState = IDLE;
			}
			break;
		case CALLIB: 
			if(eReadDetector()==INACTIVE){
				LedStepRight();
			}else {
				sServo.eState = IDLE;
				sServo.uiCurrentPosition = 0;
				sServo.uiDesiredPosition = 0;
			}
			break;
		}
	}

void ServoInit(unsigned int uiServoFrequency){

	Timer0Interrupts_Init(1000000/uiServoFrequency, &ServoAutomat);
	LedInit();
	DetectorInit();
	ServoCallib();
}
