//program name and function definitions

#include "p30f6010A.h"

#define RL1_ON PORTGbits.RG0
#define RL2_ON PORTGbits.RG1
#define RL3_ON PORTGbits.RG2
#define RL4_ON PORTGbits.RG3
#define RL5_ON PORTGbits.RG6

#define PWM1 PDC1 //R PWM
#define PWM2 PDC2 //Y PWM
#define PWM3 PDC3 //B PWM

#define T1us_Flag IFS0bits.T1IF //every 1.5us flag

#define PID_V_count 33 //20.2Khz sampling
#define PID_I_count 12 //55.55Khz sampling
#define DCLinkCount 1000 //1.5ms sampling

#define PWM_PERIOD PTPER 
#define Converstion_Done ADCON1bits.DONE
#define Start_Converstion ADCON1bits.SAMP

#define CH0_MULA ADCHSbits.CH0SA
#define CH0_MULB ADCHSbits.CH0SB

#define PWMenable PTCONbits.PTEN
#define PWM_Flag IFS2bits.PWMIF

#define Ipeak_flag IFS2bits.PWMIF

#define VDCLink_Trip 905 //1023counts == 200V
#define VDCBalance 100 //Dc link unbalace limit @20V

////////////////////////////////////////////////////////////////////////////////////////
#define Irdc_Limit 180  //op 4A current trip //250//345 //armature current limit setting
#define Iydc_Limit 90 //op 0.6A current trip //250//45  //field current limit setting
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////
#define V_Pgain 16 //voltage PI gain
#define V_Igain 1  //voltage PI integral gain

#define I_Pgain 37 //current PI p gain
////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
#define Vr_ref 870 //1count == .218volts //max voltage ref 920 //armature voltage setting
#define Vy_ref 3450//1count == .218volts //max voltage ref 920 //field voltage setting
//////////////////////////////////////////////////////////////////////////////////////////
                                        
#define softCount 1000 //0.75mS

#define IrTIMER_ON T2CONbits.TON //timer2 for peak current detect
#define IrCOUNT PR2 
#define T2_Flag IFS0bits.T2IF //for Ir peak detect

#define IyTIMER_ON T3CONbits.TON //timer3 for peak current detect
#define IyCOUNT PR3
#define T3_Flag IFS0bits.T3IF //for Iy peak detect


//*************************************************//

#define ADC_ON ADCON1bits.ADON

#define SET PWMCON1 

#define fault_Flag IFS2bits.FLTBIF
#define PWM_InterruptEnable IEC2bits.PWMIE

//program user functions
int adc(char);
void delay(unsigned int);
void init(void);

