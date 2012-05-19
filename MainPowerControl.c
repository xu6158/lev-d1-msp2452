

#include "LEV_PrjDefine.h"

/******************************************************************\
| Power control to LDO                                             |
| control channel  #4 (0x40) --> PORT 2.6                          |
|                                                                  |
\******************************************************************/
/*==========================================================\
| MainPowerControl.c                                        |
| #4 P2.6 --> 0x40 (output)                                 |
| (1: turn on Main Power, 0: turn off Main Power )          |
\***********************************************************/
#define MAIN_POWER_CONTROL      BIT6
void InitMainPowerControl();
void SetMainPowerAction( unsigned char enable );

void InitMainPowerControl()
{
  P2SEL &= ~(MAIN_POWER_CONTROL);
  P2DIR |= MAIN_POWER_CONTROL;    // Set P2.6 to output direction
  P2OUT &= ~(MAIN_POWER_CONTROL);        //set channel Low
}

void SetMainPowerAction( unsigned char enable ){
  if(enable){
    P2OUT |= (MAIN_POWER_CONTROL);        //set channel High
  }else{
    P2OUT &= ~(MAIN_POWER_CONTROL);        //set channel Low
  }
  
}
