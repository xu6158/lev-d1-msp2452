#include "SOC_OCV_Table.h"

//unsigned char OV_Capacity;
//unsigned char UV_Capacity;
unsigned char getRealCapacityByCell(int mVoltage);
float getCapacityByTable(int mVoltage);

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
unsigned char getRealCapacityByCell(int mVoltage){
  float temp_f2;
  float cap;
  float OV_Capacity;
  float UV_Capacity;
  
  OV_Capacity = getCapacityByTable(SOC_1st_CELL_OV_VOLTAGE);
  UV_Capacity = getCapacityByTable(SOC_1st_CELL_UV_VOLTAGE);
  cap = getCapacityByTable( mVoltage);
  
  temp_f2 = OV_Capacity - UV_Capacity;
  temp_f2 = (cap - UV_Capacity) / temp_f2 * 100;
  return (unsigned char)temp_f2;
}

//用內插法，計算SOC
float getCapacityByTable(int mVoltage){
  float cap;
  float cap1, dv;
  unsigned char i;
  
  cap1 = 0.0f;
  if(mVoltage >= RealOCVTable[0][TERMINAL_VOLTAGE_INDEX]){
    cap1 = RealOCVTable[0][RESIDUAL_CAPACITY_INDEX];
  }else if(mVoltage <= RealOCVTable[OCV_TABLE_POINTS - 1][TERMINAL_VOLTAGE_INDEX]){
    cap1 = RealOCVTable[OCV_TABLE_POINTS - 1][RESIDUAL_CAPACITY_INDEX];
  }else{
    for(i = 1; i < OCV_TABLE_POINTS; i++){
      if(mVoltage > RealOCVTable[i][TERMINAL_VOLTAGE_INDEX]){
        if(mVoltage == RealOCVTable[i - 1][TERMINAL_VOLTAGE_INDEX]){
          cap1 = RealOCVTable[i - 1][RESIDUAL_CAPACITY_INDEX];
        }else{
          cap1 = RealOCVTable[i][RESIDUAL_CAPACITY_INDEX];
          cap1 = RealOCVTable[i - 1][RESIDUAL_CAPACITY_INDEX] - cap1;
          dv = mVoltage - RealOCVTable[i][TERMINAL_VOLTAGE_INDEX];
          dv = dv / (RealOCVTable[i - 1][TERMINAL_VOLTAGE_INDEX] - RealOCVTable[i][TERMINAL_VOLTAGE_INDEX]);
          cap1 =  RealOCVTable[i][RESIDUAL_CAPACITY_INDEX] + (cap1 * dv);
        }
        break;
      }
    }
  }
  cap = (cap1/100);
  return cap;
}


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
