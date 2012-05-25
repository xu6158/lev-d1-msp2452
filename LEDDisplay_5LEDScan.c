
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
#if _5LEDs_TYPE_ > 0

#define RESET_ALL_LED_PORT  /*clear to output*/ \
  P2DIR |= ALL_LED_PORT; /* Set to output direction*/ \
  P2OUT &= ~ALL_LED_PORT;
#define SHOW_LED12        /*HLH*/ \
  P2OUT |= (LED_PORT1 + LED_PORT3);\
  P2OUT &= ~(LED_PORT2);
#define SHOW_LED34        /*LHL*/ \
  P2OUT &= ~(LED_PORT1 + LED_PORT3);\
  P2OUT |= (LED_PORT2);
#define SHOW_LED2        /*FLH*/ \
  P2DIR &= ~LED_PORT1; /* Set to input direction */ \
  P2OUT &= ~(LED_PORT2);\
  P2OUT |= (LED_PORT3);
#define SHOW_LED4        /*FHL*/ \
  P2DIR &= ~LED_PORT1; /* Set to input direction */ \
  P2OUT |= (LED_PORT2);\
  P2OUT &= ~(LED_PORT3);
#define SHOW_LED5        /*HFL*/ \
  P2OUT |= (LED_PORT1);\
  P2DIR &= ~LED_PORT2; /* Set to input direction */ \
  P2OUT &= ~(LED_PORT3);


#define CAPACITY_1          16
#define CAPACITY_2          37
#define CAPACITY_3          58
#define CAPACITY_4          79

enum For_Show_LED_Code  //unsigned char
{
  No_LED_SHOW,
  SHOW_1_LED_RED,
  SHOW_1_LED_GREEN,
  SHOW_2_LEDS,
  SHOW_3_LEDS,
  SHOW_4_LEDS,
};


static unsigned char Blink_LED_BIT;
static unsigned char ScanFlag;
static unsigned char Show_LED_Codes;
void InitLEDPort()
{

  P2SEL &= ~(ALL_LED_PORT);
  P2DIR |= ALL_LED_PORT;    // Set P2.3, P2.4, P2.5, P2.6 to output direction
  P2OUT &= ~(ALL_LED_PORT);
  ScanFlag = 0;
  Show_LED_Codes = 0;
  Blink_LED_BIT = 0;
  
 
}

//#define CAPACITY_1          16
//#define CAPACITY_2          37
//#define CAPACITY_3          58
//#define CAPACITY_4          79
//unsigned char g_test = 0;
void DisplayCapacity(unsigned char capacity, char isOn)
{

  
  if(isOn)
  {
//    capacity = g_test;
//    g_test += 10;
    
    
    if(capacity <= CAPACITY_1){
      Show_LED_Codes = SHOW_1_LED_RED;
    }else if(capacity <= CAPACITY_2){
      Show_LED_Codes = SHOW_1_LED_GREEN;
    }else if(capacity <= CAPACITY_3){
      Show_LED_Codes = SHOW_2_LEDS;
    }else if(capacity <= CAPACITY_4){
      Show_LED_Codes = SHOW_3_LEDS;
    }else{
      Show_LED_Codes = SHOW_4_LEDS;
    }
    G_Activate_Action_Status |= CAPACITY_DISLALY;
  }
  else
  {
    RESET_ALL_LED_PORT;
    G_Activate_Action_Status &= ~CAPACITY_DISLALY;
  }
}

void Display5LEDsCapacityByScanning(){
  RESET_ALL_LED_PORT;
    switch(Show_LED_Codes){
      case SHOW_1_LED_RED:
        if(ScanFlag & BIT1){
          SHOW_LED5;
        }
        break;
      case SHOW_1_LED_GREEN:
        if(ScanFlag & BIT1){
          SHOW_LED4;
        }
        break;
      case SHOW_2_LEDS:
        if(ScanFlag & BIT1){
          SHOW_LED34;        
        }
        break;
      case SHOW_3_LEDS:
        if(ScanFlag & BIT1){
          SHOW_LED2;        
        }else{
          SHOW_LED34;        
        }
        break;
      case SHOW_4_LEDS:
        if(ScanFlag & BIT1){
          SHOW_LED34;        
        }else{
          SHOW_LED12;
        }
        break;
      default:
        break;
    }
  ScanFlag ^= BIT1;
  
}


void Blink2SeparateLEDs()
{
  if((G_Activate_Action_Status & CAPACITY_DISLALY)==0){
    if(Blink_LED_BIT & BIT0){
      SHOW_LED12;
    }else{
      SHOW_LED34;
    }
  }
  Blink_LED_BIT ^= BIT0;
}
#endif
