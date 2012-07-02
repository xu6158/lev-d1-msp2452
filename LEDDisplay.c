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
#if _5LEDs_Thick_TYPE_ == 0

// 4 LEDs
#define CAPACITY_1          25
#define CAPACITY_2          50
#define CAPACITY_3          75
//#define CAPACITY_4          100


#define LED_CAPACITY_1                    LED_PORT1  // one LED on
#define LED_CAPACITY_2                    (LED_PORT1+LED_PORT2)  // two LEDs on
#define LED_CAPACITY_3                    (LED_PORT1+LED_PORT2+LED_PORT3)  // three LEDs on
#define LED_CAPACITY_4                    ALL_LED_PORT


unsigned char LED_OUT_BIT;

void InitLEDPort()
{
  P2SEL &= ~(ALL_LED_PORT);
  P2DIR |= ALL_LED_PORT;    // Set P2.3, P2.4, P2.5, P2.6 to output direction
  P2OUT &= ~(ALL_LED_PORT);
  LED_OUT_BIT = 0;
}
void InitBlinkLEDs(){
  int i;
  for(i=0; i<5; i++){
    P2OUT ^= ALL_LED_PORT;
    __delay_cycles(100000);  // 100ms ==> 1MHz clock
  }
  P2OUT &=~ALL_LED_PORT;
  __delay_cycles(100000);  // 100ms ==> 1MHz clock
}

void DisplayCapacity(unsigned char capacity, char isOn)
{
  if(isOn)
  {
    if(capacity <= CAPACITY_1){
      P2OUT |= LED_CAPACITY_1;
    }else if(capacity <= CAPACITY_2){
      P2OUT |= LED_CAPACITY_2;
    }else if(capacity <= CAPACITY_3){
      P2OUT |= LED_CAPACITY_3;
    }else{
      P2OUT |= LED_CAPACITY_4;
    }
    G_Activate_Action_Status |= CAPACITY_DISLALY;
  }
  else
  {
    P2OUT &= ~ALL_LED_PORT;
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

#endif
