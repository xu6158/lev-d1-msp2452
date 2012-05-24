

#include "..\LEV_PrjDefine.h"
//TimerA_IntervalTime_MS
#define AccumulatingIntervalTime    0.5f  //0.5 sec.
#define AccumulatingTimeFactor      (AccumulatingIntervalTime / 3600)  // maH

unsigned long g_Adc_accumulatingQ;
unsigned int Cyclecount;

void InitCoulombCounter(){
  
  if(CURRENT_CYCLECOUNT == 0xffff){
    Cyclecount = 0;
  }else{
    Cyclecount = CURRENT_CYCLECOUNT;
  }
  
  if(CURRENT_ADC_ACCUMULATING_Q == 0xffffffff){
    g_Adc_accumulatingQ = 0;
  }else{
    g_Adc_accumulatingQ = CURRENT_ADC_ACCUMULATING_Q;
  }
  InitADPControl();
  CheckAndSetChagerVoltage();

}


void AccumulatingQ(){
  unsigned int temp;
  if(g_Adc_accumulatingQ >= ADC_CYCLECOUNT_THRESHOLD){
    if(Cyclecount <= CYCLECOUNT_FOR_CHG_LEVEL_2){
      Cyclecount++;
    }
    g_Adc_accumulatingQ = 0;
    CheckAndSetChagerVoltage();
  }
  temp = GetADCValue(IDSG_ADC);
  if( temp >= ADC_DETECT_CURRENT_OF_DSG_STATUS){
    g_Adc_accumulatingQ += (unsigned long)temp;
  }

}
void CheckAndSetChagerVoltage(){
  if(Cyclecount < CYCLECOUNT_FOR_CHG_LEVEL_1){
    setADPControlPin(true);
  }else if(Cyclecount < CYCLECOUNT_FOR_CHG_LEVEL_2) {
    setADPControlPin(false);
  }else{
    setADPControlPin(true);
  }
  
}

void WriteCoulombCounterDataToFlash(){
////#define CURRENT_CYCLECOUNT_offset                   32
////#define CURRENT_ADC_ACCUMULATING_Q_offset           34
  unsigned char temp[6];
  unsigned long temp1;
  temp1 = g_Adc_accumulatingQ;
  temp[0] = Cyclecount;
  temp[1] = Cyclecount >> 8;
  temp[2] = temp1;
  temp[3] = temp1 >> 8;
  temp[4] = temp1 >> 16;
  temp[5] = temp1 >> 24;
  WriteDataToFlash(CURRENT_CYCLECOUNT_offset, temp, 6, 1);  //0:CONFIG_SEGMENT, 1:CALIBRATION_SEGMENT
}


void InitADPControl(){
  ADP_Control_PORT_SEL &= ~(ADP_Control_Pin);
  ADP_Control_PORT_DIR |= (ADP_Control_Pin);  //1: The port pin is switched to output direction, 0: The port pin is switched to input direction
  ADP_Control_PORT_OUT &= ~(ADP_Control_Pin);
}
void setADPControlPin(unsigned char enable){
  if(enable){
    ADP_Control_PORT_OUT |= (ADP_Control_Pin);
  }else{
    ADP_Control_PORT_OUT &= ~(ADP_Control_Pin);
  }
}
