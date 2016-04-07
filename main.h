// audio level meter
// 2 x 9 LED
// main.h (file 1 of 2)
// compiler: "HI-TECH C PRO for the PIC18 MCU Family  V9.63PL3"
// 2016


#define _XTAL_FREQ 8000000


#include <htc.h>


//********************************** Config Bits for PIC18F4320 **********************************

// oscillator
__CONFIG(1, RCIO );

// power-up timer  //BOR 2.0V  //disable BOR  //WDT prescaler  //disable WDT
__CONFIG(2, PWRTEN & BORV20 & BORDIS & WDTPS512 & WDTDIS );

// CCP2 multiplexed to RC1
__CONFIG(3, CCP2RC1 );

// background debugger  //low-voltage ICSP enabled  //stack full/underflow reset
__CONFIG(4, DEBUGDIS & LVPEN & STVREN );

// unprotected code
__CONFIG(5, UNPROTECT );

// write enabled
__CONFIG(6, WRTEN );

// table read unprotected 
__CONFIG(7, TRU	);

//************************************************************************************************


__EEPROM_DATA(0, 0, 0, 0, 0, 0, 3, 0);  //byte6 - mode
#define OPMOD_ADDR 6


#define  TMR0_LOAD 65450 //preload Timer 0 for overflow
//Fovf = ((Fosc/4)/prescaler)/(65536-LOAD)
//125k/(65536-65450)


//**********************************  Button  **************************************

#define BUT_HLD  1000
#define BUT_DEB  100


#define BTN1_PIN   0
#define BTN1_PORT  PORTB
#define BTN1_TRIS  TRISB

#define BTN2_PIN   7
#define BTN2_PORT  PORTD
#define BTN2_TRIS  TRISD


//***************************  Voltage reference circuit  **************************
#define TL431_PIN   2
#define TL431_PORT  PORTC
#define TL431_TRIS  TRISC


//*********************************   "Mode" LED   *********************************
#define LEDM1_PIN   3
#define LEDM1_PORT  PORTC
#define LEDM1_TRIS  TRISC

#define LEDM2_PIN   7
#define LEDM2_PORT  PORTA
#define LEDM2_TRIS  TRISA


//*****************************  2 x 9 dual-color LEDs  ****************************

#define LED0_PIN   2
#define LED0_PORT  PORTD
#define LED0_TRIS  TRISD

#define LED1_PIN   3
#define LED1_PORT  PORTD
#define LED1_TRIS  TRISD

#define LED2_PIN   4
#define LED2_PORT  PORTC
#define LED2_TRIS  TRISC

#define LED3_PIN   5
#define LED3_PORT  PORTC
#define LED3_TRIS  TRISC

#define LED4_PIN   6
#define LED4_PORT  PORTC
#define LED4_TRIS  TRISC

#define LED5_PIN   7
#define LED5_PORT  PORTC
#define LED5_TRIS  TRISC

#define LED6_PIN   4
#define LED6_PORT  PORTD
#define LED6_TRIS  TRISD

#define LED7_PIN   5
#define LED7_PORT  PORTD
#define LED7_TRIS  TRISD

#define LED8_PIN   6
#define LED8_PORT  PORTD
#define LED8_TRIS  TRISD

//common wires
#define COM0_PIN   1
#define COM0_PORT  PORTD
#define COM0_TRIS  TRISD

#define COM1_PIN   0
#define COM1_PORT  PORTD
#define COM1_TRIS  TRISD

//**********************************************************************************


#define BIT_IS_SET(reg, bit) ((reg>>bit)&1)
#define BIT_IS_CLR(reg, bit) (!((reg>>bit)&1))

#define SET_BIT(reg, bit) (reg |= (1<<bit))
#define CLR_BIT(reg, bit) (reg &= (~(1<<bit)))


#define BTN1_CLR  CLR_BIT(BTN1_PORT, BTN1_PIN)
#define BTN1_SET  SET_BIT(BTN1_PORT, BTN1_PIN)
#define BTN1_OUT  CLR_BIT(BTN1_TRIS, BTN1_PIN)
#define BTN1_INP  SET_BIT(BTN1_TRIS, BTN1_PIN)

#define BTN2_CLR  CLR_BIT(BTN2_PORT, BTN2_PIN)
#define BTN2_SET  SET_BIT(BTN2_PORT, BTN2_PIN)
#define BTN2_OUT  CLR_BIT(BTN2_TRIS, BTN2_PIN)
#define BTN2_INP  SET_BIT(BTN2_TRIS, BTN2_PIN)

#define BUTTON_INIT  BTN2_OUT; BTN2_CLR; BTN1_INP

#define BUTTON   BIT_IS_CLR(BTN1_PORT, BTN1_PIN)


#define TL431_CLR  CLR_BIT(TL431_PORT, TL431_PIN)
#define TL431_SET  SET_BIT(TL431_PORT, TL431_PIN)
#define TL431_OUT  CLR_BIT(TL431_TRIS, TL431_PIN)
#define TL431_INP  SET_BIT(TL431_TRIS, TL431_PIN)

#define TL431_INIT  TL431_OUT; TL431_SET
#define TL431_OFF   TL431_INP


#define LEDM1_CLR  CLR_BIT(LEDM1_PORT, LEDM1_PIN)
#define LEDM1_SET  SET_BIT(LEDM1_PORT, LEDM1_PIN)
#define LEDM1_OUT  CLR_BIT(LEDM1_TRIS, LEDM1_PIN)
#define LEDM1_INP  SET_BIT(LEDM1_TRIS, LEDM1_PIN)

#define LEDM2_CLR  CLR_BIT(LEDM2_PORT, LEDM2_PIN)
#define LEDM2_SET  SET_BIT(LEDM2_PORT, LEDM2_PIN)
#define LEDM2_OUT  CLR_BIT(LEDM2_TRIS, LEDM2_PIN)
#define LEDM2_INP  SET_BIT(LEDM2_TRIS, LEDM2_PIN)

#define LEDM_OFF   LEDM1_CLR; LEDM2_CLR

#define LEDM_INIT  LEDM1_OUT; LEDM2_OUT; LEDM_OFF

#define LEDM_GRN   LEDM1_CLR; LEDM2_SET
#define LEDM_RED   LEDM1_SET; LEDM2_CLR


#define LED0_CLR  CLR_BIT(LED0_PORT, LED0_PIN)
#define LED0_SET  SET_BIT(LED0_PORT, LED0_PIN)
#define LED0_OUT  CLR_BIT(LED0_TRIS, LED0_PIN)
#define LED0_INP  SET_BIT(LED0_TRIS, LED0_PIN)

#define LED1_CLR  CLR_BIT(LED1_PORT, LED1_PIN)
#define LED1_SET  SET_BIT(LED1_PORT, LED1_PIN)
#define LED1_OUT  CLR_BIT(LED1_TRIS, LED1_PIN)
#define LED1_INP  SET_BIT(LED1_TRIS, LED1_PIN)

#define LED2_CLR  CLR_BIT(LED2_PORT, LED2_PIN)
#define LED2_SET  SET_BIT(LED2_PORT, LED2_PIN)
#define LED2_OUT  CLR_BIT(LED2_TRIS, LED2_PIN)
#define LED2_INP  SET_BIT(LED2_TRIS, LED2_PIN)

#define LED3_CLR  CLR_BIT(LED3_PORT, LED3_PIN)
#define LED3_SET  SET_BIT(LED3_PORT, LED3_PIN)
#define LED3_OUT  CLR_BIT(LED3_TRIS, LED3_PIN)
#define LED3_INP  SET_BIT(LED3_TRIS, LED3_PIN)

#define LED4_CLR  CLR_BIT(LED4_PORT, LED4_PIN)
#define LED4_SET  SET_BIT(LED4_PORT, LED4_PIN)
#define LED4_OUT  CLR_BIT(LED4_TRIS, LED4_PIN)
#define LED4_INP  SET_BIT(LED4_TRIS, LED4_PIN)

#define LED5_CLR  CLR_BIT(LED5_PORT, LED5_PIN)
#define LED5_SET  SET_BIT(LED5_PORT, LED5_PIN)
#define LED5_OUT  CLR_BIT(LED5_TRIS, LED5_PIN)
#define LED5_INP  SET_BIT(LED5_TRIS, LED5_PIN)

#define LED6_CLR  CLR_BIT(LED6_PORT, LED6_PIN)
#define LED6_SET  SET_BIT(LED6_PORT, LED6_PIN)
#define LED6_OUT  CLR_BIT(LED6_TRIS, LED6_PIN)
#define LED6_INP  SET_BIT(LED6_TRIS, LED6_PIN)

#define LED7_CLR  CLR_BIT(LED7_PORT, LED7_PIN)
#define LED7_SET  SET_BIT(LED7_PORT, LED7_PIN)
#define LED7_OUT  CLR_BIT(LED7_TRIS, LED7_PIN)
#define LED7_INP  SET_BIT(LED7_TRIS, LED7_PIN)

#define LED8_CLR  CLR_BIT(LED8_PORT, LED8_PIN)
#define LED8_SET  SET_BIT(LED8_PORT, LED8_PIN)
#define LED8_OUT  CLR_BIT(LED8_TRIS, LED8_PIN)
#define LED8_INP  SET_BIT(LED8_TRIS, LED8_PIN)


#define COM0_CLR  CLR_BIT(COM0_PORT, COM0_PIN)
#define COM0_SET  SET_BIT(COM0_PORT, COM0_PIN)
#define COM0_OUT  CLR_BIT(COM0_TRIS, COM0_PIN)
#define COM0_INP  SET_BIT(COM0_TRIS, COM0_PIN)

#define COM1_CLR  CLR_BIT(COM1_PORT, COM1_PIN)
#define COM1_SET  SET_BIT(COM1_PORT, COM1_PIN)
#define COM1_OUT  CLR_BIT(COM1_TRIS, COM1_PIN)
#define COM1_INP  SET_BIT(COM1_TRIS, COM1_PIN)


#define COM0_RED  COM0_OUT; COM0_CLR
#define COM0_GRN  COM0_OUT; COM0_SET

#define COM1_RED  COM1_OUT; COM1_SET
#define COM1_GRN  COM1_OUT; COM1_CLR

//column 0
#define C0LD0_RED  LED0_OUT; LED0_SET; COM0_RED
#define C0LD1_RED  LED1_OUT; LED1_SET; COM0_RED
#define C0LD2_RED  LED2_OUT; LED2_SET; COM0_RED
#define C0LD3_RED  LED3_OUT; LED3_SET; COM0_RED
#define C0LD4_RED  LED4_OUT; LED4_SET; COM0_RED
#define C0LD5_RED  LED5_OUT; LED5_SET; COM0_RED
#define C0LD6_RED  LED6_OUT; LED6_SET; COM0_RED
#define C0LD7_RED  LED7_OUT; LED7_SET; COM0_RED
#define C0LD8_RED  LED8_OUT; LED8_SET; COM0_RED

#define C0LD0_GRN  LED0_OUT; LED0_CLR; COM0_GRN
#define C0LD1_GRN  LED1_OUT; LED1_CLR; COM0_GRN
#define C0LD2_GRN  LED2_OUT; LED2_CLR; COM0_GRN
#define C0LD3_GRN  LED3_OUT; LED3_CLR; COM0_GRN
#define C0LD4_GRN  LED4_OUT; LED4_CLR; COM0_GRN
#define C0LD5_GRN  LED5_OUT; LED5_CLR; COM0_GRN
#define C0LD6_GRN  LED6_OUT; LED6_CLR; COM0_GRN
#define C0LD7_GRN  LED7_OUT; LED7_CLR; COM0_GRN
#define C0LD8_GRN  LED8_OUT; LED8_CLR; COM0_GRN

//column 1
#define C1LD0_RED  LED0_OUT; LED0_CLR; COM1_RED
#define C1LD1_RED  LED1_OUT; LED1_CLR; COM1_RED
#define C1LD2_RED  LED2_OUT; LED2_CLR; COM1_RED
#define C1LD3_RED  LED3_OUT; LED3_CLR; COM1_RED
#define C1LD4_RED  LED4_OUT; LED4_CLR; COM1_RED
#define C1LD5_RED  LED5_OUT; LED5_CLR; COM1_RED
#define C1LD6_RED  LED6_OUT; LED6_CLR; COM1_RED
#define C1LD7_RED  LED7_OUT; LED7_CLR; COM1_RED
#define C1LD8_RED  LED8_OUT; LED8_CLR; COM1_RED

#define C1LD0_GRN  LED0_OUT; LED0_SET; COM1_GRN
#define C1LD1_GRN  LED1_OUT; LED1_SET; COM1_GRN
#define C1LD2_GRN  LED2_OUT; LED2_SET; COM1_GRN
#define C1LD3_GRN  LED3_OUT; LED3_SET; COM1_GRN
#define C1LD4_GRN  LED4_OUT; LED4_SET; COM1_GRN
#define C1LD5_GRN  LED5_OUT; LED5_SET; COM1_GRN
#define C1LD6_GRN  LED6_OUT; LED6_SET; COM1_GRN
#define C1LD7_GRN  LED7_OUT; LED7_SET; COM1_GRN
#define C1LD8_GRN  LED8_OUT; LED8_SET; COM1_GRN


//all LEDs turn OFF
#define LED_ALL_OFF  LED0_INP; \
                     LED1_INP; \
                     LED2_INP; \
                     LED3_INP; \
                     LED4_INP; \
                     LED5_INP; \
                     LED6_INP; \
                     LED7_INP; \
                     LED8_INP; \
                     COM0_INP; \
                     COM1_INP


typedef signed char int8_t; //stdint
typedef unsigned char uint8_t;
typedef signed int int16_t;
typedef unsigned int uint16_t;


// 1023 724 512 362 256 181 128 91 64 45 32 22 16 11 8 6 4 3 2 1

const uint16_t levels[6][9] = {
  {   1,   2,   3,   4,   6,   8,  11,  16,  22}, // 0 //
  {  11,  16,  22,  32,  45,  64,  91, 128, 181}, // 1 //
  {  64,  91, 128, 181, 256, 362, 512, 724,1023}, // 2 //
  //
  {  22,  16,  11,   8,   6,   4,   3,   2,   1}, // 3 //
  { 181, 128,  91,  64,  45,  32,  22,  16,  11}, // 4 //
  {1023, 724, 512, 362, 256, 181, 128,  91,  64}  // 5 //
};

const uint8_t colors[6][9] = {
  {1,1,1,1,1,1,1,2,2},
  {1,1,1,1,1,1,2,2,2},
  {1,1,1,1,1,2,2,2,2},
  //
  {2,2,1,1,1,1,1,1,1},
  {2,2,2,1,1,1,1,1,1},
  {2,2,2,2,1,1,1,1,1}
};

uint16_t chan[2] = {0,0};  //ADC result for two channels

uint16_t butcnt=0; //counters
uint8_t muxcnt=0;

uint8_t butst=0;  //button state: 2 - long press, 1 - short press, 0 - not pressed
uint8_t opmod=0;  //the mode of operation

uint8_t msrun=0;  //flag: run the measurement

