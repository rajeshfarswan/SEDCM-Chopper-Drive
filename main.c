//*********************************************************************//
//            AC LAB KIT BUCK SEDCM DRIVE                              //   
//        R phase for Armature closed loop control                     //
//        Y/B phase for field open loop control                        //
//*********************************************************************//

//include files
#include "p30f6010A.h"
#include "main.h"
//

//define program variables
unsigned int DCLinkSample = 0;
char PID_Vsample =0;           //voltage PID sampling counter
char PID_Isample = 0;          //current PID sampling counter
unsigned int softStart = 0;    //soft start counter

int Ir = 0;
int Iy = 0;

static int VDC1 = 0;
int VDC2 = 0;

static int Voffset = 0; //2.5V offset
static int Ir_offset = 0;
static int Iy_offset = 0;

static int DCLink_PREvalue = 0;
static long int Vr_PREerror = 0;

static long int R_Integral = 0;

static int Vr_PI_value = 0;

static int VrREF = -(55200/(int)V_Pgain); //armature voltage initial reference
static int VyREF = 0; //field reference
//

//define program functions
int adc(char);     //adc function

void PID_Vr(void); // Voltage PI
void PID_Ir(void); // Inner loop current PI

void delay(unsigned int); //general purpose delay function
//

int main(void)
{

init(); // call processor inialisation routine

//precharging init
PWMenable = 0; //PWM is off
delay(30);     //delay 30ms
RL1_ON = 1;    //precharging enable
delay(1000);   //delay 1000ms
RL1_ON = 0;    //precharging disable
delay(30);     //delay 30ms
RL2_ON = 1;    //bypass precharging
delay(30);     //delay 30ms
PWMenable = 1; //PWM is on

PWM1 = 0;
PWM2 = 0;
PWM3 = 0;
//precharging init ends

Voffset = adc(14); //read 2.5V offset
Ir_offset = adc(8); //read Ir* offset //adc no. 8//

//resetting PID parameters
PID_Vr(); //voltage loop PI //armature voltage
PID_Ir(); //Inner current loop PI //armature current

ClrWdt(); //reset watchdog timer

    while(1)
{

//DCLink protection loop 
if(DCLinkSample >= (int)DCLinkCount)
{
VDC1 = adc(3); //read dc -link voltage values                  //adc channel 3
VDC2 = adc(2);                                                 //adc channel 2
if((VDC1 >= (int)VDCLink_Trip) || (VDC2 >= (int)VDCLink_Trip)) //overvoltage protection trip
PWMenable = 0;                                                 //disable on dc link overvoltage
else if ((VDC1 - VDC2) >= (int)VDCBalance) 
PWMenable = 0;                                                 //disable PWM on dc link unbalance
else if ((VDC1 - VDC2) <= -(int)VDCBalance) 
PWMenable = 0;                                                 //disable PWM on dc link unbalance
else Nop();
DCLinkSample = 0;
} //DCLink Status monitor end

if(PID_Vsample >= (char)PID_V_count) //voltage PI loop
   {
    PID_Vr(); //armature voltage control 

    PID_Vsample = 0;

   } //voltage sample end

if(PID_Isample >= (char)PID_I_count) //current PI loop
   {
    VDC1 = adc(3);           //dc link voltage for feedforward correction //adc channel 3
    PID_Ir(); //armature current control

    VDC1 = adc(3);          //dc link voltage for feedforward correction
    DCLink_PREvalue = VDC1; //dc-link previous value
    PID_Isample = 0;

   } //current sample end

if(softStart >= (unsigned int)softCount) //soft start for field and armature winding
   {
    VyREF++; //soft start open loop armature setting reference

    if(VyREF >= (int)Vy_ref) 
         {     
              VyREF = (int)Vy_ref; //final set point field setting
                 }  
 
    if(VyREF >= (int)Vy_ref) //if field value is set 
                    {
                    VrREF++; //set point armature setting
                     if(VrREF >= (int)Vr_ref) VrREF = (int)Vr_ref;
                        }
    softStart = 0;

   } //soft start end

    } //while end

} //main end

// ***************************************************************

//Vr PID routine *************************************************
void PID_Vr()
{

int currentError = 0;
long int Pterm = 0;
long int Iterm = 0;

int voltage;
long int PI_value = 0;

voltage = adc(5); //read Vr adc channel 5
voltage = (voltage - (Voffset+2))<<1; //subtract 2.5 V offset in feedback

if(voltage <= 0) voltage = 0;

currentError = VrREF - voltage;  // calculate error

Pterm = (long int)currentError*(long int)V_Pgain; //calculate Proportional term = error*Pgain
if(Pterm >= 55200) Pterm = 55200;
if(Pterm <= -55200) Pterm = -55200;

R_Integral = Vr_PREerror + (long int)currentError;  //calculated integrated error term = current error+previous error
if(R_Integral >= 55200) R_Integral = 55200;
if(R_Integral <= -55200) R_Integral = -55200;
Vr_PREerror = R_Integral;

Iterm = R_Integral*(long int)V_Igain; //calculate Integral term = integarted error*Igain
if(Iterm >= 55200) Iterm = 55200;
if(Iterm <= -55200) Iterm = -55200;

PI_value = Pterm + Iterm; // PI output = Pterm+Iterm
if(PI_value >= 55200) PI_value = 55200;
if(PI_value <= -55200) PI_value = -55200;

PI_value = PI_value>>5;

Vr_PI_value = (int)PI_value; //voltage PI output is current reference

}
//Vr PID routine ends ***************************************

//Ir PID routine *************************************************
void PID_Ir()
{
int currentError = 0;
long int Pterm = 0;

int current;
long int PI_value = 0;

current = adc(8); //adc no. 8//
current = (current - Ir_offset)<<1; //subtract 2.5 V offset in feedback

if(current <= 0) current = 0;

Ir = current;

currentError = Vr_PI_value - current;  // calculate error

Pterm = (long int)currentError*(long int)I_Pgain; //calculate Proportional term = error*Pgain
if(Pterm >= 55200) Pterm = 55200;
if(Pterm <= -55200) Pterm = -55200;

PI_value = Pterm;                                 // PI output = Pterm
if(PI_value >= 55200) PI_value = 55200;
if(PI_value <= -55200) PI_value = -55200;

PI_value = PI_value + 55200;
PI_value = PI_value>>5;

if(VrREF >= (int)Vr_ref)
PI_value =  PI_value - ((VDC1 - DCLink_PREvalue)<<3); //feedforward corection = pi value - (dc link voltage - dc link old value)
if(PI_value <= 0) PI_value = 0;       //pwm min. value
if(PI_value >= 3450) PI_value = 3450; //pwm max. value
PWM1 = (int)PI_value;  //program duty cycle 1 (armature winding)
PWM2 = VyREF;          //program open loop duty cycle 2 (field winding)
}

//Ir PID routine end *********************************************




/////////////////////////////////////////////////////////////////////////////
//fault interrupt
void _ISRFAST __attribute__((interrupt, no_auto_psv)) _FLTBInterrupt(void)
   {
     
     PWMenable = 0; //disable pwm if fault
     SET = 0;       //all switches off
     
ClrWdt();           //clear watchdog timer

fault_Flag = 0;
   }//fault end

/////////////////////////////////////////////////////////////////////////


// Pwm interrupt for over current fault detection

void _ISRFAST __attribute__((interrupt, no_auto_psv)) _PWMInterrupt(void)
{
if(Ir >= (int)Irdc_Limit) asm("BSET _IFS2+1,#4"); //set fault if armature over current

Iy = (adc(9) - Iy_offset)<<1; //read field current
if(Iy <= 0) Iy = 0;
if(Iy >= (int)Iydc_Limit) asm("BSET _IFS2+1,#4"); //set fault if field over current

PWM_Flag = 0;
}

//timers for program tracking
void _ISRFAST __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
  {     
     PID_Vsample++; 
     PID_Isample++;
     softStart++;
     DCLinkSample++;

ClrWdt();
        
     T1us_Flag = 0;
   } //T1 interupt end

///////////////////////////////////////////////////////////////////////////
