
//#include "io430.h"
//#include "msp430g2452.h"

#include "LEV_PrjDefine.h"



unsigned char G_uc_SysModeStatusCode;


void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  
  G_uc_SysModeStatusCode = StartUp;

  if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)                                     
  {  
    // If calibration constants erased, do not load, trap CPU!!
    while(1);
    //G_uc_SystemFailureCode = McuCalValueFail;
  } 
  // Set DCOCLK to 1MHz
  DCOCTL = 0x00;
  BCSCTL1 = CALBC1_1MHZ;                  
  DCOCTL = CALDCO_1MHZ;
  // For ACLK Clock
  // LFXT1S_2 : ACLK VLO = ~12KHz
  BCSCTL3 |= LFXT1S_2;     
  
  //delay 20 mS
  __delay_cycles(200000);  // 200ms ==> 1MHz clock
  
//  P2OUT ^= LED4;  
//  __delay_cycles(100000);  // 100.8 ms ==> 1MHz clock
//  __delay_cycles(10000);  // 10.1ms ==> 1MHz clock
//  __delay_cycles(1000);  // 1ms ==> 1MHz clock
//  __delay_cycles(100);  // 106us ==> 1MHz clock
//  __delay_cycles(10);  // 165s ==> 1MHz clock
  
#if _Erase_InformationMemory_At_Flash_segment_C_ > 0
  extern void EraseMemory_Func( );  
  EraseMemory_Func( );
#endif  
  
  FirstInitial_Func(); 
  

  
  while (1)
  {
    switch (G_uc_SysModeStatusCode)
    {
    case StartUp:
      G_uc_SysModeStatusCode = Startup_Func();
      break;
      
    case NormalMode:
      G_uc_SysModeStatusCode = Normal_Func();
      
      //Coulomb Counter Stop
      G_Activate_Action_Status &= ~ACCUMULATING_Q_ENABLE;

      WriteCoulombCounterDataToFlash();
      break;
      
    case FailureMode:
      G_uc_SysModeStatusCode = Failure_Func();
      break;
            
    case ShutdownMode:
      G_uc_SysModeStatusCode = Shutdown_Func();
      break;
      
    case CalibrationMode:
#if _5LEDs_Thick_TYPE_ > 0
      G_Activate_Action_Status_Other1 |= Blink_TWO_LED;
      G_uc_SysModeStatusCode = Calibration_Func();
      G_Activate_Action_Status_Other1 &= ~Blink_TWO_LED;
#else
      setBlinkLED(ALL_LED_PORT, true);;
      G_uc_SysModeStatusCode = Calibration_Func();
      setBlinkLED(ALL_LED_PORT, false);;
#endif      
      break;
//    case ShippingMode:
//      G_uc_SysModeStatusCode = Shipping_Func();
//      break;
    default:
      G_uc_SysModeStatusCode = StartUp;
      break;
    }
  }
  //return 0;
}//main
