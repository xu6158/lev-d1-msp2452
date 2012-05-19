

//#include "msp430g2452.h"
#include "LEV_PrjDefine.h"

/****************************************************************************\
| Utilize Timer_A to monitor button pressing time                            |
| Timer_A will go to interrupt when TACCR0 exceeded specific time (ex: 50ms) |
\****************************************************************************/


void InitTimerB()
{

  //BCSCTL3 |= LFXT1S_2;                        // ACLK VLO
  TBCCR0 = 564;                               // 50ms; ACLK = ~12KHz; oscilloscope measurement = 11.28KHz
  //TBCTL = TASSEL_2 + MC_1 + TAIE + TACLR;   // SMCLK, upmode, interrupt, reset
  TBCTL = TASSEL_1 + MC_1 + TAIE + TACLR;   // ACLK, upmode, interrupt, reset
  
  TBCCTL1 |= CCIE;      // Enable interrupt, CCR1
  //TBCCTL2 |= CCIE;      // Enable interrupt, CCR2
}

// Timer_B3 Interrupt Vector (TB0IV) handler
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer_B(void)
{
  switch( TB0IV )
  {
   case  2:                                 // CCR1 is used
     //P2OUT ^= LED3;
     break;
   case  4:                                 // CCR2 Not used
     //P2OUT ^= LED4;
     break;
   case 10:                                 // overflow
     P2OUT ^= LED3;
     break;
  }
}


//void OnPress(unsigned int capTimer)
//{
//  //ButtonCapFlag = 0;
//  //OnPressTime = capTimer;
//  //TACCTL1 |= CCIE;      // Enable interrupt, CCR1
//}
//
//void OnLongPress(unsigned int shutdownTimer)
//{
//  //OnLongPressTime = shutdownTimer;
//}
//
//
//
//char IsButtonPressed()
//{
//  return (P1IN & BUTTON_PORT) ? 1 : 0;
//}
//
//void EnableOCPTimer(char isEnabled)
//{
//  OCTimerCapFlag = (isEnabled) ? true : false;
//  Delay_Count = 0;
//}

#if 0

// Timer_A3 Interrupt Vector (TA0IV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
  switch( TA0IV )
  {
   case  2:                                 // CCR1 is used
     if (IsButtonPressed())
     {
       ButtonCapFlag = 1;
       OnPressDelayCounter++;
     }
     else if ( (!(IsButtonPressed())) && 
               (ButtonCapFlag) && 
               (OnPressDelayCounter < BUTTON_PRESSED_ONCE) )
     {
       //P2OUT ^= 0x10;
       OnPressFlag = true;
       OnPressDelayCounter = 0;
       ButtonCapFlag = 0;
     }
     else if (!(IsButtonPressed()))
     {
       ButtonCapFlag = 0;
       OnPressDelayCounter = 0;
       OnPressFlag = false;
     }
     break;
   case  4: break;                          // CCR2 not used
   case 10:                                 // overflow
     TimeCallingFunctionForSOC();
     if ((BAL_FLAG) && (Bal_Delay++ >= DELAY_TIME_10SEC))
     {
       Bal_Delay = 0;
       BAL_FLAG = false;
       DisableAllBalance();
     }
     
     if ((WAKEUP_TIMEOUT) && (Delay_Count++ >= DELAY_TIME_5SEC))
     {
       Delay_Count = 0;
       WAKEUP_TIMEOUT = false;
     }
     
     if ( (OCTimerCapFlag) && (Delay_Count++ >= DELAY_TIME_5SEC) )
     {
       if (COC_FLAG) {
         COC_FLAG = false;
       } else if (DOC_FLAG) {
         DOC_FLAG = false;
       }
       Delay_Count = 0;
       OCTimerCapFlag = false;
     }
     
     if ( IsButtonPressed() && (OnLongPressDelayCounter++ >= BUTTON_PRESSED_4SEC) )
     {
       //P2OUT ^= 0x08;
       OnLongPressFlag = true;
       OnLongPressDelayCounter = 0;
       __bic_SR_register_on_exit(LPM0_bits);
       //__bic_SR_register_on_exit(LPM3_bits);
     }  
     else if (!IsButtonPressed())
     {
       OnLongPressDelayCounter = 0;
       OnLongPressFlag = false;
     }
     
     break;
  }
}


#endif