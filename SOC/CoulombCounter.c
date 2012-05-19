
#include "..\SystemInitialData\SystemInformation.h"
#include "..\LEV_PrjDefine.h"
//TimerA_IntervalTime_MS
#define AccumulatingIntervalTime    0.5f  //0.5 sec.
#define AccumulatingTimeFactor      (AccumulatingIntervalTime / 3600)  // maH

float adc_accumulatingQ;
unsigned int Cyclecount;

void InitCoulombCounter(){
  
  if(CURRENT_CYCLECOUNT == 0xffff){
    Cyclecount = 0;
  }else{
    Cyclecount = CURRENT_CYCLECOUNT;
  }
  
  if(CURRENT_ADC_ACCUMULATING_Q == 0xffff){
    adc_accumulatingQ = 0.0;
  }else{
    adc_accumulatingQ = (float)CURRENT_ADC_ACCUMULATING_Q;
  }
}


void AccumulatingQ(){
  float q;
  
  if(adc_accumulatingQ >= ADC_CYCLECOUNT_THRESHOLD){
    Cyclecount++;
    adc_accumulatingQ = 0.0f;
  }
  
  q = (float)(GetADCValue(IDSG_ADC));
  q = q * AccumulatingTimeFactor;
  adc_accumulatingQ += q;
}

void WriteCoulombCounterDataToFlash(){
////#define CURRENT_CYCLECOUNT_offset                   32
////#define CURRENT_ADC_ACCUMULATING_Q_offset           34
  unsigned char temp[4];
  unsigned int temp1;
  temp1 = (unsigned int) adc_accumulatingQ;
  temp[0] = Cyclecount;
  temp[1] = Cyclecount >> 8;
  temp[2] = temp1;
  temp[3] = temp1 >> 8;
  WriteDataToFlash(CURRENT_CYCLECOUNT_offset, temp, 4, 1);  //0:CONFIG_SEGMENT, 1:CALIBRATION_SEGMENT

}
