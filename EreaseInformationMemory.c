

#include "LEV_PrjDefine.h"

extern void EraseInformationMemory(unsigned char SegmentSelection);
///////////////////////////////////////////////////////////
// Define Flash Segment Address (one segment = 64 bytes)
///////////////////////////////////////////////////////////
//#define Flash_segment_Size 64  // bytes
//#define Flash_segment_A   0x10C0  // protected
//#define Flash_segment_B   0x1080  // for user
//#define Flash_segment_C   0x1040  // can not burning data
//#define Flash_segment_D   0x1000  // has already data inside
//
//#define CONFIG_SEGMENT        Flash_segment_B
//#define CALIBRATION_SEGMENT   Flash_segment_C

void EraseMemory_Func( )
{
//  // Stop watchdog timer to prevent time out reset
//  WDTCTL = WDTPW + WDTHOLD;
//
//  if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)                                     
//  {  
//    // If calibration constants erased, do not load, trap CPU!!
//    //while(1);
//    //G_uc_SystemFailureCode = McuCalValueFail;
//  } 
//  // Set DCOCLK to 1MHz
//  DCOCTL = 0x00;
//  BCSCTL1 = CALBC1_1MHZ;                  
//  DCOCTL = CALDCO_1MHZ;
//  // For ACLK Clock
//  // LFXT1S_2 : ACLK VLO = ~12KHz
//  BCSCTL3 |= LFXT1S_2;     

  EraseInformationMemory(1);  
  //delay 20 mS
  __delay_cycles(200000);  // 200ms ==> 1MHz clock
  while (1);
}



void EraseInformationMemory(unsigned char SegmentSelection){
  unsigned char *Initial_Sgement_ptr;
  unsigned char i;

  switch(SegmentSelection){
    case 0:   //config data
      Initial_Sgement_ptr = (unsigned  char *)CONFIG_SEGMENT; // Initialize Flash pointer
      break;
    case 1:   //calibration data
      Initial_Sgement_ptr = (unsigned  char *)CALIBRATION_SEGMENT; // Initialize Flash pointer
      break;
    default:
      return;
  } 
  
  _DINT();
  
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator

  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *Initial_Sgement_ptr = 0;                 // Dummy write to erase Flash seg
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation

  for (i = 0; i < Flash_segment_Size; i++)
  {
    *Initial_Sgement_ptr++ = 0xff;                 // Write value to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit

  while(FCTL3 & BUSY);

  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit  
  
  _EINT();
  
}