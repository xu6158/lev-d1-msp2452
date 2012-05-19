
#include "LEV_PrjDefine.h"


////////////////////////////////////////////////////////////////////////////
//Input Signal configuration 
// P1.5 PIC_UVP
// P2.0 PIC_OVP
////////////////////////////////////////////////////////////////////////////


void InitInputSignalGetting(){
  // Set up pins used by IO unit
  P1DIR &= ~PIC_UVP_Channel;
  P1SEL &= ~PIC_UVP_Channel;
  
  P2DIR &= ~PIC_OVP_Channel;
  P2SEL &= ~PIC_OVP_Channel;
 
}

unsigned char get_PIC_UVP_Status(){
  return ((P1IN & PIC_UVP_Channel)== 0)? false : true;
}

unsigned char get_PIC_OVP_Status(){
  return ((P2IN & PIC_OVP_Channel)== 0)? false : true;
}

