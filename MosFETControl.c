/*
    MosFET Control configuration 
    P2.1 CHG control
    P2.2 DSG control
    Hi: turn on
    Lo: turn off
*/

#include "LEV_PrjDefine.h"

//static unsigned char CHGflag=0;
//static unsigned char DSGflag=0;

void InitMosControl(){

  // Set up pins used by IO unit
  MOSFET_PORT_SEL &= ~(MOSFET_CHG + MOSFET_DSG);
  MOSFET_PORT_DIR |= MOSFET_CHG + MOSFET_DSG;
  MOSFET_PORT_OUT &= ~(MOSFET_CHG + MOSFET_DSG);
//  CHGflag=0;
//  DSGflag=0;
  
}
void setMosFET(unsigned char MosFet, unsigned char enable){
  if(enable){
    MOSFET_PORT_OUT |= (MosFet);
  }else{
    MOSFET_PORT_OUT &= ~(MosFet);
  }
}
char getMosCHG(){
  return (MOSFET_PORT_OUT & MOSFET_CHG)?true:false;
}
char getMosDSG(){
  return (MOSFET_PORT_OUT & MOSFET_DSG)?true:false;
}

//void setMosCHG(char enable){
//  if(enable){
//    MOSFET_PORT_OUT |= (MOSFET_CHG);
//    CHGflag = enable;
//  }else{
//    MOSFET_PORT_OUT &= ~(MOSFET_CHG);
//    CHGflag = enable;
//  }
//}
//
//void setMosDSG(char enable){
//  if(enable){
//    MOSFET_PORT_OUT |= (MOSFET_DSG);
//    DSGflag = enable;
//  }else{
//    MOSFET_PORT_OUT &= ~(MOSFET_DSG);
//    DSGflag = enable;
//  }
//}
//char getMosCHG(){
//  return CHGflag;
//}
//char getMosDSG(){
//  return DSGflag;
//}
