

//#include "io430.h"
#include <msp430g2452.h>
#include "LEV_CommonDefine.h"
#include "SystemInitialData\SystemInformation.h"


//extern unsigned char gTempChar64[64];


/***********************************************************\
| LEDDisplay.c                                              |
| #1 P2.3 --> 0x08                                          |
| #2 P2.4 --> 0x10                                          |
| #3 P2.5 --> 0x20                                          |
| #4 P2.6 --> 0x40                                          |
\***********************************************************/
#define LED1                              BIT3  //red
#define LED2                              BIT4
#define LED3                              BIT5
#define LED4                              BIT6
#define LED_SET_ALL                       (LED1+LED2+LED3+LED4)
#define LED_CAPACITY_1                    LED1  // one LED on
#define LED_CAPACITY_2                    (LED1+LED2)  // two LEDs on
#define LED_CAPACITY_3                    (LED1+LED2+LED3)  // three LEDs on
#define LED_CAPACITY_4                    LED_SET_ALL
#define DisplayCAPACITY_TIME              2 //sec
#define CAPACITY_1          25
#define CAPACITY_2          50
#define CAPACITY_3          75
//#define CAPACITY_4          74

//#define LED_CAPACITY_25                   LED1  // one LED on
//#define LED_CAPACITY_50                   (LED1+LED2)  // two LEDs on
//#define LED_CAPACITY_75                   (LED1+LED2+LED3)  // three LEDs on
//#define LED_CAPACITY_100                  LED_SET_ALL

void InitLEDPort();
void setBlinkLED(unsigned char LedNumber, char isOn);
void DisplayLED(unsigned char LedNumber, char isOn);
void DisplayCapacity(unsigned char capacity, char isOn);

void BlinkLEDs();

#define SystemFailBlinkLED      LED1
#define OC_BlinkLED             LED2
#define OV_UV_BlinkLED          LED3
//#define OT_UT_BlinkLED          LED4
#define OT_UT_BlinkLED          LED3


/************************************************************\
| Timer.c                                                   |
| Button Port: P1.4                                          |
| Timer overflow: 50ms                                       |
\************************************************************/
#define BUTTON_PORT                       0x10  //P1.4
#define BUTTON_PRESSED_4SEC               80
#define BUTTON_PRESSED_ONCE               30
#define DELAY_TIME_5SEC                   100  //delay 5sec
#define DELAY_TIME_10SEC                  200  //delay 10sec
#define OC_RETRY                          3
void InitTimerA();
void InitTimerB();
//char IsButtonPressed();
//void ResetOnPress();
//void ResetOnLongPress();
//void EnableOCPTimer(char);
//void settf();
//void distf();

/************************************************************\
| AdcReader.c                                               
| ADC Port: 
| P1.0:A0: Vbat, 
| P1.1:A1: T1,
| P1.2:A2: I_CHG,   
| P1.3:A3: I_DSG,
\************************************************************/
enum For_ADC_Channel  //unsigned char
{
  IDSG_ADC,  //A3, first in data buffer
  ICHG_ADC, //A2, second in data buffer
  T1_ADC,   //A1, 4th in data buffer 
  Vbat_ADC  //A0, last in data buffer
};

//void InitAdcReader();
//void OffAdcReader();
void StartAdcConversion();
//void StopAdcConversion();
unsigned int GetADCValue(unsigned char ADC_Channel);

////////////////////////////////////////////////////////////////////////////
//  
//  Button configuration
//  Switch P2.3, press = high, releae = low
////////////////////////////////////////////////////////////////////////////
#define SW1_BUTTON              BIT7
#define BUTTON_PORT_IN          P2IN
#define BUTTON_PORT_DIR			    P2DIR
#define BUTTON_PORT_SEL			    P2SEL
#define BUTTON_PORT_IE          P2IE
#define BUTTON_PORT_IES         P2IES
#define BUTTON_PORT_IFG         P2IFG
#define BUTTON_PORT_VECTOR      PORT2_VECTOR

/*
#define SW1_BUTTON              BIT4
#define BUTTON_PORT_IN          P1IN
#define BUTTON_PORT_DIR			    P1DIR
#define BUTTON_PORT_SEL			    P1SEL
#define BUTTON_PORT_IE          P1IE
#define BUTTON_PORT_IES         P1IES
#define BUTTON_PORT_IFG         P1IFG
#define BUTTON_PORT_VECTOR      PORT1_VECTOR
*/

void InitButton();
void Enable_SW_Interrupt(unsigned char enable);
unsigned char getSW1Status();

////////////////////////////////////////////////////////////////////////////
//MosFET Control configuration 
// P9.7 CHG control
// P9.6 DSG control
////////////////////////////////////////////////////////////////////////////
#define MOSFET_PORT_REN      P2REN  //1: Pullup/pulldown resistor enabled, 0: Pullup/pulldown resistor disabled
#define MOSFET_PORT_OUT      P2OUT  //1: The output is high or Pullup, 0: The output is low or pulldown
#define MOSFET_PORT_DIR      P2DIR  //1: The port pin is switched to output direction, 0: The port pin is switched to input direction
//  PxSEL2 PxSEL  Pin Function
//    0     0     I/O function is selected.
//    0     1     Primary peripheral module function is selected.
//    1     0     Reserved. See device-specific data sheet.
//    1     1     Secondary peripheral module function is selected.
#define MOSFET_PORT_SEL      P2SEL
#define MOSFET_PORT_SEL2     P2SEL2

#define MOSFET_CHG            BIT1
#define MOSFET_DSG            BIT2
void InitMosControl();
//void setMosCHG(char enable);
//void setMosDSG(char enable);
void setMosFET(unsigned char MosFet, unsigned char enable);
char getMosCHG();
char getMosDSG();


////////////////////////////////////////////////////////////////////////////
//Input Signal configuration 
// P1.5 PIC_UVP
// P2.0 PIC_OVP
////////////////////////////////////////////////////////////////////////////
#define PIC_UVP_Channel   BIT5
#define PIC_OVP_Channel   BIT0

void InitInputSignalGetting();
unsigned char get_PIC_UVP_Status();
unsigned char get_PIC_OVP_Status();


////////////////////////////////////////////////////////////////////////////
//Read/Write Information Flash Memory functions
//
////////////////////////////////////////////////////////////////////////////
//extern unsigned char InitialFlashContent[];

void WriteDataToFlash(unsigned char Offset_Address, unsigned char *value, unsigned char dataLength, unsigned char SegmentSelection);
//void ReadInitialDataFromFlash(unsigned char Offset_Address, unsigned char *value, unsigned char dataLength, unsigned char SegmentSelection);
void ReadInitialDataFromFlash(unsigned char Offset_Address, unsigned int *out_ptr, unsigned char dataLength, unsigned char SegmentSelection);



/************************************************************\
| IIC_Slave.c                                               |
| IIC Port: P1.6 and P1.7                                    |
\************************************************************/
//byte, IIC Status Code
#define ADDRESS_NOT_MATCH				  (0x01)    //
#define SLV_TRANSMIT    				  (0x02)    //
#define SLV_RECEIVE_CMD_DONE      (0x04)    //RECEIVED CMD has been calculated done.
#define NoUs2        				      (0x08)    //No Use,
#define NoUs3        				      (0x10)    //No Use,
#define NoUs4        				      (0x20)    //No Use,
#define NoUs5        				      (0x40)    //No Use,
#define NoUs6        				      (0x80)    //No Use,


#define SlaveAddress7Bit    0x48
#define Number_of_Bytes  10         // **** How many bytes?? for Rx Tx buffer array ****


void Setup_USI_Slave();

////////////////////////////////////////////
// ADC / Protection transform
////////////////////////////////////////////
void CalculatingSettingProtectionToADC();
void SetCalibrationData(unsigned char Function_Selection, unsigned char Cal_point, unsigned int present_Values);

////////////////////////////////////////////
// SOC Function
////////////////////////////////////////////
unsigned char getRealCapacityByCell(int ADC_Voltage, int ADC_current);


////////////////////////////////////////////
// Coulomb Counter Function
////////////////////////////////////////////
void InitCoulombCounter();
void AccumulatingQ();
void WriteCoulombCounterDataToFlash();

