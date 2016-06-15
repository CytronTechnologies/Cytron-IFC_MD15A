//=====================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller-Brush Motor Card (IFC-MD15A)
//	Project description	: Sample1 for IFC-MD15A
//  Date				: 21 April 2008
//
//=====================================================================================

//=====================================================================================
//	include h file for cards
//	User has to include h file for all the cards used 
//=====================================================================================
#include<p18f2685.h>
#include "iic.h"
#include "iic_md.h"


//=====================================================================================
//	configuration bit, User is advised not to change the setting here
//=====================================================================================
#pragma config OSC = HSPLL
#pragma config FCMEN = OFF 
#pragma config IESO = OFF 
#pragma config PWRT = OFF 
#pragma config BOREN = OFF 
#pragma config WDT = OFF 
#pragma config MCLRE = ON
#pragma config PBADEN = OFF 
#pragma config DEBUG = OFF
#pragma config XINST = OFF
#pragma config LVP = OFF 

//=====================================================================================
//	define input or ouput of board here
//	For example: 
//				1.led1 was defined for first LED on MB00
//				2.buzzer was defined for buzzer on MB00
//	The address for input/output devide on board is fixed, hence user is 
//	advised not to change the defined address for input/output 
//=====================================================================================

#define	led1		LATBbits.LATB0 	// led1 defined as output for first LED on MB00	
#define	led2		LATBbits.LATB1	// led2 defined as output for second LED on MB00
#define	led3		LATBbits.LATB2	// led3 defined as output for third LED on MB00
#define	led4		LATBbits.LATB3	// led4 defined as output for forth LED on MB00
#define	led5		LATBbits.LATB4	// led5 defined as output for fifth LED on MB00
#define	led6		LATBbits.LATB5	// led6 defined as output for sixth LED on MB00
#define	led7		LATBbits.LATB6	// led7 defined as output for seventh LED on MB00
#define	led8		LATBbits.LATB7	// led8 defined as output for eighth LED on MB00
#define	led			LATB			// led defined as output for ALL LED on MB00

#define	sw1			PORTAbits.RA2	// sw1 defined as input for SW1 on MB00
#define	sw2			PORTAbits.RA1	// sw2 defined as input for SW2 on MB00
#define	sw3			PORTAbits.RA0	// sw3 defined as input for SW3 on MB00
#define	buzzer		LATCbits.LATC5	// buzzer defined as output for buzzer on MB00
#define busy_led	LATCbits.LATC1	// busy_led defined as output for Busy LED on MB00
#define error_led	LATCbits.LATC2	// error_led defined as output for Error LED on MB00
#define slave_reset	LATCbits.LATC0	// slave_reset define as output to reset slave cards

//=====================================================================================
//	card address
//	User can define the address of slave card here
//=====================================================================================
#define add_md1 		0b000011	// Please make sure the communication address on 
									// IFC-MD15A is compatible with the address 

//=====================================================================================
//	function prototype
//	User needs to include all the function prototype here
//	The first 4 functions prototype are fixed, user is also advised not to
//	change it.
//=====================================================================================
void ISRHigh(void);
void ISRLow(void);
void delay(unsigned long data);
void init(void);
	
//=====================================================================================
//	Main Function
//	This is the main function where program start to execute 
//=====================================================================================
void main(void)
{
	//--------------------------------------------------------------------------------
	//	Program start
	//	User can start to write/modify the program here
	//--------------------------------------------------------------------------------

	unsigned short i,temp1,speed;	// define 16-bits local variable

	//	User is advised not to change or remove the initialization function called 
	init();						// call initialization function of the setting in program

	while(1)
	{

	
		//--------------------------------------------------------------------------------
		// This program is mainly for slave card,DI08. It will test the 3 push buttons
		// on MB00, the push buttons are active low, hence, when any of the push button 
		// is pressed, value '0' will be sent to microcontroller on master card, MB00.
		// Hence, the brush motor will run base on program
		//--------------------------------------------------------------------------------

	
		if(sw1==0)							// Test whether SW1 on IFC-MB00 is pressed
		{
			led1 = 1;						// turn ON LED1 on MB00 by sending value '1'
			buzzer = 1;						// turn ON buzzer on MB00 by sending value '1'
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// turn OFF buzzer on MB00 by sending value '0'
			i=0;
			while(1)
			{
				// this is an infinity loop after SW1 on IFC-MB00 is pressed, 
				// user need to press reset button on MB00 to exit from the mode		
				// Brush Motor is activated. 

				md_cw(add_md1);				// Run motor in clockwise

				md_speed(add_md1,250);		// motor's speed = 250
				led4=md_runstat(add_md1);	// turn ON led4 when motor run
				for(i=0;i<10;i++)			// delay for motor running time
				{
					delay(50000);
				}
				md_stop(add_md1);			// stop motor
				led4=md_runstat(add_md1);	// turn OFF led4 when motor stop
				for(i=0;i<5;i++)			// delay for motor stop time
				{
					delay(50000);
				}
				md_ccw(add_md1);			// Run motor in counter-clockwise
				md_speed(add_md1,150);		// motor's speed = 150
				led4=md_runstat(add_md1);	// turn ON led4 when motor run
				for(i=0;i<10;i++)			// delay for motor running time
				{
					delay(50000);
				}
				md_brake(add_md1);			// brake motor
				led4=md_runstat(add_md1);	// turn OFF led4 when motor brake
				for(i=0;i<5;i++)			// delay for motor brake time
				{
					delay(50000);
				}
			}
		}

		if(sw2==0)							// Test whether SW2 on IFC-MB00 is pressed
		{
			led2 = 1;						// turn ON LED2 on MB00 by sending value '1'
			buzzer = 1;						// buzzer ON for first time
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			delay(50000);					// delay for buzzer OFF time
			buzzer = 1;						// buzzer ON for second times
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			
		
			while(1)
			{				
				// this is an infinity loop after SW2 on IFC-MB00 is pressed, 
				// user need to press reset button on MB00 to exit from the mode
				// Brush Motor is activated. The motor will change the running 
				//direction to counter-clockwise and speed to 100 when
				// encoder value = 1000.
				md_cw(add_md1);						// Run motor in clockwise
				md_speed(add_md1,250);				// motor's speed = 250
				md_encon(add_md1, 1000, 4, 100);	// change motor speed and direction
													// when encoder value = 1000.
				led5=md_enstat(add_md1);			// turn ON led5 when encoder in process
			}
		}

		if(sw3==0)			   				// Test whether SW3 on IFC-MB00 is pressed
		{
			temp1=0;						// clear temp1
			speed=0;						// clear speed
			led3 = 1;						// turn ON LED3 on MB00 by sending value '1'
			buzzer = 1;						// buzzer ON for first time
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			delay(50000);					// delay for buzzer OFF time
			buzzer = 1;						// buzzer ON for second times
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			delay(50000);					// delay for buzzer OFF time
			buzzer = 1;						// buzzer ON for third times
			delay(50000);					// delay for buzzer ON time
			buzzer = 0;						// OFF buzzer
			speed=md_spval(add_md1);		// store speed value to speed
			while(1)
			{
				// this is an infinity loop after SW3 on IFC-MB00 is pressed, 
				// user need to press reset button on MB00 to exit from the mode.
				// Brush Motor is activated. The motor's speed will increase gradually 
				// and maintain in its maximum value, 255 after the increment.
				temp1+=1;						// increase temp1
				if(temp1>10000)					// the value of temp1 here will determine the speed of
				{								// motor being increase, the bigger the value here, 
												// the slower the speed of the increment
					temp1=0;					// clear temp1	
					if(speed<255)speed+=1;		// increase speed value if less than 255
					md_cw(add_md1);				// Run motor in clockwise
					md_speed(add_md1,speed);	// set motor's speed
				}
			}	
		}
	}
}

//=====================================================================================
//	functions
//  User can write all the necessary function here
//=====================================================================================
void delay(unsigned long data)
{
	// this is a delay function for user to use when the program need a delay
	// this function can be call by type : delay(xxxx),
	// user can replace the 'xxxx' with a value to determine how long the program
	// should delay for, the bigger the value, the longer the time of the delay
	for( ;data>0;data-=1);
}


void init(void)
{
	// this is a initialization for set the input and output of PIC18F2685
	// User is advised NOT to change the setting here
	// tris
	TRISB=0x00;				// led as output
	TRISA=0xff;				// push button as input
	TRISC=0x00;				// set port c as output
	
	// the setting for analog to digital converter, ADC for PIC18F2685
	// User is advised NOT to change the setting here
	// adc
	ADCON1=0x0F;
	
	//call the initialization function of I2C
	iic_init();		
	
	//	set the initial condition of output device on board
	//	User is advised not to change the initial condition
	led=0x00;				// turn OFF all LED on board by sending value '0x00'
	buzzer=0;				// turn OFF buzzer on board by sending value '0'
	error_led=0;			// turn OFF Error LED on board by sending value '0'
	busy_led=1;				// turn ON Busy LED on board by sending value '1'
	slave_reset=1;			// reset slave cards
	delay(10000);			
	slave_reset=0;			
	delay(50000);			
	busy_led=0;				// turn OFF Busy LED on board by sending value '0'
}


//=====================================================================================
//	Interrupt vector
//=====================================================================================
#pragma	code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 	_asm
		goto ISRHigh		// jump to interrupt routine
	_endasm
}
#pragma code
#pragma	code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
 	_asm
		goto ISRLow			// jump to interrupt routine
	_endasm
}
#pragma code

//=====================================================================================
//	Interupt Service Routine
//	this a function reserved for interrupt service routine
//	User may need it in advance development of the program
//=====================================================================================
#pragma interrupt ISRHigh
void ISRHigh(void)
{

}

#pragma interrupt ISRLow
void ISRLow(void)
{

}

