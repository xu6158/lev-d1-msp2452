
#include "..\LEV_PrjDefine.h"

//unsigned char InitialFlashContent[Flash_segment_Size];


void WriteDataToFlash(unsigned char Offset_Address, unsigned char *value, unsigned char dataLength, unsigned char SegmentSelection){
//void WriteDataToFlash(unsigned int Offset_Address, unsigned char *value, unsigned char dataLength, unsigned int SegmentSelection){
  unsigned char i;
  unsigned char InitialFlashContent[Flash_segment_Size];
  unsigned char *Initial_Sgement_ptr;

  switch(SegmentSelection){
    case 0:   //config data
      Initial_Sgement_ptr = (unsigned  char *)CONFIG_SEGMENT; // Initialize Flash pointer
      break;
    case 1:   //calibration data
      Initial_Sgement_ptr = (unsigned  char *)CALIBRATION_SEGMENT; // Initialize Flash pointer
      break;
    case 2:   // config data
      Initial_Sgement_ptr = (unsigned  char *)CONFIG_SEGMENT; // Initialize Flash pointer
      break;
    default:
      return;
  }

  //read data from information falsh segment to array
  for(i = 0; i < Flash_segment_Size; i++){
    InitialFlashContent[i] = *(Initial_Sgement_ptr + i);
  }

  //write setting data to array
  for(i = 0; i < dataLength; i++){
    if(Offset_Address + i >= Flash_segment_Size){
      break;
    }
    InitialFlashContent[Offset_Address + i] = *value++;
  }

  //Write array values to flash
  //__disable_interrupt();                    // 5xx Workaround: Disable global
                                            // interrupt while erasing. Re-Enable
                                            // GIE if needed

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

  //_EINT();
}

void ReadInitialDataFromFlash(unsigned char Offset_Address, unsigned int *out_ptr, unsigned char dataLength, unsigned char SegmentSelection){
  //unsigned char InitialFlashContent[Flash_segment_Size];
  switch(SegmentSelection){
    case 0:   //config data
      *out_ptr = CONFIG_SEGMENT + Offset_Address; // Initialize Flash pointer
      break;
    case 1:   //calibration data
      *out_ptr = CALIBRATION_SEGMENT + Offset_Address; // Initialize Flash pointer
      break;
    case 2:   // config data
      *out_ptr = CONFIG_SEGMENT + Offset_Address; // Initialize Flash pointer
      break;
    default:
      return;
  }
}



/*
void ReadInitialDataFromFlash(unsigned char Offset_Address, unsigned int *out_ptr, unsigned char dataLength, unsigned char SegmentSelection){

  //unsigned char i;
  unsigned char *Initial_Sgement_ptr;
  
  switch(SegmentSelection){
    case 0:   //config data
      Initial_Sgement_ptr = (unsigned  char *)CONFIG_SEGMENT; // Initialize Flash pointer
      break;
    case 1:   //calibration data
      Initial_Sgement_ptr = (unsigned  char *)CALIBRATION_SEGMENT; // Initialize Flash pointer
      break;
    case 2:   // config data
      Initial_Sgement_ptr = (unsigned  char *)CONFIG_SEGMENT; // Initialize Flash pointer
      break;
    default:
      return;
  }

  //read data from information falsh segment to array
  //  for(i = 0; i < dataLength; i++){
  //    if(Offset_Address + i >= Flash_segment_Size){
  //      break;
  //    }
  //    *value++ = *(Initial_Sgement_ptr + Offset_Address + i) ;
  //  }
  // *out_ptr = (Initial_Sgement_ptr + Offset_Address);
}


//void FlashWriteSeg(unsigned int seg_addr, char value){
void FlashWriteSeg(unsigned int Flash_Start_Address, char value){

  unsigned int i;
  char * Flash_ptr;                         // Initialize Flash pointer
  Flash_ptr = (char *) Flash_Start_Address;
  __disable_interrupt();                    // 5xx Workaround: Disable global
                                            // interrupt while erasing. Re-Enable
                                            // GIE if needed
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash seg
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

  for (i = 0; i < 128; i++)
  {
    *Flash_ptr++ = value++;                 // Write value to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit
  
  while(FCTL3 & BUSY);
  
  
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

  _EINT();
  
}


void FlashWriteData(unsigned int Flash_Start_Address, char *value, unsigned char dataLength){

  unsigned int i;
  char * Flash_ptr;                         // Initialize Flash pointer
  Flash_ptr = (char *)Flash_Start_Address;
  __disable_interrupt();                    // 5xx Workaround: Disable global
                                            // interrupt while erasing. Re-Enable
                                            // GIE if needed
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash seg
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

  for (i = 0; i < dataLength; i++)
  {
    *Flash_ptr++ = *value++;                 // Write value to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit
  
  while(FCTL3 & BUSY);
  
  
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit

  _EINT();
  
}
*/

