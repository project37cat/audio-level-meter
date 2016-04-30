// audio level meter
// main.c (file 2 of 2)


#include "main.h"


//---------------------------------------------------------------------------------------
void led_con(uint8_t num, uint8_t color)  //color: 2==RED, 1==GREEN
	{
	switch(num)
		{
		case 0:  if(color==2) { C0LD0_RED; } else if(color==1) { C0LD0_GRN; } break;
		case 1:  if(color==2) { C0LD1_RED; } else if(color==1) { C0LD1_GRN; } break;
		case 2:  if(color==2) { C0LD2_RED; } else if(color==1) { C0LD2_GRN; } break;
		case 3:  if(color==2) { C0LD3_RED; } else if(color==1) { C0LD3_GRN; } break;
		case 4:  if(color==2) { C0LD4_RED; } else if(color==1) { C0LD4_GRN; } break;
		case 5:  if(color==2) { C0LD5_RED; } else if(color==1) { C0LD5_GRN; } break;
		case 6:  if(color==2) { C0LD6_RED; } else if(color==1) { C0LD6_GRN; } break;
		case 7:  if(color==2) { C0LD7_RED; } else if(color==1) { C0LD7_GRN; } break;
		case 8:  if(color==2) { C0LD8_RED; } else if(color==1) { C0LD8_GRN; } break;
		//
		case 9:  if(color==2) { C1LD0_RED; } else if(color==1) { C1LD0_GRN; } break;
		case 10: if(color==2) { C1LD1_RED; } else if(color==1) { C1LD1_GRN; } break;
		case 11: if(color==2) { C1LD2_RED; } else if(color==1) { C1LD2_GRN; } break;
		case 12: if(color==2) { C1LD3_RED; } else if(color==1) { C1LD3_GRN; } break;
		case 13: if(color==2) { C1LD4_RED; } else if(color==1) { C1LD4_GRN; } break;
		case 14: if(color==2) { C1LD5_RED; } else if(color==1) { C1LD5_GRN; } break;
		case 15: if(color==2) { C1LD6_RED; } else if(color==1) { C1LD6_GRN; } break;
		case 16: if(color==2) { C1LD7_RED; } else if(color==1) { C1LD7_GRN; } break;
		case 17: if(color==2) { C1LD8_RED; } else if(color==1) { C1LD8_GRN; } break;
		}
	}


//-------------------------------------------------------------------------------------------------
void interrupt handler(void)
	{
	if(LVDIF && LVDIE)  //********************  low-voltage detect interrupt  *********************
		{
		LED_ALL_OFF; //all leds turn off
		LEDM_OFF;
		TL431_OFF;
		if(EEPROM_READ(OPMOD_ADDR) != opmod)  //save setting
			{
			EEPROM_WRITE(OPMOD_ADDR,opmod);
			C0LD8_RED;
			}
		  else { C0LD8_GRN; }
		while(1);
		}
		
	if(TMR0IF && TMR0IE)  //****************  timer0 overflow interrupt ~1kHz  ********************
		{
		TMR0IF=0;
		TMR0=TMR0_LOAD;
		
		/////////////////////////////////////  LED multiplexing  //////////////////////////////////
		static uint16_t buff0=0;
		static uint16_t buff1=0;
		static uint8_t tmp=0;
		
		LEDM_OFF;
		
		if(muxcnt & 0b00000011)  {  if(opmod!=2 && opmod!=5) { LEDM_GRN; }  }
		  else  {  if(opmod!=0 && opmod!=3) { LEDM_RED; }  }
		  
		LED_ALL_OFF;
		
		if(muxcnt==0)  { buff0=chan[0]; buff1=chan[1]; }  //if new loop, get ADC results
		
		if(muxcnt>8) { buff0=buff1; tmp=(muxcnt-9); } else { tmp=muxcnt; }  //if the second column
		
		if(buff0>=levels[opmod][tmp])  led_con(muxcnt, colors[opmod][tmp]);  //led control
		
		if(++muxcnt>17) { muxcnt=0; msrun=1; }
		
		///////////////////////////////////////  Button  //////////////////////////////////////////
		static uint8_t buthold=0;
		
		if(BUTTON) //if button is pressed
			{
			if(++butcnt>=BUT_HLD)
				{
				butcnt=0;
				butst=2; //long press
				buthold=1;
				}
			}
		else
			{
			if(butcnt>=BUT_DEB && buthold==0) butst=1; //short press
			buthold=0;
			butcnt=0;
			}
		}
	}


//---------------------------------------------------------------------------------------
void delay_ms(uint16_t val)  //delay milliseconds
	{
	while(val--) __delay_ms(1);
	}


//---------------------------------------------------------------------------------------
void led_test(uint8_t delay)  //delay x10ms
	{
	chan[1]=chan[0]=1023;
	msrun=0;
	muxcnt=0;
	delay_ms((uint16_t)delay*10U);
	}


//---------------------------------------------------------------------------------------
void setup(void) //init MCU
	{
	IRCF2=1; IRCF1=1; IRCF0=1; //8MHz internal RC oscillator  //Fosc
	
	SWDTEN=0; //disable watchdog timer
	
	RBPU=0; //enable pull-up on PORTB
	
	BUTTON_INIT;
	TL431_INIT;
	LEDM_INIT;
	LEDM_GRN;

	ADCON2 = 0b10111110; //A/D Result Format  //6-N/A  //111=20TAD  //110=FOSC/64
	ADCON1 = 0b011101; //Voltage Reference - pin AN3 //Port Configuration - pin AN0,AN1
	ADCON0 = 0b000001; //Channel 0  //A/D converter module is enabled

	//TMR0ON T08BIT T0CS T0SE PSA T0PS2 T0PS1 T0PS0
	T0CON=0b10010011; //prescaler 011 - 1:16  Fosc/4=2000, 2000/16=125kHz
	TMR0=TMR0_LOAD; //preload
	TMR0IE=1; //timer0 overflow intterrupt enable
	
	LVDCON=0b11110; //low-voltage detect enable //1110 = 4.50V-4.78V
	
	PEIE=1; //enable interrupts
	GIE=1;
	
	opmod = EEPROM_READ(OPMOD_ADDR); //read setting
	
	led_test(120);
	
	LVDIF=0;
	LVDIE=1;  //low-voltage detect interrupt enable
	}


//---------------------------------------------------------------------------------------
int main(void)
	{
	setup();
	
	for(;;) //main loop
		{
		if(msrun)  //measurement
			{
			ADCON0 = 0b000001;  //select AN0
			GODONE=1;           //start ADC
			while(GODONE);      //wait end of ADC
			(opmod<3) ? chan[0]=ADRES : chan[1]=ADRES;  //ADC result
			
			ADCON0 = 0b000101;  //select AN1
			GODONE=1;
			while(GODONE);
			(opmod<3) ? chan[1]=ADRES : chan[0]=ADRES;
			
			msrun=0;
			}
		
		switch(butst) //check the button state
			{
			case 2:  //long press  //change direction
				switch(opmod)
					{
					case 0: opmod=3; break;
					case 1: opmod=4; break;
					case 2: opmod=5; break;
					//
					case 3: opmod=0; break;
					case 4: opmod=1; break;
					case 5: opmod=2; break;
					}
				led_test(40);
				butcnt=0; //button reset
				butst=0;
				break;
			//
			case 1:  //short press  //change the range
				butst=0;
				switch(opmod)
					{
					case 0: opmod=1; break;
					case 1: opmod=2; break;
					case 2: opmod=0; break;
					//
					case 3: opmod=4; break;
					case 4: opmod=5; break;
					case 5: opmod=3; break;
					}
				led_test(20);
				break;
			}
		}
	}

