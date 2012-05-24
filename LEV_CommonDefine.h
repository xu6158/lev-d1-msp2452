/////////////////////////////////////////////
// System global define
/////////////////////////////////////////////

#define true            1
#define false           0


enum Device_Enable
{
  DeviceOff,
  DeviceOn
};

enum For_G_uc_SysModeStatusCode  //unsigned char
{
  UnKnowStatus,
  StartUp,
  POSTMode, //Power-on self-test
  NormalMode,
  FailureMode,
  ShutdownMode,
  CalibrationMode,
  //ShippingMode,
  SuspendMode
};
enum For_G_uc_SystemFailureCode  //unsigned char
{
  SystemNormal,
  McuCalValueFail,
  MOSFETFail,
  CurrentOPFail,
  _2nd_Level_Vol_Fail,
  AbnormalFlagFail,
  ThermalFail
};

//////////////////////////////////////
extern unsigned char G_uc_SysModeStatusCode;
extern unsigned int G_Module_Status;
extern unsigned char G_uc_SystemFailureCode;
extern unsigned int G_Activate_Action_Status;
//extern unsigned char G_CHG_CV_MODE_Cycle_Count;
extern unsigned int G_Activate_Action_Status_Other1;

//////////////////////////////////////

/* Module Status Control Bits */
/* For G_Module_Status ; unsigned int */
//Low byte
#define Module_DSG          				(0x0001)    //Discharging, 0 = in charging mode,1 = in discharging mode or relaxation mode.
#define Module_RELAX         				(0x0002)    //relaxation mode or Quiescent current, usually equal to discharging
#define NoUse1        				(0x0004)    //No Use,
#define NoUse2        				(0x0008)    //No Use,
#define NoUse3        				(0x0010)    //No Use,
#define NoUse4        				(0x0020)    //No Use,
#define NoUse5        				(0x0040)    //No Use,
#define Module_UT        			(0x0080)    // Under Temperature Alarm
//Hight byte
#define Module_D_OC         (0x0100)    //=D_OC, DisCharge over current flag
#define Module_C_OC         (0x0200)    //=C_OC, Charge over current flag
#define Module_OV           (0x0400)    // Over Voltage Alarm
#define Module_UV           (0x0800)    // Under Voltage Alarm
#define Module_2nd_OV       (0x1000)    // 2nd level Over Voltage Alarm
#define Module_2nd_UV       (0x2000)    // 2nd level Under Voltage Alarm
#define Module_CHG_OT       (0x4000)    // CHG Over Temperature Alarm
#define Module_DSG_OT       (0x8000)    // DSG Over Temperature Alarm 
//#define Module_2nd_OV       (0x1000)    // 2nd level Over Voltage Alarm
//#define Module_2nd_UV       (0x2000)    // 2nd level Under Voltage Alarm

/* Module Status Control Bits */
/* For G_Activate_Action_Status ; unsigned int */
//Low byte
#define BUTTON_PRESS_FLAG               (0x0001)    //if button has been pressed, will set BUTTON_PRESS_FLAG flag
#define BUTTON_LONG_PRESS_FLAG          (0x0002)    //if button has been pressed longer than setting time, will set BUTTON_LONG_PRESS_Flag flag
#define ENABLE_OC_COUNTER               (0x0004)    //Start Over current counting. if finish, will set OC_COUNTING_FINISH flag
#define OC_COUNTING_FINISH              (0x0008)    //Over current counting Finish.
#define CHG_CV_MODE_START               (0x0010)    //Start CV_MODE_RELEASE waiting time counting. if finish, will set CV_MODE_RELEASE_COUNTING_FINISH flag
#define ENABLE_CV_MODE_COUNTER          (0x0020)    //Start CV_MODE_RELEASE waiting time counting. if finish, will set CV_MODE_RELEASE_COUNTING_FINISH flag
#define CV_MODE_RELEASE_COUNTING_FINISH (0x0040)    //CV_MODE_RELEASE waiting time counting Finish.
#define BUTTON_CLICK_FLAG               (0x0080)    //if button has been pressed and then release with out BUTTON_LONG_PRESS_FLAG is set
//Hight byte
#define ENABLE_WAKE_UP_COUNTER          (0x0100)    //
#define WAKE_UP_COUNTING_FINISH         (0x0200)    //
#define ENABLE_SUSPEND_COUNTER          (0x0400)    // 
#define SUSPEND_COUNTING_FINISH         (0x0800)    // 
#define ENABLE_SUSPEND_WAKE_UP_COUNTER  (0x1000)    // 
#define SUSPEND_WAKE_UP_COUNTING_FINISH (0x2000)    // 
#define CAPACITY_DISLALY                (0x4000)    // 
#define ACCUMULATING_Q_ENABLE           (0x8000)    // 


/* Module Status Control Bits */
/* For G_Activate_Action_Status_Other1 ; unsigned int */
//Low byte
#define Low_Power_Mode   (0x0001)    //
//#define _No_Used_   (0x0002)    //
//#define _No_Used_   (0x0004)    //
//#define _No_Used_   (0x0008)    //
//#define _No_Used_   (0x0010)    //
//#define _No_Used_   (0x0020)    //
//#define _No_Used_   (0x0040)    //
//#define _No_Used_   (0x0080)    //
////Hight byte
//#define _No_Used_   (0x0100)    //
//#define _No_Used_   (0x0200)    //
//#define _No_Used_   (0x0400)    //
//#define _No_Used_   (0x0800)    //
//#define _No_Used_   (0x1000)    //
//#define _No_Used_   (0x2000)    //
//#define _No_Used_   (0x4000)    //
//#define _No_Used_   (0x8000)    //




