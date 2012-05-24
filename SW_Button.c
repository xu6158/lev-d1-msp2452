
#include "LEV_PrjDefine.h"

////////////////////////////////////////////////////////////////////////////
//  Button configuration
//  Switch P2.7, press = high, releae = low
//  Software Port Interrup
//  A hi/low transition on P1.4 will trigger P1_ISR
////////////////////////////////////////////////////////////////////////////
void InitButton(){
  // Set up pins used by IO unit
  BUTTON_PORT_DIR &= ~SW1_BUTTON;
  BUTTON_PORT_SEL &= ~SW1_BUTTON;
}

void Enable_SW_Interrupt(unsigned char enable){
  if(enable){
    BUTTON_PORT_IES &= ~SW1_BUTTON;    // set button trigger Lo to Hi edge  //press
    //BUTTON_PORT_IES |= SW1_BUTTON;    // set button trigger Hi to Lo edge //release
    BUTTON_PORT_IFG &= ~SW1_BUTTON;    // set button IFG cleared 
    BUTTON_PORT_IE |= SW1_BUTTON;      // set button interrupt enabled
  }else{
    BUTTON_PORT_IE &= ~SW1_BUTTON;      // set button interrupt disabled
  }
}

unsigned char getSW1Status(){
  return ((BUTTON_PORT_IN & SW1_BUTTON)== 0)? false : true;
}


// Port  interrupt service routine
#pragma vector=BUTTON_PORT_VECTOR
__interrupt void Port(void)
{
  BUTTON_PORT_IFG &= ~SW1_BUTTON;    // set button IFG cleared
  
  __delay_cycles(1000);  // 1ms ==> 1MHz clock
  if(getSW1Status()){
    if((BUTTON_PORT_IES & SW1_BUTTON)==0){
     BUTTON_PORT_IES |= SW1_BUTTON;    // set button trigger Hi to Lo edge //release
     G_Activate_Action_Status |= BUTTON_PRESS_FLAG;
    }else{
      BUTTON_PORT_IES &= ~SW1_BUTTON;    // set button trigger Lo to Hi edge  //press
    }
  }
  
  //若 BUTTON_LONG_PRESS_FLAG set, 則 BUTTON_CLICK_FLAG不能被設，
  //因為 BUTTON_LONG_PRESS_FLAG 放開，可能會造成 BUTTON_CLICK
  if(!getSW1Status() && (BUTTON_PORT_IES & SW1_BUTTON)!=0){
    if((G_Activate_Action_Status & BUTTON_PRESS_FLAG)!=0 && (G_Activate_Action_Status & BUTTON_LONG_PRESS_FLAG)==0){
      if(G_Activate_Action_Status_Other1 & Low_Power_Mode){
          ////////////////////////////////////////////////////////////////////////////////////
          __bic_SR_register_on_exit(LPM3_bits);  // Clear LPM0 bits from 3(SR) use in interrupt    
          __delay_cycles(10);
          G_Activate_Action_Status_Other1 &= ~Low_Power_Mode;
          ////////////////////////////////////////////////////////////////////////////////////
      }
      G_Activate_Action_Status |= BUTTON_CLICK_FLAG;
    }
    G_Activate_Action_Status &= ~BUTTON_PRESS_FLAG;
    BUTTON_PORT_IES &= ~SW1_BUTTON;    // set button trigger Lo to Hi edge  //press
  }
     
//  P2OUT ^= LED4;
//  if(P2OUT & LED4){
//    StartAdcConversion();
//    setMosFET(MOSFET_CHG, DeviceOn);
//    setMosFET(MOSFET_DSG, DeviceOff);
//  }else{
//    setMosFET(MOSFET_CHG, DeviceOff);
//    setMosFET(MOSFET_DSG, DeviceOn);
//    StopAdcConversion();
//  }
  
}

