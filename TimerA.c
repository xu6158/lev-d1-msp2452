//#include "msp430g2452.h"
#include "LEV_PrjDefine.h"

/****************************************************************************\
| Utilize Timer_A to monitor button pressing time                            |
| Timer_A will go to interrupt when TACCR0 exceeded specific time (ex: 50ms) |
\****************************************************************************/

//#define _half_1_second_count_     10  // 10 * 50ms = 500ms = 0.5 sec
//#define _1_second_count_          20  // 20 * 50ms = 1000ms = 1 sec
#if _5LEDs_No_Separate_Display_ == 0 
#define _half_1_second_count_     50  // 50 * 10ms = 500ms = 0.5 sec
#define _1_second_count_          100  // 100 * 10ms = 1000ms = 1 sec
#else
#define _half_1_second_count_     100  // 100 * 5ms = 500ms = 0.5 sec
#define _1_second_count_          200  // 200 * 5ms = 1000ms = 1 sec
#endif
//////////////////////////////////////////////////////////////////////////////
unsigned char Half_Timer_Counter;
unsigned char Timer_Counter;
unsigned char Button_Timer_Counter;
//////////////////////////////////////////////////////////////////////////////
// 2012/05/07 Remove DOC Propection, hsinmo
unsigned char OC_Timer_Counter;
unsigned int CHG_CV_MODE_Timer_Counter;
unsigned int WAKE_UP_Timer_Counter;
unsigned int Suspend_Timer_Counter;
unsigned int Suspend_WAKE_UP_Timer_Counter;
unsigned char Display_Capacity_Timer_Counter;
void InitTimerA()
{
  //BCSCTL3 |= LFXT1S_2;                     // ACLK VLO
  //TACCR0 = 624;                              // 50ms; ACLK = ~12KHz; oscilloscope measurement = 11.28KHz
  //TACCR0 = 620;                              // 50ms; ACLK = ~12KHz; oscilloscope measurement = 11.28KHz

#if _5LEDs_No_Separate_Display_ == 0 
  TACCR0 = 120;                              // 10ms; ACLK = ~12KHz; oscilloscope measurement = 11.28KHz
#else
  TACCR0 = 60;                              // 5ms; ACLK = ~12KHz; oscilloscope measurement = 11.28KHz
#endif

  //TACTL = TASSEL_2 + MC_1 + TAIE + TACLR;  // SMCLK, upmode, interrupt, reset
  TACTL = TASSEL_1 + MC_1 + TAIE + TACLR;    // ACLK, upmode, interrupt, reset
  
  TACCTL1 |= CCIE;      // Enable interrupt, CCR1
  //TACCTL2 |= CCIE;      // Enable interrupt, CCR2
  Half_Timer_Counter = 0;
  Timer_Counter = 0;
  Button_Timer_Counter = 0;
//////////////////////////////////////////////////////////////////////////////
// 2012/05/07 Remove DOC Propection, hsinmo
  OC_Timer_Counter = 0;
  CHG_CV_MODE_Timer_Counter = 0;
  WAKE_UP_Timer_Counter = 0;
  Suspend_Timer_Counter = 0;
  Suspend_WAKE_UP_Timer_Counter = 0;
  Display_Capacity_Timer_Counter = 0;
}

void TimerAStop(){
  TACTL = TASSEL_1 + MC_0 + TACLR;    // ACLK, upmode, interrupt, reset
  TACCTL1 &= ~CCIE;      // disEnable interrupt, CCR1
}

// Timer_A3 Interrupt Vector (TA0IV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
  switch( TA0IV )
  {
   case  2:                                 // CCR1 is used
     if(getSW1Status()){
     }
     break;
   case  4:                                 // CCR2 Not used
     break;
   case 10:                                 // overflow
#if _5LEDs_Thick_TYPE_ > 0
     if(G_Activate_Action_Status & CAPACITY_DISLALY){
       Display5LEDsCapacityByScanning();
    }
    LightOffSequenceByScanning();
    LightOnSequenceByScanning();
#endif       
    //=========================================================================
    // each Half second into once 
    if(Half_Timer_Counter >= _half_1_second_count_){
      Half_Timer_Counter = 0;
#if _5LEDs_Thick_TYPE_ > 0
      if(G_Activate_Action_Status_Other1 & Blink_TWO_LED){
        Blink2SeparateLEDs();
      }
#else
      BlinkLEDs();
#endif       
      
      if(G_Activate_Action_Status & ACCUMULATING_Q_ENABLE){
        AccumulatingQ();
      }

      //suspend mode weak up
      if((G_Activate_Action_Status & ENABLE_SUSPEND_WAKE_UP_COUNTER) &&((G_Activate_Action_Status & SUSPEND_WAKE_UP_COUNTING_FINISH)==0)){
        if(Suspend_WAKE_UP_Timer_Counter < (WAKE_UP_TIME_FROM_SUSPEND_MODE << 1)){
          Suspend_WAKE_UP_Timer_Counter++;
        }else{
          Suspend_WAKE_UP_Timer_Counter = 0;
          G_Activate_Action_Status |= SUSPEND_WAKE_UP_COUNTING_FINISH;
          ////////////////////////////////////////////////////////////////////////////////////
          __bic_SR_register_on_exit(LPM3_bits);  // Clear LPM0 bits from 3(SR) use in interrupt    
          __delay_cycles(10);
          G_Activate_Action_Status_Other1 &= ~Low_Power_Mode;
          ////////////////////////////////////////////////////////////////////////////////////
        }
      }else{
        Suspend_WAKE_UP_Timer_Counter = 0;
      }
      
      //button press long
      if(getSW1Status()){
        if((G_Activate_Action_Status & BUTTON_LONG_PRESS_FLAG)==0){
          //G_Activate_Action_Status |= BUTTON_PRESS_FLAG;
          Button_Timer_Counter++;
        }
      }else{
        Button_Timer_Counter = 0;
      }
      if(Button_Timer_Counter >= (BUTTON_PRESS_TIME<<1)){
        G_Activate_Action_Status |= BUTTON_LONG_PRESS_FLAG;
        Button_Timer_Counter = 0;
      }
      
      //display capacity
      if(G_Activate_Action_Status & CAPACITY_DISLALY){
        Display_Capacity_Timer_Counter++;
      }else{
        Display_Capacity_Timer_Counter=0;
      }
      if(Display_Capacity_Timer_Counter >= (DisplayCAPACITY_TIME<<1)){
        //off capacity display
        G_Activate_Action_Status &= ~CAPACITY_DISLALY;
        DisplayCapacity(ALL_LED_PORT, false);
      }
    }//if(Half_Timer_Counter >= _half_1_second_count_){
    Half_Timer_Counter++;
    
//    if(G_uc_SysModeStatusCode <= StartUp){
//      break;
//    }
    //=========================================================================
    
    //=========================================================================
    ///////////////////////////////////////////////////////////////////////////
    // each one second into once
    if(Timer_Counter >= _1_second_count_){
      Timer_Counter = 0;
      

      
      if(G_uc_SysModeStatusCode == NormalMode){// || G_uc_SysModeStatusCode == FailureMode){
        //into suspend mode
        if((G_Activate_Action_Status & ENABLE_SUSPEND_COUNTER) &&((G_Activate_Action_Status & SUSPEND_COUNTING_FINISH)==0)){
          if(Suspend_Timer_Counter < FIRST_TO_SUSPEND_MODE_IN_RELEASE_TIME){
            Suspend_Timer_Counter++;
          }else{
            Suspend_Timer_Counter = 0;
            G_Activate_Action_Status |= SUSPEND_COUNTING_FINISH;
          }
        }else{
          Suspend_Timer_Counter = 0;
        }
        
//////////////////////////////////////////////////////////////////////////////
// 2012/05/07 Remove DOC Propection, hsinmo
        //over current counter
        if(G_Activate_Action_Status & ENABLE_OC_COUNTER){
          if(OC_Timer_Counter < OC_PROTECTION_RELEASE_TIME){
            OC_Timer_Counter++;
          }else{
            G_Activate_Action_Status |= OC_COUNTING_FINISH;
          }
        }else{
          OC_Timer_Counter = 0;
        }
        
//        //CHG CV-MODE Release Counter
//        if((G_Activate_Action_Status & CHG_CV_MODE_START)&&(G_Activate_Action_Status & ENABLE_CV_MODE_COUNTER)){
//          if(CHG_CV_MODE_Timer_Counter < CHG_CV_MODE_RELEASE_WAIT_TIME){
//            CHG_CV_MODE_Timer_Counter++;
//          }else{
//            G_Activate_Action_Status |= CV_MODE_RELEASE_COUNTING_FINISH;
//          }
//        }else{
//         CHG_CV_MODE_Timer_Counter = 0; 
//        }
      }//if(G_uc_SysModeStatusCode == NormalMode || G_uc_SysModeStatusCode == FailureMode){

//      if(G_uc_SysModeStatusCode == ShippingMode){
//        //WAKE UP counter
//        if(G_Activate_Action_Status & ENABLE_WAKE_UP_COUNTER){
//          if(WAKE_UP_Timer_Counter >= WAKE_UP_TIME_FROM_SHIP_MODE){
//            G_Activate_Action_Status |= WAKE_UP_COUNTING_FINISH;
//            WAKE_UP_Timer_Counter = 0;
//            G_Activate_Action_Status &= ~ENABLE_WAKE_UP_COUNTER;
//            __bic_SR_register_on_exit(LPM3_bits);  // Clear LPM3 bits from 0(SR) use in interrupt
//          }else{
//            WAKE_UP_Timer_Counter++;
//          }
//        }
//      }//if(G_uc_SysModeStatusCode == ShippingMode)
      
      
    }//if(Timer_Counter >= _1_second_count_){
    Timer_Counter++;
    //=========================================================================

     break;
  }
}

//// Timer_A3 Interrupt Vector (TA0IV) handler
//#pragma vector=TIMER0_A1_VECTOR
//__interrupt void Timer_A(void)
//{
//  switch( TA0IV )
//  {
//   case  2:                                 // CCR1 is used
//     if(getSW1Status()){
//       P2OUT ^= LED3;
//     }
//     //P2OUT ^= LED3;
//     break;
//   case  4:                                 // CCR2 Not used
//     //P2OUT ^= LED4;
//     break;
//   case 10:                                 // overflow
//   //P2OUT ^= LED4;
//    if(Half_Timer_Counter >= _half_1_second_count_){
//      Half_Timer_Counter = 0;
//      BlinkLEDs();
//    }
//    Half_Timer_Counter++;
//    
//    if(G_uc_SysModeStatusCode <= StartUp){
//      break;
//    }
//    
//    // each one second into once
//    if(Timer_Counter >= _1_second_count_){
//      Timer_Counter = 0;
//
//      if(G_uc_SysModeStatusCode == NormalMode || G_uc_SysModeStatusCode == FailureMode){
//        //button press long
//        if(getSW1Status() && !(G_Activate_Action_Status & BUTTON_LONG_PRESS_FLAG)){
//          G_Activate_Action_Status |= BUTTON_PRESS_FLAG;
//          Button_Timer_Counter++;
//        }else{
//          Button_Timer_Counter = 0;
//        }
//        if(Button_Timer_Counter >= BUTTON_PRESS_TIME){
//          G_Activate_Action_Status |= BUTTON_LONG_PRESS_FLAG;
//          Button_Timer_Counter = 0;
//        }
//        
//        //over current counter
//        if(G_Activate_Action_Status & ENABLE_OC_COUNTER){
//          if(OC_Timer_Counter < OC_PROTECTION_RELEASE_TIME){
//            OC_Timer_Counter++;
//          }else{
//            G_Activate_Action_Status |= OC_COUNTING_FINISH;
//          }
//        }else{
//          OC_Timer_Counter = 0;
//        }
//        
//        //CHG CV-MODE Release Counter
//        if((G_Activate_Action_Status & CHG_CV_MODE_START)&&(G_Activate_Action_Status & ENABLE_CV_MODE_COUNTER)){
//          if(CHG_CV_MODE_Timer_Counter < CHG_CV_MODE_RELEASE_WAIT_TIME){
//            CHG_CV_MODE_Timer_Counter++;
//          }else{
//            G_Activate_Action_Status |= CV_MODE_RELEASE_COUNTING_FINISH;
//          }
//        }else{
//         CHG_CV_MODE_Timer_Counter = 0; 
//        }
//      }//if(G_uc_SysModeStatusCode == NormalMode || G_uc_SysModeStatusCode == FailureMode){
//
//      if(G_uc_SysModeStatusCode == ShippingMode){
//        //WAKE UP counter
//        if(G_Activate_Action_Status & ENABLE_WAKE_UP_COUNTER){
//          if(WAKE_UP_Timer_Counter >= WAKE_UP_TIME_FROM_SHIP_MODE){
//            G_Activate_Action_Status |= WAKE_UP_COUNTING_FINISH;
//            WAKE_UP_Timer_Counter = 0;
//            G_Activate_Action_Status &= ~ENABLE_WAKE_UP_COUNTER;
//            __bic_SR_register_on_exit(LPM3_bits);  // Clear LPM3 bits from 0(SR) use in interrupt
//          }else{
//            WAKE_UP_Timer_Counter++;
//          }
//        }
//      }//if(G_uc_SysModeStatusCode == ShippingMode)
//      
//      
//    }//if(Timer_Counter >= _1_second_count_){
//    Timer_Counter++;
//
//     break;
//  }
//}


