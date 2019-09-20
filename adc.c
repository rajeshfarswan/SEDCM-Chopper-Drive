#include "p30f6010A.h"
#include "main.h"

// adc function // input argument is channel number//

int adc(char ADC_NO)

{
	
CH0_MULA = (int)ADC_NO;
CH0_MULB = ADC_NO;
Start_Converstion = 1;
while(!Converstion_Done);
Converstion_Done = 0;

return (ADCBUF0);
     
} //adc end
