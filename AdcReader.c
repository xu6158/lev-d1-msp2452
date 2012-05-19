//#include "msp430g2452.h"
//#include "AdcReader.h"
#include "LEV_PrjDefine.h"




#define NUM_OF_CONVERSION     20

volatile unsigned int ADC_Results[NUM_OF_CONVERSION];
//unsigned int ADCresults_Avg;

void InitAdcReader()
{
  
//  //ADC10 clock source: ADC10OSC
//  //A4/A3/A2/A1/A0, repeat multi channel,Repeat-sequence-of-channels
//  ADC10CTL1 = INCH_4 + CONSEQ_3;
//  //ADC10ON: ADC10 on
//  //REFON: Reference generator on
//  //ADC10SHT_2:   16 ��ADC10CLKs
//  //ADC10SHT_3:   64 ��ADC10CLKs
//  //MSC:Multiple sample and conversion.
//  //SREF_1: VR+ = VREF+ and VR- = VSS
//  ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE +SREF_1+REFON+REF2_5V;
//  //Analog (Input) Enable Control Register
//  ADC10AE0 = 0x1F;                          // P1.0,1,2,3,4  ADC option select
//  //the number of transfers in each block.
//  ADC10DTC1 = 0x14;//0x10;                  // 4 conversions, 20 samples
//  
//  //ADC10DTC0 = ADC10CT;
//  //ADC10SA = (unsigned int)ADC_Results;//0x200;     // Data buffer start
}
void OffAdcReader(){
//  ADC10CTL0 &= ~ENC;
//  ADC10CTL0 = 0;
}

void StartAdcConversion() 
{
  ADC10CTL1 = INCH_4 + CONSEQ_3;
  ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE +SREF_1+REFON+REF2_5V;
  ADC10AE0 = 0x1F;                          // P1.0,1,2,3,4  ADC option select
  ADC10DTC1 = 0x14;//0x10;                  // 4 conversions, 20 samples
  __delay_cycles(125);  // 125us ==> 1MHz clock

  ADC10CTL0 &= ~ENC;
  while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
  ADC10SA = (unsigned int)ADC_Results;    // Data buffer start
 
  ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion ready
  //__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
  __delay_cycles(125);  // 125us ==> 1MHz clock
  
  ADC10CTL0 &= ~ENC;
  ADC10CTL0 = 0;
  
}

//void StopAdcConversion()
//{
//  ADC10CTL0 &= ~ENC;
//}

unsigned int GetADCValue(unsigned char ADC_Channel)
{
  int i = 0;
  unsigned int ADCresults_Avg = 0;
  for (i = ADC_Channel; i < NUM_OF_CONVERSION; i += 5)
  {
    ADCresults_Avg += ADC_Results[i];
  }
  ADCresults_Avg = ADCresults_Avg >> 2;
  return ADCresults_Avg;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{

  //__delay_cycles(1000); //1MS
  //StopAdcConversion();
  //__bic_SR_register_on_exit(CPUOFF);
  
}



