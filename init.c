#include "p30f6010A.h"

//processor initialisation routine

//configure oscillator
_FOSC(CSW_FSCM_OFF & HS3_PLL16); //26.667Mhz Fcy Tcy .0374995us
_FWDT(WDT_ON & WDTPSA_1 & WDTPSB_1); //watchdog timer on
_FBORPOR(MCLR_EN & PWMxH_ACT_HI & PWMxL_ACT_HI & /*PBOR_ON & BORV_45 &*/ RST_IOPIN );

void init(void)
{

//configure digital outputs
TRISGbits.TRISG0 = 0; 
TRISGbits.TRISG1 = 0; 
TRISGbits.TRISG2 = 0; 
TRISGbits.TRISG3 = 0; 
TRISGbits.TRISG6 = 0; 

//configure PWM
//PTCONbits.PTEN = 1; //pwm enable
PTCONbits.PTMOD = 2;

PTPERbits.PTPER = 1776; //7.5Khz Fpwm

PWMCON1bits.PEN1L = 1;
PWMCON1bits.PEN2L = 1;
PWMCON1bits.PEN3L = 1;

PWMCON1bits.PEN1H = 1;
PWMCON1bits.PEN2H = 1;
PWMCON1bits.PEN3H = 1;

PWMCON2bits.IUE = 0;

//configure dead time
DTCON1bits.DTAPS = 3;
DTCON1bits.DTA = 10; //DT 2.99us

//fault input configuration
FLTBCONbits.FBEN1 = 1; //enable now
FLTBCONbits.FBEN2 = 1;
FLTBCONbits.FBEN3 = 1;

//timer1 configuration
PR1 = 40; //Period of 1.5us
T1CONbits.TON = 1;

//ADC configuration

ADCON1bits.SSRC = 7;
ADCON1bits.SIMSAM = 0;

ADCON2= 0; //CH0 only

ADCON3bits.SAMC = 2; //sample time 2*Tad
ADCON3bits.ADCS = 5; //4//Tconv = 112ns

ADPCFG = 0x8013;
ADCSSL = 0;

ADCON1bits.ADON = 1;


//configure interrupts
INTCON1bits.NSTDIS = 1; //disable interrupt nesting if set
IPC11bits.FLTBIP = 0x7;
IEC2bits.FLTBIE = 1; //enable fault interrupt
IEC0bits.T1IE = 1; //enable T1 interrupt
IPC0bits.T1IP = 0x5;
IEC2bits.PWMIE = 1; //enable pwm interrupt
IPC9bits.PWMIP = 0x6;

}



