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


//---------------------------------------------------------------------------------------
void interrupt handler(void)
	{
	if(LVDIF && LVDIE)  //*********  low-voltage detect interrupt  ************
		{
		LED_ALL_OFF;
		LEDM_OFF;
		TL431_OFF;
		
		//save settings
		if(EEPROM_READ(OPMOD_ADDR) != opmode) EEPROM_WRITE(OPMOD_ADDR,opmode);
		if(EEPROM_READ(MRANG_ADDR) != mrange) EEPROM_WRITE(MRANG_ADDR,mrange);
		
		LEDM_RED;
		while(1);
		}
	
	if(TMR0IF && TMR0IE)  //*******  timer0 overflow interrupt ~1kHz  *********
		{
		TMR0IF=0;
		TMR0=TMR0_LOAD;
		
		///////////////////////////  LED multiplexing  ////////////////////////
		
		LEDM_OFF;
		
		if(muxcnt & 0b00000011)  {  if(mrange==0) { LEDM_GRN; }  }
		  else  {  if(mrange==1) { LEDM_RED; }  }
		
		LED_ALL_OFF;
		
		led_con(muxcnt, buff1[muxcnt]);
		
		if(muxcnt==14) msrun=1;
		if(muxcnt==15) conrun=1;
		if(muxcnt==17) bufrun=1;
		
		if(++muxcnt>17) muxcnt=0;
		
		//////////////////////////////  Button  ///////////////////////////////
		
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
	for(uint8_t k=0; k<18; k++) buff1[k]=(mrange+1);
	muxcnt=0;
	delay_ms((uint16_t)delay*10U);
	}


//---------------------------------------------------------------------------------------
void peak_reset(void)  //reset peaks
	{
	pcnt0=0; pcnt1=0; p0=-1; p1=8;
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
	
	mrange=EEPROM_READ(MRANG_ADDR);
	opmode=EEPROM_READ(OPMOD_ADDR);
	
	led_test(120);
	
	LVDIF=0;
	LVDIE=1;  //low-voltage detect interrupt enable
	
	peak_reset();
	}


//---------------------------------------------------------------------------------------
void conv(void)  //converting Levels to LED bars
	{
	uint8_t c=1; //channel
	
	if(opmode)  //normal mode
		{
		for(uint8_t k=0; k<18; k++)  //converting
			{
			uint8_t t=0;
			
			if(k<9) { t=k; }
			  else  { t=k-9; c=0; }
			
			buff0[k]=OFF;  //cleaning buffer #1
			
			if(chan[c]>=levels[mrange][t]) buff0[k]=colors[mrange][t];
			}
		}
	else  //peak hold mode
		{
		for(uint8_t k=0; k<18; k++)
			{
			uint8_t t=0;
			
			if(k<9) { t=k; }
			  else  { t=k-9; c=0; }
			
			buff0[k]=OFF;
			
			if(chan[c]>=levels[mrange][t])
				{
				buff0[k]=GREEN;
				if(chan[c]<levels[mrange][t+1])
					{
					if(k<9) { if(k>=p0) { p0=k; pcnt0=PH_FORW; }  }
					  else  { if(k>=p1) { p1=k; pcnt1=PH_FORW; }  }
					}
				}
			}
		
		if(p0>=0) buff0[p0]=RED;
		if(p1>=9) buff0[p1]=RED;
	
		//////////////////////////  Peak Hold counters  ///////////////////////
		
		if(pcnt0==0 && p0>=0)  { pcnt0=PH_BACK;  p0--; }
		if(pcnt1==0 && p1>=9)  { pcnt1=PH_BACK;  p1--; }
		
		if(pcnt0) pcnt0--;
		if(pcnt1) pcnt1--;
		}
	
	conrun=0;
	}


//---------------------------------------------------------------------------------------
int main(void)
	{
	setup();
	
	for(;;) //main loop
		{
		///////////////////////////  measurement  /////////////////////////////
		if(msrun)
			{
			ADCON0 = 0b000001;  //select AN0
			GODONE=1;           //start ADC
			while(GODONE);      //wait end of ADC
			chan[0]=ADRES;      //ADC result
			
			ADCON0 = 0b000101;  //select AN1
			GODONE=1;
			while(GODONE);
			chan[1]=ADRES;
			
			msrun=0;
			}
		
		/////////////////////////////  converting  ////////////////////////////
		if(conrun) conv();
		
		/////////////////////////////  buffering  /////////////////////////////
		if(bufrun) 
			{
			for(uint8_t k=0; k<18; k++) buff1[k]=buff0[k];
			bufrun=0;
			}
			
		//////////////////////////////  button  ///////////////////////////////
		switch(butst)
			{
			case 2:  //a long press //change the mode
				if(++opmode>1) opmode=0;
				led_test(40);
				butcnt=0; 
				butst=0;
				peak_reset();
				break;
			//
			case 1:  //a short press  //change the range
				butst=0; //reset the button state
				if(++mrange>1) mrange=0;
				led_test(20);
				peak_reset();
				break;
			}
		}
	}

