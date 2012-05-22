#include "SOC_OCV_Table.h"

//unsigned char OV_Capacity;
//unsigned char UV_Capacity;

float getCapacityByTable(int real_mVoltage, unsigned char cap_Index);

//float temp_f1;

//void InitSOC(){
//  //////////////////////////////////////////////////////////////////
//  temp_f1 = SOC_1st_CELL_OV_VOLTAGE;
//  //temp_f1 = temp_f1 / NUMBER_OF_BATTERY_SERIES * 10;
//  OV_Capacity = getCapacityByTable(SOC_1st_CELL_OV_VOLTAGE);
//  
//  temp_f1 = SOC_1st_CELL_UV_VOLTAGE;
//  //temp_f1 = temp_f1 / NUMBER_OF_BATTERY_SERIES * 10;
//  UV_Capacity = getCapacityByTable((int)temp_f1);
//}
unsigned char getRealCapacityByCell(int ADC_Voltage, int ADC_current){
  float temp_f2;
  float cap;
  float OV_Capacity;
  float UV_Capacity;
  unsigned char index;
  int real_vol;

  temp_f2 = ADC_Voltage;
  temp_f2 = temp_f2 / VBAT_mV_To_ADC_Factor;
  real_vol = (unsigned int)(temp_f2 / NUMBER_OF_SERIES_CELLS);

  
  if(ADC_current <= ADC_LOOKUP_1st_OCV_CURRENT_RANGE){
    index = RESIDUAL_OCV_CAPACITY_INDEX;
  }else if(ADC_current <= ADC_LOOKUP_2nd_OCV_CURRENT_RANGE){
    index = RESIDUAL_0P5C_CAPACITY_INDEX;
  }else if(ADC_current <= ADC_LOOKUP_3rd_OCV_CURRENT_RANGE){
    index = RESIDUAL_1C_CAPACITY_INDEX;
  }else{
    index = RESIDUAL_2C_CAPACITY_INDEX;
  }
  
  OV_Capacity = getCapacityByTable(SOC_CELL_OV_VOLTAGE, index);
  UV_Capacity = getCapacityByTable(SOC_CELL_UV_VOLTAGE, index);
  cap = getCapacityByTable( real_vol, index);
  
  temp_f2 = OV_Capacity - UV_Capacity;
  temp_f2 = (cap - UV_Capacity) / temp_f2 * 100;
  return (unsigned char)temp_f2;
}
#if 0
//用內插法，計算SOC
float getCapacityByTable(int real_mVoltage, unsigned char cap_Index){
  float cap;
  float cap1, dv;
  unsigned char i;
  
  cap1 = 0.0f;
  if(real_mVoltage >= RealOCVTable[0][TERMINAL_VOLTAGE_INDEX]){
    cap1 = RealOCVTable[0][cap_Index];
  }else if(real_mVoltage <= RealOCVTable[OCV_TABLE_POINTS - 1][TERMINAL_VOLTAGE_INDEX]){
    cap1 = RealOCVTable[OCV_TABLE_POINTS - 1][cap_Index];
  }else{
    for(i = 1; i < OCV_TABLE_POINTS; i++){
      if(real_mVoltage > RealOCVTable[i][TERMINAL_VOLTAGE_INDEX]){
        if(real_mVoltage == RealOCVTable[i - 1][TERMINAL_VOLTAGE_INDEX]){
          cap1 = RealOCVTable[i - 1][cap_Index];
        }else{
          cap1 = RealOCVTable[i][cap_Index];
          cap1 = RealOCVTable[i - 1][cap_Index] - cap1;
          dv = real_mVoltage - RealOCVTable[i][TERMINAL_VOLTAGE_INDEX];
          dv = dv / (RealOCVTable[i - 1][TERMINAL_VOLTAGE_INDEX] - RealOCVTable[i][TERMINAL_VOLTAGE_INDEX]);
          cap1 =  RealOCVTable[i][cap_Index] + (cap1 * dv);
        }
        break;
      }
    }
  }
  cap = (cap1/100);
  return cap;
}
#else
//用查表法，計算SOC
float getCapacityByTable(int real_mVoltage, unsigned char cap_Index){

  float cap1;
  unsigned char i;
  
  cap1 = 0.0f;
  if(real_mVoltage >= RealOCVTable[0][TERMINAL_VOLTAGE_INDEX]){
    cap1 = RealOCVTable[0][cap_Index];
  }else if(real_mVoltage <= RealOCVTable[OCV_TABLE_POINTS - 1][TERMINAL_VOLTAGE_INDEX]){
    cap1 = RealOCVTable[OCV_TABLE_POINTS - 1][cap_Index];
  }else{
    for(i = 1; i < OCV_TABLE_POINTS; i++){
      if(real_mVoltage > RealOCVTable[i][TERMINAL_VOLTAGE_INDEX]){
        //取較大值
        //cap1 = RealOCVTable[i - 1][cap_Index];
        //取較小值
        cap1 = RealOCVTable[i][cap_Index];
        break;
      }
    }
  }
  cap1 = (cap1/100);
  return cap1;
}
#endif
/*
unsigned int getCapacity1(int mVoltage){
  unsigned int cap;
  float cap1;
  long rm_ADC;
  unsigned char i,j;
  
  cap1 = 0.0f;
  if(mVoltage >= RealOCVTable[0][TERMINAL_VOLTAGE_INDEX]){
    cap1 = RealOCVTable[0][RESIDUAL_CAPACITY_INDEX];
  }else if(mVoltage <= RealOCVTable[OCV_TABLE_POINTS - 1][TERMINAL_VOLTAGE_INDEX]){
    cap1 = RealOCVTable[OCV_TABLE_POINTS - 1][RESIDUAL_CAPACITY_INDEX];
  }else{
    for(i = 0; i < (OCV_TABLE_POINTS - 1); i++){
      if(mVoltage < RealOCVTable[i][TERMINAL_VOLTAGE_INDEX]){
        cap1 = RealOCVTable[i + 1][RESIDUAL_CAPACITY_INDEX];
      }
    }
  }
  cap = cap1/100;
  return cap;
}

*/
