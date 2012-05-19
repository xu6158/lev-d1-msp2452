//#include "msp430g2452.h"
#include "LEV_PrjDefine.h"

/******************************************************************\
| LEV D1 has 4 LEDs                                                |
| LED #1 (0x08) --> PORT 2.3                                       |
| LED #2 (0x10) --> PORT 2.4                                       |
| LED #3 (0x20) --> PORT 2.5                                       |
| LED #4 (0x40) --> PORT 2.6 // reserved                           |
|                                                                  |
|         STATUS       |   LED1   |   LED2   |   LED3   |   LED4   |
| ---------------------|----------|----------|----------|--------- |
|  Initialization      |    ON    |    ON    |    ON    |   ON     |
|  Normal Mode         |    OFF   |    OFF   |    OFF   |   OFF    |
|  Failure Mode        |    OFF   |    ON    |    OFF   |   ON     |
|  Shutdown Mode       |    OFF   |    OFF   |    OFF   |   OFF    |
|  System Error        |  ON-OFF  |  ON-OFF  |  ON-OFF  |  ON-OFF  |
|  Batt Capacity (100%)|    ON    |    ON    |    ON    |   ON     |
|  Batt Capacity (75%) |    ON    |    ON    |    ON    |   OFF    |
|  Batt Capacity (50%) |    ON    |    ON    |    OFF   |   OFF    |
|  Batt Capacity (25%) |    ON    |    OFF   |    OFF   |   OFF    |
\******************************************************************/

unsigned char LED_OUT_BIT;
void InitLEDPort()
{
  P2SEL &= ~(LED_SET_ALL);
  P2DIR |= LED_SET_ALL;    // Set P2.3, P2.4, P2.5, P2.6 to output direction
  P2OUT &= ~(LED_SET_ALL);
  LED_OUT_BIT = 0;
}

void DisplayCapacity(unsigned char capacity, char isOn)
{
  if(isOn)
  {
    if(capacity <= CAPACITY_1){
      P2OUT = LED_CAPACITY_1;
    }else if(capacity <= CAPACITY_2){
      P2OUT = LED_CAPACITY_2;
    }else if(capacity <= CAPACITY_3){
      P2OUT = LED_CAPACITY_3;
    }else{
      P2OUT = LED_CAPACITY_4;
    }
    G_Activate_Action_Status |= CAPACITY_DISLALY;
  }
  else
  {
    P2OUT &= ~LED_SET_ALL;
    G_Activate_Action_Status &= ~CAPACITY_DISLALY;
  }
}
void DisplayLED(unsigned char LedNumber, char isOn)
{  
  if(isOn)
  {
    P2OUT |= LedNumber;
  }
  else
  {
    P2OUT &= ~LedNumber;
  }
}
void setBlinkLED(unsigned char LedNumber, char isOn)
{  
  if(isOn)
  {
    LED_OUT_BIT |= LedNumber;
  }
  else
  {
    P2OUT &= ~LedNumber;  //clear
    LED_OUT_BIT &= ~LedNumber;
  }
}
void BlinkLEDs()
{
  P2OUT ^= LED_OUT_BIT;
}

