// File Name: main.c 
// Author:	Daniel Dienemann
// Date:		05.03.2021

//=========================================================
//Includes
//=========================================================
#include "mcc_generated_files/mcc.h"
#include "zyklus.h"

//=========================================================
//Defines
//=========================================================
//Define Ports
#define LED PORTA
#define BUTTON PORTC

//TimerNames for blink
#define getBlink1 get_Zyklus() 		//Used for blink 1

#define resetBlink1 reset_Zyklus() 	//Used for blink 1


//Standard
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0
#define LEFT 1
#define RIGHT 0

//SwitchCase States
#define CLOSED 1
#define OPENED 2
#define PROGRAMMING_CODE 3
#define ALARM 4

//Blinking defines
#define alarmBlink blink1(200,200) // in ms, 2.5Hz, 50:50

//=========================================================
//Global Variables
//=========================================================
//Standard
unsigned char copyOutput = 0;
unsigned char copyInput = 0;

//Input Variables
unsigned char switchProgrammingCodeNegEdge = 0;
unsigned char switchProgrammingCodePosEdge = 0;
unsigned char switchReadCodeNegEdge = 0;
unsigned char switchReadCodePosEdge = 0;
unsigned char switchCodeInput = 0;

//Output Variables
unsigned char ledProgramming = 0;
unsigned char ledAlarm = 0;
unsigned char ledOpened = 0;

//Project specific
unsigned char state = 0;
unsigned char currentCode = 0;
unsigned char newCode = 0;
unsigned char falseCounter = 0;

//=========================================================
//Prototypes
//=========================================================
void readInput();
void process();
void writeOutput();

unsigned char blink1(unsigned int tOn, unsigned int tOff);

//=========================================================
//Name: initializing
//Function: Initialise the MicroController and System.
//Return Value: None
//=========================================================

void initializing()
{
    SYSTEM_Initialize(); //Initialise uC
    ZYKLUS_Initialize(); //Initialise Timer

    //Call function blink, reset_Zyklus & get_Zyklus to avoid warnings if they aren't used.
    //These functions are used for timing and blinking and work as 3 different, individual timers reps. blinkers.
    get_Zyklus();
    get_Zyklus1();
    get_Zyklus2();
    reset_Zyklus();
    reset_Zyklus1();
    reset_Zyklus2();
    blink1(0, 0);

    //Project specific
    state = CLOSED;
    currentCode = 0b0000;
}

//=========================================================
//Name: main
//Function: main function
//Return Value: None
//=========================================================

void main()
{
    initializing();
    while (1)
    {
	readInput();
	process();
	writeOutput();
    }
}

//=========================================================
//Name: readInput
//Function:Read Input from Switches (P2)
//Return Value: None
//=========================================================

void readInput()
{
    static unsigned char oldInput = 0; //"static" to make sure variable only gets defined once
    static unsigned char posEdge = 0;
    static unsigned char negEdge = 0;

    copyInput = BUTTON; //Create Copy of Buttons/Switches

    //Positive Edge
    posEdge = (~oldInput) & copyInput; //exclude unnecessary pressed buttons
    switchReadCodePosEdge = (posEdge & 0x20) >> 5; //button S5  PosEdge
    switchProgrammingCodePosEdge = (posEdge & 0x80) >> 7; //button S7  PosEdge

    //Negative Edge
    negEdge = oldInput & (~copyInput); //exclude unnecessary pressed buttons
    switchReadCodeNegEdge = (negEdge & 0x20) >> 5; //button S5 NegEdge
    switchProgrammingCodeNegEdge = (negEdge & 0x80) >> 7; //button S7 NegEdge

    oldInput = copyInput;

    switchCodeInput = copyInput & 0b1111; //masking the Input to prevent 

}

//=========================================================
//Name: process
//Function: Process the main Code
//Return Value: None
//=========================================================

void process()
{

    ledOpened = OFF;
    ledProgramming = OFF;

    switch (state)
    {
	case CLOSED: //lock is closed
	    if (switchReadCodePosEdge == TRUE)
	    {
		if (switchCodeInput == currentCode)
		{
		    state = OPENED;
		    falseCounter = 0;
		}
		else
		{
		    falseCounter++;
		}
	    }

	    if (falseCounter >= 3)
	    {
		state = ALARM;
	    }
	break;

	case OPENED: //lock is opened
	    ledOpened = ON;

	    if (switchReadCodeNegEdge == TRUE)
	    {
		state = CLOSED;
	    }
	    else if (switchProgrammingCodePosEdge == TRUE)
	    {
		state = PROGRAMMING_CODE;
	    }
	    break;

	case PROGRAMMING_CODE: //programming a new code
	    ledProgramming = ON;
	    ledOpened = ON;

	    if (switchReadCodePosEdge == TRUE)
	    {
		newCode = switchCodeInput;
	    }

	    if (switchProgrammingCodeNegEdge == TRUE && newCode != currentCode)
	    {
		currentCode = newCode;
		state = CLOSED;
	    }
	    break;

	case ALARM: //the Alarm is triggered because the Code was typed in wrong 3 times
	    ledAlarm = alarmBlink;
	    break;

	default:
	    //only for debug
	    break;
    }

}

//=========================================================
//Name: writeOutput
//Function: Write Output to LED (P1)
//Return Value: None
//=========================================================

void writeOutput()
{
    //Project specific

    //Standard
    copyOutput = ledOpened; //LED 0
    copyOutput |= (ledAlarm << 1) & 0x02; //LED 1
    copyOutput |= (ledProgramming << 7) & 0x80; //LED 7

    LED = copyOutput; //Write Copy to the LEDs
}

//=========================================================
//Name: functions
//Function: process the functions
//Return Value: depending on the function
//=========================================================

unsigned char blink1(unsigned int tOn, unsigned int tOff)
{
    unsigned int status = 0;

    if (getBlink1 <= tOn)
    {
	status = ON;
    }

    if (getBlink1 >= tOn)
    {
	status = OFF;
    }

    if (getBlink1 >= tOn + tOff)
    {
	resetBlink1;
    }
    return status;
}