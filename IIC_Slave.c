
#include "LEV_PrjDefine.h"
#include "IIC_CommonDefine.h"
//#include "SystemInformation.h"

int I2C_State, Bytecount;     // State variables

unsigned char SLV_RxTxBuffer[Number_of_Bytes];
unsigned char *SLV_Tx_Pointer;
unsigned char RxCMDLength;
unsigned char RxCount;
unsigned char IIC_SLV_Status;
////////////////////////////////////////

#if 0

#define IIC_Trace_arryaL  100
enum For_IICTrace_value  //unsigned char
{
  c0,
  c2,
  c4,
  c6,
  c8,
  c10,
  c12,
  c14,
  c4_t0,
  c4_t1,
  c4_matchAdd,
  c4_nomatchAdd,
  c8_notLast,
  c8_Last,
  c14_NackRec,
  c14_AckRec
};
unsigned char IIC_Trace[IIC_Trace_arryaL];
unsigned char IICTraceCount = 0;
void setTrace(unsigned char value){
  if(IICTraceCount >= IIC_Trace_arryaL){
    return;
  }
  IIC_Trace[IICTraceCount] = value;
  IICTraceCount++;
}
#endif


void Setup_USI_Slave(){
  P1DIR |= 0xC0;                             // set pins as outputs
  P1REN |= 0xC0;                            // P1.6 & P1.7 Pullups
  P1OUT |= 0xC0;                             // P1.6 & P1.7 out
  P1SEL |= 0xC0;                             // P1.6 & P1.7 

  
  USICTL0 = USIPE6+USIPE7+USISWRST;         // Port & USI mode setup
  USICTL1 = USII2C+USIIE+USISTTIE;          // Enable I2C mode & USI interrupts
  USICKCTL = USICKPL;                       // Setup clock polarity
  USICNT |= USIIFGCC;                       // Disable automatic clear control
  USICTL0 &= ~USISWRST;                     // Enable USI
  USICTL1 &= ~USIIFG;                       // Clear pending flag
  
  IIC_SLV_Status = 0;
  RxCount = 0;
  SLV_Tx_Pointer = SLV_RxTxBuffer;

}

//void Data_RX(){
//  
//              USICTL0 &= ~USIOE;            // SDA = input
//              USICNT |=  0x08;              // Bit counter = 8, RX data
//              I2C_State = 8;                // next state: Test data and (N)Ack
//}
//
void TX_Data(){
              USICTL0 |= USIOE;             // SDA = output
              //USISRL = SLV_Data++;
              //USISRL = SLV_RxTxBuffer[Bytecount++];
              USISRL = SLV_Tx_Pointer[Bytecount++];
              USICNT |=  0x08;              // Bit counter = 8, TX data
              I2C_State = 12;               // Go to next state: receive (N)Ack
}

void checkRXAndSetTX()
{
  unsigned int uINT_Value = 0;
  unsigned char *Sgement_ptr;
  
  uINT_Value = RxCount;
  if(RxCount > 1){
    uINT_Value = SLV_RxTxBuffer[1] + 2;
  }
  for(; uINT_Value < Number_of_Bytes ; uINT_Value++){
    SLV_RxTxBuffer[uINT_Value] = 0;
  }
  
  if(G_uc_SysModeStatusCode <= StartUp){
    //return;
  }
  
  switch (SLV_RxTxBuffer[0]) {
    
    
    /////////////////////////////////////////////
    //reset system,
    /////////////////////////////////////////////
    case 0xc0:
      G_uc_SysModeStatusCode = StartUp;
      break;
    
    /////////////////////////////////////////////
    //get Timer A TACCR0 Values,
    /////////////////////////////////////////////
    case 0xd0:
      uINT_Value = TACCR0;
      SLV_RxTxBuffer[0] = uINT_Value & 0x00ff;
      SLV_RxTxBuffer[1] = (uINT_Value & 0xff00) >> 8;
      break;
    /////////////////////////////////////////////
    //set Timer A TACCR0 Values,
    /////////////////////////////////////////////
    //  SLV_RxTxBuffer[1] : data length = 2
    //  SLV_RxTxBuffer[2] : TACCR0, Low bytes
    //  SLV_RxTxBuffer[3] : TACCR0, High bytes
    //  example: d1, 02, XX, XX ==> set Timer A TACCR0 Values
    case 0xd1:
      if(RxCount != (SLV_RxTxBuffer[1]+2)){break;}
      TACCR0 = SLV_RxTxBuffer[3] << 8 | SLV_RxTxBuffer[2];
      break;
    
    ///////////////////////////////////////////////
    //  get ADC values
    //  10 bytes
    ///////////////////////////////////////////////
    case 0xd2:
      uINT_Value = GetADCValue(Vbat_ADC);
      SLV_RxTxBuffer[0] = uINT_Value & 0x00ff;
      SLV_RxTxBuffer[1] = (uINT_Value & 0xff00) >> 8;
      uINT_Value = GetADCValue(T1_ADC);
      SLV_RxTxBuffer[2] = uINT_Value & 0x00ff;
      SLV_RxTxBuffer[3] = (uINT_Value & 0xff00) >> 8;
      uINT_Value = GetADCValue(ICHG_ADC);
      SLV_RxTxBuffer[4] = uINT_Value & 0x00ff;
      SLV_RxTxBuffer[5] = (uINT_Value & 0xff00) >> 8;
      uINT_Value = GetADCValue(IDSG_ADC);
      SLV_RxTxBuffer[6] = uINT_Value & 0x00ff;
      SLV_RxTxBuffer[7] = (uINT_Value & 0xff00) >> 8;
      break;
    
      // get status
    case 0xd3:
      SLV_RxTxBuffer[0] = G_Module_Status;
      SLV_RxTxBuffer[1] = G_Module_Status >> 8;
      SLV_RxTxBuffer[2] = G_uc_SysModeStatusCode;
      SLV_RxTxBuffer[3] = G_uc_SystemFailureCode;
      SLV_RxTxBuffer[4] = G_Activate_Action_Status;
      SLV_RxTxBuffer[5] = G_Activate_Action_Status >> 8;
      SLV_RxTxBuffer[6] = G_CHG_CV_MODE_Cycle_Count;
      break;
    /////////////////////////////////////////////
    //set mosfet on/off,
    /////////////////////////////////////////////
    //SLV_RxTxBuffer[1] : data length = 2
    //SLV_RxTxBuffer[2] : 1 = CHG_mos, 2 = DSG_mos.
    //SLV_RxTxBuffer[3] : 0 = off, 1 = on.
    //  example: d4, 02, 01, 01 ==> CHG_mos turn on
    case 0xd4:
      if(RxCount != (SLV_RxTxBuffer[1]+2)){break;}
      if(SLV_RxTxBuffer[2] == 1){
        setMosFET(MOSFET_CHG, SLV_RxTxBuffer[3]);
      }else if(SLV_RxTxBuffer[2] == 2){
        setMosFET(MOSFET_DSG, SLV_RxTxBuffer[3]);
      }    
      break;
    /////////////////////////////////////////////
    //get IO Status,
    /////////////////////////////////////////////
    case 0xd5:
      //SLV_RxTxBuffer[0] = 0xf0;
      SLV_RxTxBuffer[0] = get_PIC_UVP_Status();
      SLV_RxTxBuffer[1] = get_PIC_OVP_Status();
      SLV_RxTxBuffer[2] = getMosCHG();
      SLV_RxTxBuffer[3] = getMosDSG();
      break;
    /////////////////////////////////////////////
    //set Read Information Flash var
    /////////////////////////////////////////////
    //SLV_RxTxBuffer[1] : data length = 2 
    //SLV_RxTxBuffer[2] : 1 = CONFIG_SEGMENT, 2 = CALIBRATION_SEGMENT.
    //SLV_RxTxBuffer[3] : Number of section. (information flash = 64 bytes, so there are 8 number of section(from 0 ~ 7 sections)
    //  example: d6, 02, 01, 01 ==> Read CONFIG_SEGMENT at 1 section.
    case 0xd6:
      if(RxCount != (SLV_RxTxBuffer[1]+2)){break;}
      if(SLV_RxTxBuffer[2] == 2){
        Sgement_ptr = (unsigned  char *)CALIBRATION_SEGMENT;
      }else{
        Sgement_ptr = (unsigned  char *)CONFIG_SEGMENT;
      }
      uINT_Value = 8 * SLV_RxTxBuffer[3];
      Sgement_ptr += uINT_Value;
      for(uINT_Value = 0; uINT_Value < 8; uINT_Value++){
        SLV_RxTxBuffer[uINT_Value] = *Sgement_ptr;
        Sgement_ptr++;
      }
      break;
  
      /////////////////////////////////////////////////////
      //information flash memory Write
      /////////////////////////////////////////////////////
      //SLV_RxTxBuffer[0] == 0xe0;
      //SLV_RxTxBuffer[1] : receive Data length==> 2 + n of write data
      //SLV_RxTxBuffer[2] : SegmentSelection, 0 =Flash_segment_B(0x1080,CONFIG_SEGMENT), 1 =Flash_segment_C(0x1040,CALIBRATION_SEGMENT)
      //SLV_RxTxBuffer[3] : data offset address
      //SLV_RxTxBuffer[4]...[n] : write data (length = SLV_RxTxBuffer[1] - 2)
    case 0xe0:
      if(RxCount != (SLV_RxTxBuffer[1]+2)){break;}
      //void WriteDataToFlash(unsigned char Offset_Address, unsigned char *value, unsigned char dataLength, unsigned char SegmentSelection);
      WriteDataToFlash(SLV_RxTxBuffer[3], &SLV_RxTxBuffer[4], SLV_RxTxBuffer[1]-2, SLV_RxTxBuffer[2]);
      break;
      /////////////////////////////////////////////////////
      //information flash memory Read
      /////////////////////////////////////////////////////
      //SLV_RxTxBuffer[0] == 0xe1;
      //SLV_RxTxBuffer[1] = 3; receive Data length
      //SLV_RxTxBuffer[2] : SegmentSelection, 0 =Flash_segment_B(0x1080,CONFIG_SEGMENT), 1 =Flash_segment_C(0x1040,CALIBRATION_SEGMENT)
      //SLV_RxTxBuffer[3] : data offset address
      //SLV_RxTxBuffer[4] : read data length
    case 0xe1:
      if(RxCount != (SLV_RxTxBuffer[1]+2)){break;}
      //void ReadInitialDataFromFlash(unsigned char Offset_Address, unsigned char *out_ptr, unsigned char dataLength, unsigned char SegmentSelection);
      ReadInitialDataFromFlash(SLV_RxTxBuffer[3], &uINT_Value, SLV_RxTxBuffer[4], SLV_RxTxBuffer[2]);
      SLV_Tx_Pointer = (unsigned char *)uINT_Value;
      break;
  
      /////////////////////////////////////////////////////
      //Calibration Region , Two points calibration
      /////////////////////////////////////////////////////
      /////////////////////////////////////////////////////
      // into Calibration mode
    case 0xf0:
      G_uc_SysModeStatusCode = CalibrationMode;
      break;
      /////////////////////////////////////////////////////
      // Set Calibration Data
      //SLV_RxTxBuffer[0] == 0xe1;
      //SLV_RxTxBuffer[1] = 4; Sending Data length
      //SLV_RxTxBuffer[2] : Function Selection, 1 =DSG Current, 2 =CHG Current, 3 =VBAT
      //SLV_RxTxBuffer[3] : 1 = First_Cal point, 2 = Second_Cal point
      //SLV_RxTxBuffer[4] : Lo byte of present Current mA Values or present Voltage 10mV Values
      //SLV_RxTxBuffer[5] : Hi byte of present Current mA Values or present Voltage 10mV Values
      //example 1: f1, 04, 02, 01 , E8, 03 ==> set CHG Current at First Cal. point, 1000mA
      //example 2: f1, 04, 03, 01 , E8, 03 ==> set VBAT Voltage at First Cal. point, 1000 * 10mV = 10000mV = 10V
    case 0xf1:
      if(RxCount != (SLV_RxTxBuffer[1]+2)){break;}
      //SetCalibrationData(unsigned char Function_Selection, unsigned char Cal_point, unsigned int present_Values);
      //void ReadInitialDataFromFlash(unsigned char Offset_Address, unsigned char *out_ptr, unsigned char dataLength, unsigned char SegmentSelection);
      ReadInitialDataFromFlash(SLV_RxTxBuffer[3], &uINT_Value, SLV_RxTxBuffer[4], SLV_RxTxBuffer[2]);
      SLV_Tx_Pointer = (unsigned char *)uINT_Value;
      break;
    case 0xf2:
      G_uc_SysModeStatusCode = StartUp;
      break;
  
    default:
      break;
  }
}

void SLV_receive_Stop(){
    USISRL = 0xFF;              // Send NAck
    USICTL0 &= ~USIOE;            // SDA = input
    I2C_State = 0;                // Reset state machine
    Bytecount =0;                 // Reset counter for next TX/RX
}
//******************************************************************************
// USI interrupt service routine
// Rx bytes from master: State 2->4->6->8 
// Tx bytes to Master: State 2->4->10->12->14
//******************************************************************************
#pragma vector = USI_VECTOR
__interrupt void USI_TXRX (void)
{
  if (USICTL1 & USISTTIFG)                  // Start entry?
  {
    //P1OUT |= 0x01;                          // LED on: sequence start
    I2C_State = 2;                          // Enter 1st state on start
  }

  switch(__even_in_range(I2C_State,14))
    {
      case 0:                               // Idle, should not get here
              break;

      case 2: // RX Address
              //setTrace(c2);
              Bytecount = 0;
              USICNT = (USICNT & 0xE0) + 0x08; // Bit counter = 8, RX address
              USICTL1 &= ~USISTTIFG;        // Clear start flag
              I2C_State = 4;                // Go to next state: check address
              IIC_SLV_Status = 0;
              break;

      case 4: // Process Address and send (N)Ack
              //setTrace(c4);
              if (USISRL & 0x01){            // If master read...
                //SLV Transmit
                //setTrace(c4_t1);
                IIC_SLV_Status |= SLV_TRANSMIT;
              }
              else{
                //if master write
                //SLV Receive
                //setTrace(c4_t0);
                IIC_SLV_Status &= ~SLV_TRANSMIT;
              }
              USICTL0 |= USIOE;             // SDA = output
              if ((USISRL >> 1) == SlaveAddress7Bit)       // Address match?
              {
                //match address
                //setTrace(c4_matchAdd);
                IIC_SLV_Status &= ~ADDRESS_NOT_MATCH;
                USISRL = 0x00;              // Send Ack

                if (IIC_SLV_Status & SLV_TRANSMIT){ 
                  //SLV going to transmit data
                  //如果接收到一個byte的cmd ，則會在write之後執行此段
                  if(RxCount < 2){
                    checkRXAndSetTX();
                  }
                  I2C_State = 10; // Else go to next state: TX data
                }else{
                  //SLV going to receive data
                  RxCount = 0;
                  IIC_SLV_Status &= ~SLV_RECEIVE_CMD_DONE;
                  I2C_State = 6;  // Go to next state: RX data
                }
              }
              else
              {
                //setTrace(c4_nomatchAdd);
                IIC_SLV_Status |= ADDRESS_NOT_MATCH;
                USISRL = 0xFF;              // Send NAck
                I2C_State = 8;              // next state: prep for next Start
              }
              USICNT |= 0x01;               // Bit counter = 1, send (N)Ack bit
              break;

    case 6: // Receive data byte
              //setTrace(c6);
              //Data_RX();
              
              USICTL0 &= ~USIOE;            // SDA = input
              USICNT |=  0x08;              // Bit counter = 8, RX data
              I2C_State = 8;                // next state: Test data and (N)Ack
              break;  

      case 8:// Check Data & TX (N)Ack
              //setTrace(c8);
              
              if(IIC_SLV_Status & ADDRESS_NOT_MATCH){
                IIC_SLV_Status &= ~ADDRESS_NOT_MATCH;
                SLV_receive_Stop();
//                USISRL = 0xFF;              // Send NAck
//                USICTL0 &= ~USIOE;            // SDA = input
//                I2C_State = 0;                // Reset state machine
//                Bytecount =0;                 // Reset counter for next TX/RX
                break;
              }
              
              // get Receive data byte
              // SLV_RxTxBuffer[1] is cmd length
//              SLV_RxTxBuffer[RxCount++] = USISRL;
//              if(RxCount >= 2){
//                if(SLV_RxTxBuffer[1] == (RxCount - 2)){
//                  checkRXAndSetTX();
//                  IIC_SLV_Status |= SLV_RECEIVE_CMD_DONE;
//                  SLV_receive_Stop();
//                }
//              }
              ///////////////////////////////////////////
              // using pointer
              // get Receive data byte
              // SLV_Tx_Pointer[1] is cmd length
              SLV_Tx_Pointer[RxCount++] = USISRL;
              //如果接收到2個以上bytes的cmd ，則會在read開始時執行此段
              if(RxCount >= 2){
                if(SLV_Tx_Pointer[1] == (RxCount - 2)){
                  checkRXAndSetTX();
                  IIC_SLV_Status |= SLV_RECEIVE_CMD_DONE;
                  SLV_receive_Stop();
                }
              }
              
              USICTL0 |= USIOE;             // SDA = output
              if (Bytecount <= (Number_of_Bytes-2))          
                                            // If not last byte
              {
                //setTrace(c8_notLast);
                USISRL = 0x00;              // Send Ack
                I2C_State = 6;              // Rcv another byte
                Bytecount++;
                USICNT |= 0x01;             // Bit counter = 1, send (N)Ack bit
              }
              else                          // Last Byte
              {
                //setTrace(c8_Last);
                SLV_receive_Stop();
//                USISRL = 0xFF;              // Send NAck
//                USICTL0 &= ~USIOE;            // SDA = input
//                I2C_State = 0;                // Reset state machine
//                Bytecount =0;                 // Reset counter for next TX/RX
              }
              
              break;

     case 10: // Send Data byte
              //setTrace(c10);
              TX_Data();
              break;

      case 12:// Receive Data (N)Ack
              //setTrace(c12);
              USICTL0 &= ~USIOE;            // SDA = input
              USICNT |= 0x01;               // Bit counter = 1, receive (N)Ack
              I2C_State = 14;               // Go to next state: check (N)Ack
              break;

      case 14:// Process Data Ack/NAck
              //setTrace(c14);
              if (USISRL & 0x01)               // If Nack received...
              {
                //setTrace(c14_NackRec);
                SLV_Tx_Pointer = SLV_RxTxBuffer;
                USICTL0 &= ~USIOE;            // SDA = input
                I2C_State = 0;                // Reset state machine
                Bytecount = 0;
                // LPM0_EXIT;                  // Exit active for next transfer
              }
              else                          // Ack received
              {
                //setTrace(c14_AckRec);
                TX_Data();                  // TX next byte
               }
	       break;
      }
  USICTL1 &= ~USIIFG;                       // Clear pending flags
  
}


