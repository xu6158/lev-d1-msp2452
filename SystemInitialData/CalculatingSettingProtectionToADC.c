
#include "SystemInformation.h"
#include "..\LEV_PrjDefine.h"


//Hi 4-bit as 
float temp_f;
unsigned char Cal_Status;
//Function Selection, 1 =DSG Current, 2 =CHG Current, 3 =VBAT
unsigned char Function_Selection_Status;

extern void WriteDataToFlash(unsigned char Offset_Address, unsigned char *value, unsigned char dataLength, unsigned char SegmentSelection);
#if 0
void CalculatingSettingProtectionToADC(){
  unsigned char i;
  unsigned char InitialFlashContent[Flash_segment_Size];
  unsigned char *Initial_Sgement_ptr;
  float ft;
  
  Initial_Sgement_ptr = (unsigned  char *)CALIBRATION_SEGMENT;
  //read data from information falsh segment to array
  for(i = 0; i < Flash_segment_Size; i++){
    InitialFlashContent[i] = *(Initial_Sgement_ptr + i);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // calculation current
  ft = DETECT_CURRENT_OF_DSG_STATUS * DSG_MA_TO_ADC_FACTOR + DSG_OP_ADC_OFFSET;
  InitialFlashContent[ADC_DETECT_CURRENT_OF_DSG_STATUS_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_DETECT_CURRENT_OF_DSG_STATUS_offset + 1] = (unsigned int)ft >> 8;
  ft = DETECT_CURRENT_OF_CHG_STATUS * CHG_MA_TO_ADC_FACTOR + CHG_OP_ADC_OFFSET;
  InitialFlashContent[ADC_DETECT_CURRENT_OF_CHG_STATUS_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_DETECT_CURRENT_OF_CHG_STATUS_offset + 1] = (unsigned int)ft >> 8;
  
//////////////////////////////////////////////////////////////////////////////
// 2012/05/07 Remove DOC Propection, hsinmo
  // calculation current PROTECTION
  ft = DOC_PROTECTION * DSG_MA_TO_ADC_FACTOR * 10 + DSG_OP_ADC_OFFSET;
  InitialFlashContent[ADC_DOC_PROTECTION_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_DOC_PROTECTION_offset + 1] = (unsigned int)ft >> 8;
  ft = COC_PROTECTION * CHG_MA_TO_ADC_FACTOR * 10 + CHG_OP_ADC_OFFSET;
  InitialFlashContent[ADC_COC_PROTECTION_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_COC_PROTECTION_offset + 1] = (unsigned int)ft >> 8;
  
  // calculation Voltage
  ft = _2ND_BATTERY_OV_PROTECTION * VBAT_10MV_TO_ADC_FACTOR + VBAT_ADC_OFFSET;
  InitialFlashContent[ADC_2ND_BATTERY_OV_PROTECTION_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_2ND_BATTERY_OV_PROTECTION_offset + 1] = (unsigned int)ft >> 8;
  ft = _2ND_BATTERY_UV_PROTECTION * VBAT_10MV_TO_ADC_FACTOR + VBAT_ADC_OFFSET;
  InitialFlashContent[ADC_2ND_BATTERY_UV_PROTECTION_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_2ND_BATTERY_UV_PROTECTION_offset + 1] = (unsigned int)ft >> 8;
  
  ft = _2ND_BATTERY_OV_RELEASE * VBAT_10MV_TO_ADC_FACTOR + VBAT_ADC_OFFSET;
  InitialFlashContent[ADC_2ND_BATTERY_OV_RELEASE_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_2ND_BATTERY_OV_RELEASE_offset + 1] = (unsigned int)ft >> 8;
  ft = _2ND_BATTERY_UV_RELEASE * VBAT_10MV_TO_ADC_FACTOR + VBAT_ADC_OFFSET;
  InitialFlashContent[ADC_2ND_BATTERY_UV_RELEASE_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_2ND_BATTERY_UV_RELEASE_offset + 1] = (unsigned int)ft >> 8;

  ft = CHG_CV_MODE_LIMIT_VOLTAGE * VBAT_10MV_TO_ADC_FACTOR + VBAT_ADC_OFFSET;
  InitialFlashContent[ADC_CHG_CV_MODE_LIMIT_VOLTAGE_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_CHG_CV_MODE_LIMIT_VOLTAGE_offset + 1] = (unsigned int)ft >> 8;
  
  
  
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // NTC Thermistor setting by voltage
  ft = DSG_OT_PROTECTION_VOLTAGE * Thermistor_mV_To_ADC_Factor;
  InitialFlashContent[ADC_DSG_OT_PROTECTION_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_DSG_OT_PROTECTION_offset + 1] = (unsigned int)ft >> 8;
  ft = DSG_OT_RELEASE_VOLTAGE * Thermistor_mV_To_ADC_Factor;
  InitialFlashContent[ADC_DSG_OT_RELEASE_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_DSG_OT_RELEASE_offset + 1] = (unsigned int)ft >> 8;
  
  ft = CHG_OT_PROTECTION_VOLTAGE * Thermistor_mV_To_ADC_Factor;
  InitialFlashContent[ADC_CHG_OT_PROTECTION_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_CHG_OT_PROTECTION_offset + 1] = (unsigned int)ft >> 8;
  ft = CHG_OT_RELEASE_VOLTAGE * Thermistor_mV_To_ADC_Factor;
  InitialFlashContent[ADC_CHG_OT_RELEASE_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_CHG_OT_RELEASE_offset + 1] = (unsigned int)ft >> 8;

  ft = UT_PROTECTION_VOLTAGE * Thermistor_mV_To_ADC_Factor;
  InitialFlashContent[ADC_UT_PROTECTION_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_UT_PROTECTION_offset + 1] = (unsigned int)ft >> 8;
  
  ft = UT_RELEASE_VOLTAGE * Thermistor_mV_To_ADC_Factor;
  InitialFlashContent[ADC_UT_RELEASE_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_UT_RELEASE_offset + 1] = (unsigned int)ft >> 8;
  ///////////////////////////////////////////////////////////////////////////////////////////////
  ft = CYCLECOUNT_DSG_CAP_THRESHOLD * CHG_MA_TO_ADC_FACTOR + CHG_OP_ADC_OFFSET;
  InitialFlashContent[ADC_CYCLECOUNT_THRESHOLD_offset] = (unsigned int)ft;
  InitialFlashContent[ADC_CYCLECOUNT_THRESHOLD_offset + 1] = (unsigned int)ft >> 8;
  
  ///////////////////////////////////////////////////////////////////////////////////////////////
  //Write array values to flash
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *Initial_Sgement_ptr = 0;                 // Dummy write to erase Flash seg
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation
  for (i = 0; i < Flash_segment_Size; i++)
  {
    *Initial_Sgement_ptr++ = InitialFlashContent[i];                 // Write value to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit
  while(FCTL3 & BUSY);
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit
  ///////////////////////////////////////////////////////////////////////////////////////////////
  
  
//  ft = ADC_DETECT_CURRENT_OF_DSG_STATUS;
//  ft = ADC_DETECT_CURRENT_OF_CHG_STATUS;
//  ft = ADC_DOC_PROTECTION;
//  ft = ADC_COC_PROTECTION;
//  ft = ADC_2ND_BATTERY_OV_PROTECTION;
//  ft = ADC_2ND_BATTERY_UV_PROTECTION;
 
}


void SetCalibrationData(unsigned char Function_Selection, unsigned char Cal_point, unsigned int present_Values){
//Function Selection, 1 =DSG Current, 2 =CHG Current, 3 =VBAT
  if(Function_Selection_Status != Function_Selection){
    temp_f = 0.0f;
    Cal_Status = 0;
    Function_Selection_Status = Function_Selection;
  }
  switch(Function_Selection_Status){
    case 0x01:  //DSG Current
      temp_f += (float)GetADCValue(IDSG_ADC)/ present_Values;
      Cal_Status++;
      break;
    case 0x02:  //CHG Current
      temp_f += (float)GetADCValue(ICHG_ADC)/ present_Values;
      Cal_Status++;
      break;
    case 0x03:  //VBAT
      temp_f += (float)GetADCValue(Vbat_ADC)/ present_Values;
      Cal_Status++;
      break;
    default:
      temp_f = 0.0f;
      Cal_Status = 0;
      Function_Selection_Status = 0;
      break;
  }
  //SegmentSelection, 0 =Flash_segment_B(0x1080,CONFIG_SEGMENT), 1 =Flash_segment_C(0x1040,CALIBRATION_SEGMENT)
  if(Cal_Status == 2){
    temp_f = temp_f / 2;
    switch(Function_Selection_Status){
      case 0x01:  //DSG Current
        WriteDataToFlash(DSG_MA_TO_ADC_FACTOR_offset, (unsigned char *)&temp_f, 4, 0);
        break;
      case 0x02:  //CHG Current
        WriteDataToFlash(CHG_MA_TO_ADC_FACTOR_offset, (unsigned char *)&temp_f, 4, 0);
        break;
      case 0x03:  //VBAT
        WriteDataToFlash(VBAT_10MV_TO_ADC_FACTOR_offset, (unsigned char *)&temp_f, 4, 0);
        break;
      default:
        break;
    }  
  }
}

#endif
