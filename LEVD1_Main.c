
#include "LEV_PrjDefine.h"

#define Mid_Count   64
#define _1st_Priority_Values  10
#define _2nd_Priority_Values  5
#define _3rd_Priority_Values  3

unsigned int G_Module_Status;
unsigned char G_uc_SystemFailureCode;
unsigned int G_Activate_Action_Status;
//unsigned char G_CHG_CV_MODE_Cycle_Count;
unsigned int G_Activate_Action_Status_Other1;

unsigned char Suspend_Func();


//////////////////////////////////////////////////////////////////////////////
// at Timer A
//////////////////////////////////////////////////////////////////////////////
// 2012/05/07 Remove DOC Propection, hsinmo
extern unsigned char OC_Timer_Counter;
extern unsigned int CHG_CV_MODE_Timer_Counter;
extern unsigned int WAKE_UP_Timer_Counter;


//////////////////////////////////////////////////////////////////////////////
unsigned int values;

//////////////////////////////////////////////////////////////////////////////
unsigned char FirstInitial_Func(){
  
  // Initialize LEDs
  InitLEDPort(); 
  InitBlinkLEDs();
  
//  __delay_cycles(100000);  // 100.8 ms ==> 1MHz clock
//  __delay_cycles(10000);  // 10.1ms ==> 1MHz clock
//  __delay_cycles(1000);  // 1ms ==> 1MHz clock
//  __delay_cycles(100);  // 106us ==> 1MHz clock
//  __delay_cycles(10);  // 165s ==> 1MHz clock

  //getRealCapacityByCell(int ADC_Voltage, int ADC_current)
//  values = getRealCapacityByCell(668,60);
//  values = getRealCapacityByCell(668,50);  
//  values = getRealCapacityByCell(668,61);  
//  values = getRealCapacityByCell(668,181);  
//  values = getRealCapacityByCell(668,182);  
//  values = getRealCapacityByCell(668,362);  
//  values = getRealCapacityByCell(668,363);  
//  values = getRealCapacityByCell(668,400);  
//  values = getRealCapacityByCell(668,60);  
  
  //Setup_USI_Slave();
  return 0;
}


unsigned char Startup_Func()
{
  
  //Initialization
  _DINT();
  // Initialize LEDs
  InitLEDPort();
  InitBlinkLEDs();
 
  
  __delay_cycles(100000);  // 100ms ==> 1MHz clock
  
  Setup_USI_Slave();
  InitTimerA();
  InitButton();
  Enable_SW_Interrupt(true);
  
  //InitAdcReader();
  InitInputSignalGetting();
  InitMosControl();
  
  _EINT();
  
  StartAdcConversion();

  G_Module_Status = 0;
  G_uc_SystemFailureCode = SystemNormal;
  G_Activate_Action_Status = 0;
  G_Activate_Action_Status_Other1 = 0;
  //return ShippingMode;
  InitCoulombCounter();

  //Power-on self-test
  G_uc_SysModeStatusCode = POSTMode;
  StartAdcConversion();  
  //MOSFET Fail Check
//  if( GetADCValue(IDSG_ADC) > ADC_DETECT_CURRENT_OF_DSG_STATUS ||
//      GetADCValue(ICHG_ADC) > ADC_DETECT_CURRENT_OF_CHG_STATUS ){
//      G_uc_SystemFailureCode = MOSFETFail;
//      setBlinkLED(SystemFailBlinkLED, true);
//      return FailureMode;
//    }


  return NormalMode;
}

unsigned char Normal_Func(){
  //////////////////////////////////////////////////////////////////////////////
  // 2012/05/07 Remove DOC Propection, hsinmo
  //unsigned int DSG_OC_Delay_Count;
  unsigned int CHG_OC_Delay_Count;
  unsigned int _2ND_OV_Delay_Count;
  unsigned int _2ND_UV_Delay_Count;
  unsigned int iTemp1, iTemp2;
  
  //DSG_OC_Delay_Count = 0;
  CHG_OC_Delay_Count = 0;
  _2ND_OV_Delay_Count = 0;
  _2ND_UV_Delay_Count = 0;
  
  setMosFET(MOSFET_CHG, DeviceOn);
  setMosFET(MOSFET_DSG, DeviceOn);


  //Clear Activate Action Flag
  G_Activate_Action_Status = 0;
  G_Activate_Action_Status_Other1  = 0;
  //G_CHG_CV_MODE_Cycle_Count = 0;
  //G_DSG_CHG_OC_Delay_Count = 0;
  
  //Coulomb Counter Start
  G_Activate_Action_Status |= ACCUMULATING_Q_ENABLE;
  
  while(1){

    
    __delay_cycles(3000);  // 3ms ==> 1MHz clock
    StartAdcConversion();
    
    ///////////////////////////////////////
    // Current Detection
    iTemp1 = GetADCValue(ICHG_ADC);
    iTemp2 = GetADCValue(IDSG_ADC);
    if( iTemp1 >= ADC_DETECT_CURRENT_OF_CHG_STATUS){
      // in charging status
      G_Module_Status &= ~Module_DSG;
      G_Module_Status &= ~Module_RELAX;
    }else if( iTemp2 < ADC_DETECT_CURRENT_OF_DSG_STATUS ){
      // in relaxation mode or Quiescent current
      G_Module_Status |= Module_DSG;
      G_Module_Status |= Module_RELAX;
    }else{
      // in discharging
      G_Module_Status &= ~Module_RELAX;
      G_Module_Status |= Module_DSG;
    }
  
    
    if( iTemp1 >= ADC_COC_PROTECTION && ((G_Module_Status & Module_C_OC) == 0 )){
      CHG_OC_Delay_Count++;
      if(CHG_OC_Delay_Count >= OC_PROTECTION_DELAY_LOOP_COUNT){
        G_Module_Status |= Module_C_OC;
        //setBlinkLED(OC_BlinkLED, true);
      }
    }else{
      CHG_OC_Delay_Count = 0;
    }
//////////////////////////////////////////////////////////////////////////////
// 2012/05/07 Remove DOC Propection, hsinmo
//    if( iTemp2 >= ADC_DOC_PROTECTION && ((G_Module_Status & Module_D_OC) == 0 )){
//      G_DSG_CHG_OC_Delay_Count += (1 << 4);
//      if((G_DSG_CHG_OC_Delay_Count & 0xf0) >= (PROTECTION_DELAY_LOOP_COUNT << 4 )){
//        G_Module_Status |= Module_D_OC;
//        setBlinkLED(OC_BlinkLED, true);
//      }
//    }else{
//      G_DSG_CHG_OC_Delay_Count &= ~(0xf0);
//    }
    
    
    
    ///////////////////////////////////////
    //  Voltage control
    //Vbat Voltage check ==> 2nd level
    iTemp1 = GetADCValue(Vbat_ADC);
   
    if( iTemp1 > ADC_2ND_BATTERY_OV_PROTECTION &&
        (G_Module_Status & Module_2nd_OV) == 0)
    {
      _2ND_OV_Delay_Count++;
      if(_2ND_OV_Delay_Count >= OV_PROTECTION_DELAY_LOOP_COUNT){
        G_Module_Status |= Module_2nd_OV;
        //setBlinkLED(OV_UV_BlinkLED, true);
      }
    }else if( iTemp1 < ADC_2ND_BATTERY_UV_PROTECTION && 
        (G_Module_Status & Module_2nd_UV) == 0)
    {
      _2ND_UV_Delay_Count++;
      if(_2ND_UV_Delay_Count >= UV_PROTECTION_DELAY_LOOP_COUNT){
        G_Module_Status |= Module_2nd_UV;
        //setBlinkLED(OV_UV_BlinkLED, true);
      }
    }else{
      _2ND_UV_Delay_Count = 0;
      _2ND_OV_Delay_Count = 0;
    }
    
    //Cell OV/UV protection and release
    if(get_PIC_UVP_Status()){
      G_Module_Status |= Module_UV;
    }else{
      G_Module_Status &= ~Module_UV;
    }
    if(get_PIC_OVP_Status()){
      G_Module_Status |= Module_OV;
    }else{
      G_Module_Status &= ~Module_OV;
    }   
    

    ///////////////////////////////////////
    //Thermal protection
    iTemp1 = GetADCValue(T1_ADC);
    if( iTemp1 <= ADC_CHG_OT_PROTECTION){
      G_Module_Status |= Module_CHG_OT;
    }
    if( iTemp1 <= ADC_DSG_OT_PROTECTION){
      G_Module_Status |= Module_DSG_OT;
    }
    
    if( iTemp1 >= ADC_UT_PROTECTION){
      G_Module_Status |= Module_UT;
    }
    if(G_Module_Status & Module_DSG_OT ||
       G_Module_Status & Module_CHG_OT ||
       G_Module_Status & Module_UT  ||
       G_Module_Status & Module_OV  ||
       G_Module_Status & Module_UV){
      //setBlinkLED(OT_UT_BlinkLED, true);
//      setBlinkLED(OV_UV_BlinkLED, true);
    }else{
      //setBlinkLED(OT_UT_BlinkLED, false);
//      setBlinkLED(OV_UV_BlinkLED, false);
    }
    
    /////////////////////////////////////////////////////////
    // Check abnormal Flag
    /////////////////////////////////////////////////////////
//    if((G_Module_Status & Module_2nd_OV) && (G_Module_Status & Module_2nd_UV)) {
//      G_uc_SystemFailureCode = _2nd_Level_Vol_Fail;
//      return FailureMode;
//    }
    /////////////////////////////////////////////////////////
    //2012/05/21 remove Fail mode function, hsinmo
//    if((G_Module_Status & Module_RELAX) && ((G_Module_Status & Module_DSG)==0)){
//      G_uc_SystemFailureCode = AbnormalFlagFail;
//      return FailureMode;
//    }
//    if((G_Module_Status & Module_C_OC) && (G_Module_Status & Module_D_OC)){
//      G_uc_SystemFailureCode = AbnormalFlagFail;
//      return FailureMode;
//    }
//    if((G_Module_Status & Module_UV) && (G_Module_Status & Module_OV)){
//      G_uc_SystemFailureCode = AbnormalFlagFail;
//      return FailureMode;
//    }
//    if((G_Module_Status & Module_DSG_OT) && (G_Module_Status & Module_UT)){
//      G_uc_SystemFailureCode = ThermalFail;
//      return FailureMode;
//    }
//    if((G_Module_Status & Module_CHG_OT) && (G_Module_Status & Module_UT)){
//      G_uc_SystemFailureCode = ThermalFail;
//      return FailureMode;
//    }
    
    /////////////////////////////////////////////////////////
    // Comparation protection and active function section
    /////////////////////////////////////////////////////////
    //check suspend mode
    if(G_Module_Status & Module_RELAX){
      G_Activate_Action_Status |= ENABLE_SUSPEND_COUNTER;
    }else{
      G_Activate_Action_Status &= ~ENABLE_SUSPEND_COUNTER;
      G_Activate_Action_Status &= ~SUSPEND_COUNTING_FINISH;
    }
    
    
    if(G_Module_Status & (Module_D_OC)){
//////////////////////////////////////////////////////////////////////////////
// 2012/05/07 Remove DOC Propection, hsinmo
//      setMosFET(MOSFET_CHG, DeviceOff);
//      setMosFET(MOSFET_DSG, DeviceOff);
//      if((G_Activate_Action_Status & ENABLE_OC_COUNTER)==0){
//        G_Activate_Action_Status |= ENABLE_OC_COUNTER;
//      }
//      //OC Release and clear Flag
//      if(G_Activate_Action_Status & OC_COUNTING_FINISH){
//        G_Activate_Action_Status &= ~ENABLE_OC_COUNTER;
//        G_Activate_Action_Status &= ~OC_COUNTING_FINISH;
//        OC_Timer_Counter = 0;
//        G_Module_Status &= ~Module_D_OC;
//        setMosFET(MOSFET_CHG, DeviceOn);
//        setMosFET(MOSFET_DSG, DeviceOn);
//        setBlinkLED(OC_BlinkLED, false);
//      }
    }else if(G_Module_Status & (Module_C_OC)){
      setMosFET(MOSFET_CHG, DeviceOff);
      setMosFET(MOSFET_DSG, DeviceOff);
      if((G_Activate_Action_Status & ENABLE_OC_COUNTER)==0){
        G_Activate_Action_Status |= ENABLE_OC_COUNTER;
      }
      //OC Release and clear Flag
      if(G_Activate_Action_Status & OC_COUNTING_FINISH){
        OC_Timer_Counter = 0;
        G_Activate_Action_Status &= ~ENABLE_OC_COUNTER;
        G_Activate_Action_Status &= ~OC_COUNTING_FINISH;
        G_Module_Status &= ~Module_C_OC;
        setMosFET(MOSFET_CHG, DeviceOn);
        setMosFET(MOSFET_DSG, DeviceOn);
        //setBlinkLED(OC_BlinkLED, false);
      }
    }else if(G_Module_Status & (Module_DSG_OT+ Module_CHG_OT + Module_UT)){
      
      iTemp1 = GetADCValue(T1_ADC);
      //////////////////////////////////////////////////////////////////////////////
      // 2012/05/08 Remove T2 temperature, hsinmo
      if(G_Module_Status & (Module_CHG_OT + Module_UT)){
        setMosFET(MOSFET_CHG, DeviceOff);
        if(G_Module_Status & Module_CHG_OT){
          if( iTemp1 > ADC_CHG_OT_RELEASE){
            G_Module_Status &= ~Module_CHG_OT;
          }
        }
        if(G_Module_Status & Module_UT){
          if( iTemp1 < ADC_UT_RELEASE){
            G_Module_Status &= ~Module_UT;
          }
        }
        if((G_Module_Status & (Module_CHG_OT + Module_UT))== 0){
          setMosFET(MOSFET_CHG, DeviceOn);
        }
        
      }//if(G_Module_Status & (Module_CHG_OT + Module_UT){
      if(G_Module_Status & Module_DSG_OT){
        setMosFET(MOSFET_DSG, DeviceOff);
        if(G_Module_Status & Module_DSG_OT){
          if( iTemp1 > ADC_DSG_OT_RELEASE){
            G_Module_Status &= ~Module_DSG_OT;
            setMosFET(MOSFET_DSG, DeviceOn);
          }
        }        
      }//if(G_Module_Status & (Module_DSG_OT){
      
    }else if(G_Module_Status & Module_2nd_OV){
        setMosFET(MOSFET_CHG, DeviceOff);
        //relese 2nd_OV
        iTemp1 = GetADCValue(Vbat_ADC);
        if( iTemp1 < ADC_2ND_BATTERY_OV_RELEASE){
          G_Module_Status &= ~Module_2nd_OV;
          setMosFET(MOSFET_CHG, DeviceOn);
        }
    }else if(G_Module_Status & Module_2nd_UV){
        
        /////////////////////////////////////////////////
        //remove by hsinmo 2012/05/23
        //relese 2nd_UV By Release Voltage
        /////////////////////////////////////////////////
        //setMosFET(MOSFET_DSG, DeviceOff);
        //iTemp1 = GetADCValue(Vbat_ADC);
        //if( iTemp1 > ADC_2ND_BATTERY_UV_RELEASE){
        //  G_Module_Status &= ~Module_2nd_UV;
        //  setMosFET(MOSFET_DSG, DeviceOn);
        //}
      
        /////////////////////////////////////////////////
        //add by hsinmo 2012/05/23
        //relese 2nd_UV By Charger staus
        if((G_Module_Status & Module_DSG)==0){
          // CHG status
          G_Module_Status &= ~Module_2nd_UV;
          setMosFET(MOSFET_DSG, DeviceOn);
        }else{
          // DSG status
          setMosFET(MOSFET_DSG, DeviceOff);
        }
    }else{  //else #1 start
      
      setMosFET(MOSFET_DSG, DeviceOn);
      setMosFET(MOSFET_CHG, DeviceOn);
      ////////////////////////////////////////////////////////////////////////////
      // if charging status and voltage higher then ADC_CHG_CV_MODE_LIMIT_VOLTAGE
      // set CHG_CV_MODE_START flag
      ////////////////////////////////////////////////////////////////////////////
      if((G_Module_Status & Module_DSG)==0){
        //charging status
//        iTemp1 = GetADCValue(Vbat_ADC);
//        if(iTemp1 >= ADC_CHG_CV_MODE_LIMIT_VOLTAGE && ((G_Activate_Action_Status & CHG_CV_MODE_START) == 0)){
//          G_Activate_Action_Status |= CHG_CV_MODE_START;
//        }
//        if((G_Activate_Action_Status & CHG_CV_MODE_START) == 0){
//          setMosFET(MOSFET_CHG, DeviceOn);
//        }
//        setMosFET(MOSFET_DSG, DeviceOn);
      }else{
        //discharging and releax status
        ////////////////////////////////////////////////////////////////////////////
        // if CHG_CV_MODE_START be set then RELAX status not for discharging
        ////////////////////////////////////////////////////////////////////////////
//        if((G_Module_Status & Module_RELAX) && (G_Activate_Action_Status & CHG_CV_MODE_START)){
//          //Still in CV_MODE
//          //do nothing
//        }else{
//          // release CV_MODE COUNTER
//          G_Activate_Action_Status &= ~CHG_CV_MODE_START;
//          G_CHG_CV_MODE_Cycle_Count = 0;
//          G_Activate_Action_Status &= ~ENABLE_CV_MODE_COUNTER;
//          G_Activate_Action_Status &= ~CV_MODE_RELEASE_COUNTING_FINISH;
//          
//          setMosFET(MOSFET_CHG, DeviceOn);
//          setMosFET(MOSFET_DSG, DeviceOn);
//        }
      }
    } //else #1 end

    ////////////////////////////////////////////////////////////////////////////
    // CHG_CV_MODE_START Process
    ////////////////////////////////////////////////////////////////////////////
//    if((G_Activate_Action_Status & CHG_CV_MODE_START)&&(G_CHG_CV_MODE_Cycle_Count < CHG_CV_MODE_REPEATING_CYCLE)){
//      iTemp1 = GetADCValue(Vbat_ADC);
//      if((iTemp1 >= ADC_CHG_CV_MODE_LIMIT_VOLTAGE) && (G_Activate_Action_Status & ENABLE_CV_MODE_COUNTER) == 0){
//        setMosFET(MOSFET_CHG, DeviceOff);
//        G_CHG_CV_MODE_Cycle_Count++;
//        G_Activate_Action_Status |= ENABLE_CV_MODE_COUNTER;
//      }
//      /////////////////////////////////////////////////////
//      //Last cycle will not be into this section
//      // it's mean Last cycle Chg mos will be turned off
//      if(G_Activate_Action_Status & CV_MODE_RELEASE_COUNTING_FINISH){
//        setMosFET(MOSFET_CHG, DeviceOn);
//        G_Activate_Action_Status &= ~ENABLE_CV_MODE_COUNTER;
//        G_Activate_Action_Status &= ~CV_MODE_RELEASE_COUNTING_FINISH;
//        CHG_CV_MODE_Timer_Counter = 0;
//      }
//    }
//    
    
    
    
    ///////////////////////////////////////////////////////////////////////////////
    // Button Function
    ///////////////////////////////////////////////////////////////////////////////
    if(G_Activate_Action_Status & BUTTON_LONG_PRESS_FLAG){
      G_Activate_Action_Status = 0;
      G_Activate_Action_Status_Other1 = 0;
      G_Activate_Action_Status_Other1 |= LightOff_Seq_LED;
      return ShutdownMode;
    }
    if(G_Activate_Action_Status & BUTTON_CLICK_FLAG){
      G_Activate_Action_Status &= ~BUTTON_CLICK_FLAG;
      if((G_Module_Status & Module_DSG)==0){
        iTemp1 = ADC_LOOKUP_1st_OCV_CURRENT_RANGE - 2;//charger=2A ==>4p cell=0.1C
        //iTemp1 = GetADCValue(ICHG_ADC);
      }else{
        iTemp1 = GetADCValue(IDSG_ADC);
      }
      iTemp2 = GetADCValue(Vbat_ADC);
      DisplayCapacity(getRealCapacityByCell(iTemp2,iTemp1), true);
    }
    
#if _SUSPEND_ENABLE_ > 0
    //into suspend mode
    if(G_Activate_Action_Status & SUSPEND_COUNTING_FINISH){
      //Coulomb Counter Stop
      G_Activate_Action_Status &= ~ACCUMULATING_Q_ENABLE;
      G_uc_SysModeStatusCode = Suspend_Func();
      //Coulomb Counter Start
      G_Activate_Action_Status |= ACCUMULATING_Q_ENABLE;
    }
#endif
    
    __delay_cycles(6000);  // 6ms ==> 1MHz clock
    if(G_uc_SysModeStatusCode != NormalMode){
      return G_uc_SysModeStatusCode;
    }

  } //while(1)
  
  return NormalMode;
  
}

unsigned char Failure_Func(){

  setMosFET(MOSFET_CHG, DeviceOff);
  setMosFET(MOSFET_DSG, DeviceOff);
  //setBlinkLED(LED_SET_ALL, false);
  __delay_cycles(1000);  // 1ms ==> 1MHz clock
  //setBlinkLED(SystemFailBlinkLED, true);
  
  G_Activate_Action_Status =0;
  G_Activate_Action_Status_Other1 = 0;
  while(1){
    if(G_uc_SysModeStatusCode != FailureMode){
      return G_uc_SysModeStatusCode;
    }
    if(G_Activate_Action_Status & BUTTON_LONG_PRESS_FLAG){
      G_Activate_Action_Status = 0;
      G_Activate_Action_Status_Other1 = 0;
      return ShutdownMode;
    }
  };
  
  return FailureMode;
}

unsigned char Shutdown_Func(){

  setMosFET(MOSFET_CHG, DeviceOff);
  setMosFET(MOSFET_DSG, DeviceOff);
  //setBlinkLED(LED_SET_ALL, false);
  __delay_cycles(1000);  // 1ms ==> 1MHz clock
  //BlinkLED(SystemFailBlinkLED, true);

#if _5LEDs_Thick_TYPE_ == 0
  unsigned char i,j, bit;
  bit = LED_PORT4;
  P2OUT |= ALL_LED_PORT;
  
  for(i=0; i<4; i++){
    for(j=0; j< 5;j++){
      __delay_cycles(100000);  // 100ms ==> 1MHz clock
    }
    P2OUT &= ~ALL_LED_PORT;
    P2OUT |= ALL_LED_PORT & (~bit);
    bit = (bit >> 1)  | LED_PORT4;
  }
  P2OUT &=~ALL_LED_PORT;
#endif
  
  G_Activate_Action_Status = 0;
  //G_Activate_Action_Status_Other1 = 0;
  while(1){
    if(G_uc_SysModeStatusCode != ShutdownMode){
      return G_uc_SysModeStatusCode;
    }
    if(G_Activate_Action_Status & BUTTON_LONG_PRESS_FLAG){
      G_Activate_Action_Status = 0;
      G_Activate_Action_Status_Other1 = 0;
      return StartUp;
    }
  };
  return StartUp;
}
unsigned char Calibration_Func(){

  setMosFET(MOSFET_CHG, DeviceOn);
  setMosFET(MOSFET_DSG, DeviceOn);
  //setBlinkLED(LED_SET_ALL, false);
  //DisplayLED(LED_SET_ALL, DeviceOn);
  G_Activate_Action_Status = 0;
  //G_Activate_Action_Status_Other1 = 0;


  __delay_cycles(10000);  // 10ms ==> 1MHz clock
  //BlinkLED(SystemFailBlinkLED, true);
  
  while(1){
    if(G_uc_SysModeStatusCode != CalibrationMode){
      break;
    }
    if(G_Activate_Action_Status & BUTTON_LONG_PRESS_FLAG){

      break;
    }
    __delay_cycles(10000);  // 10ms ==> 1MHz clock
    StartAdcConversion();

  };
  G_Activate_Action_Status = 0;
  G_Activate_Action_Status_Other1 = 0;
  setMosFET(MOSFET_CHG, DeviceOff);
  setMosFET(MOSFET_DSG, DeviceOff);
  return StartUp;
}

unsigned char Suspend_Func(){
  
  G_uc_SysModeStatusCode = SuspendMode;
  //setBlinkLED(LED2, true);
  
  

  while(1){
    setMosFET(MOSFET_CHG, DeviceOff);
    setMosFET(MOSFET_DSG, DeviceOff);
    ///////////////////////////////////////////////////////
    //解決 BUTTON_LONG_PRESS 進入 SuspendMode 時，
    // BUTTON_CLICK 無作用.
    G_Activate_Action_Status &= ~BUTTON_LONG_PRESS_FLAG;
    
    ///////////////////////////////////////////////////////
    //解決 進入 SuspendMode 時，無法跳到 CalibrationMode
    if(G_uc_SysModeStatusCode != SuspendMode){
      return CalibrationMode;
    }
    ///////////////////////////////////////////////////////
    // into Low Power Mode
    G_Activate_Action_Status |= ENABLE_SUSPEND_WAKE_UP_COUNTER;
    
    ///////////////////////////////////////////////////////
    G_Activate_Action_Status_Other1 |= Low_Power_Mode;
    _BIS_SR(LPM3_bits);                       // Enter LPM3 w/interrupt
    ///////////////////////////////////////////////////////

    
    __delay_cycles(10);
    if(G_Activate_Action_Status & SUSPEND_WAKE_UP_COUNTING_FINISH){
      G_Activate_Action_Status &= ~SUSPEND_WAKE_UP_COUNTING_FINISH;
      G_Activate_Action_Status &= ~ENABLE_SUSPEND_WAKE_UP_COUNTER;
    }
    setMosFET(MOSFET_CHG, DeviceOn);
    setMosFET(MOSFET_DSG, DeviceOn);
    //initial
    //InitAdcReader();
    __delay_cycles(100000);  // 100ms ==> 1MHz clock
    __delay_cycles(100000);  // 100ms ==> 1MHz clock
    if(G_Activate_Action_Status & (BUTTON_CLICK_FLAG + BUTTON_LONG_PRESS_FLAG)){
      //BUTTON_CLICK 離開susspend mode 並顯示 capacity
      //G_Activate_Action_Status &= ~BUTTON_CLICK_FLAG;
      break;
    }
    
    // Start ADC conversion
    StartAdcConversion();
    // get current
    if( GetADCValue(ICHG_ADC) >= ADC_DETECT_CURRENT_OF_CHG_STATUS || GetADCValue(IDSG_ADC) >= ADC_DETECT_CURRENT_OF_DSG_STATUS){
      //Wake-up and into Normal mode
      G_Activate_Action_Status &= ~SUSPEND_COUNTING_FINISH;
      G_Activate_Action_Status &= ~ENABLE_SUSPEND_COUNTER;
      break;
    }
//    //////////////////////////////////////////////
//    //for test 起來後會直接進入Normal
//      G_Activate_Action_Status &= ~SUSPEND_COUNTING_FINISH;
//      G_Activate_Action_Status &= ~ENABLE_SUSPEND_COUNTER;
//      break;
  }
  
  
  G_Activate_Action_Status = (G_Activate_Action_Status & (BUTTON_CLICK_FLAG + BUTTON_LONG_PRESS_FLAG)); //僅留button click 狀態
  G_Activate_Action_Status_Other1 = 0;
  
  return NormalMode;
}

  //_BIS_SR(LPM3_bits);                       // Enter LPM3 w/interrupt
  //__bic_SR_register_on_exit(LPM0_bits);  // Clear LPM0 bits from 0(SR) use in interrupt



//unsigned char Shipping_Func(){
//
//  setMosFET(MOSFET_CHG, DeviceOff);
//  setMosFET(MOSFET_DSG, DeviceOff);
//  setBlinkLED(LED_SET_ALL, false);
//  
//  __delay_cycles(1000);  // 1ms ==> 1MHz clock
//  
//  while(1){
//    
//    //ADC Stop
//    ADC10CTL0 &= ~(ADC10SHT_2 + MSC + ADC10ON + ADC10IE +SREF_1+REFON+REF2_5V);
//    setMosFET(MOSFET_CHG, DeviceOff);
//    // power off 
//    //SetMainPowerAction( DeviceOff );
//    __delay_cycles(10000);  // 10ms ==> 1MHz clock
//    
//    ///////////////////////////////////////////////////////
//    // into Low Power Mode
//    G_Activate_Action_Status |= ENABLE_WAKE_UP_COUNTER;
//    _BIS_SR(LPM3_bits);                       // Enter LPM3 w/interrupt
//    G_Activate_Action_Status &= ~WAKE_UP_COUNTING_FINISH;
//    WAKE_UP_Timer_Counter = 0;
//    // power on 
//    //SetMainPowerAction( DeviceOn );
//    __delay_cycles(10000);  // 10ms ==> 1MHz clock
//    //ADC on
//    ADC10CTL0 |= (ADC10SHT_2 + MSC + ADC10ON + ADC10IE +SREF_1+REFON+REF2_5V);
//    //detect charging adapter plugging-in
//    setMosFET(MOSFET_CHG, DeviceOn);
//    __delay_cycles(10000);  // 10ms ==> 1MHz clock
//    // Start ADC conversion
//    StartAdcConversion();
//    // get current
//    if( GetADCValue(ICHG_ADC) >= ADC_DETECT_CURRENT_OF_CHG_STATUS){
//      //Wake-up and into Normal mode
//      break;
//    }
//  }
//  return StartUp;
//}








