

/*****************************************************************
  The Flash Real Data are created in SystemConfigInitialData.s43
******************************************************************/

///////////////////////////////////////////////////////////
// Define Flash Segment Address (one segment = 64 bytes)
///////////////////////////////////////////////////////////
#define Flash_segment_Size 64  // bytes
#define Flash_segment_A   0x10C0  // protected
#define Flash_segment_B   0x1080  // for user
#define Flash_segment_C   0x1040  // can not burning data
#define Flash_segment_D   0x1000  // has already data inside

#define CONFIG_SEGMENT        Flash_segment_B
#define CALIBRATION_SEGMENT   Flash_segment_C

#define MANUFACTURE_DATE_YEAR   2012
#define MANUFACTURE_DATE_MONTH  5
#define MANUFACTURE_DATE_DAY    8
#define PRODUCT_NAME            "LEV"
#define PRODUCT_NAME_LENGTH     3

#define Serial_Number                       10
#define Version		                          10
#define MinorVersion		                    0


//////////////////////////////////////////////////////////////////////////////////////////////////
#define OC_PROTECTION_DELAY_LOOP_COUNT  100 //about 1 sec = 100
#define OV_PROTECTION_DELAY_LOOP_COUNT  300 //about 3 sec = 300
#define UV_PROTECTION_DELAY_LOOP_COUNT  300 //about 3 sec = 300

//////////////////////////////////////////////////////////////////////////////////////////////////
// LEV System Initial config Data
// Used by SystemConfigInitialData.s43 to save into Flash Memory while downloading code into MCU
//////////////////////////////////////////////////////////////////////////////////////////////////

#define _SERIAL_NUMBER_                       Serial_Number     //2 bytes

#define _NUMBER_OF_PARALLEL_CELLS_            4                 //1 byte
#define _NUMBER_OF_SERIES_CELLS_              13                //1 byte
      
//=====================================================================================================================
// CHG / DSG Current Detector (op amp)
////////////////////////////////////////////////////////////////////////////////
//ADC_resolution  1024    // 10 bit ADC
//ADC_Ref         2500    // mV
//ADC_Step        2.44140625f  ==>(float)ADC_Ref /  ADC_resolution ==> (mV)
////////////////////////////////////////////////////////////////////////////////
//Rsense                  3.3333f    //mR
////////////////////////////////////////////////////////////////////////////////
//mA to ADC factor 
//Factor = Rsense(R) * op_gain / ADC step(mV) => Multiplicative inverse Factor
////////////////////////////////////////////////////////////////
// CHG_OP_Gain             68.0f
// _CHG_mA_to_ADC_factor_    0.09280f   ==> (Rsense * CHG_OP_Gain / ADC_Step / 1000)==>小數點6位
// _CHG_10mA_to_ADC_factor_  0.9280f   ==> CHG_mA_to_ADC_factor * 10
// CHG_OP_ADC_OFFSET         (signed char)0    //實際值-理論值 
////////////////////////////////////////////////////////////////
// DSG_OP_Gain   120.0f
// _DSG_mA_to_ADC_factor_      0.163839f  ==> (Rsense*DSG_OP_Gain/ADC_Step/1000)==>小數點6位
// _DSG_10mA_to_ADC_factor_    1.63839f  ==> DSG_mA_to_ADC_factor * 10
// DSG_OP_ADC_OFFSET         (signed char)0    //實際值-理論值 
// 理論值 = 實際值 - Offset
////////////////////////////////////////////////////////////////////////////////
//(無條件進位==> 會比實際值還大一點)
//(使用四捨五入)
#define _ADC_DETECT_CURRENT_OF_DSG_STATUS_     7      //40mA; unit: mA; 2bytes; if current > the define, in discharging status
#define _ADC_DETECT_CURRENT_OF_CHG_STATUS_     9     //100mA; unit: mA; 2bytes; if current > the define, in charging status
#define _ADC_DOC_PROTECTION_                   3276   // 20A; unit: 10mA; discharging current protection is positive
#define _ADC_COC_PROTECTION_                   464    // 5A; unit: 10mA; charging current protection

//=====================================================================================================================
// Battery Voltage circuit and setting
////////////////////////////////////////////////////////////////
//    Battery |------Resistor1----+---Resistor2-----|GND
//    Vltage  |       1000KR      |   43.2KR
//                                |
//                                |
//                       voltage output 
//                       to MCU ADC in
////////////////////////////////////////////////////////////////
//VBAT_mV_To_ADC_Factor    (float)1/(ADC_Step/(VBAT_Resistor2/(VBAT_Resistor1+VBAT_Resistor2))==>小數點6位
//ADC_Step        2.44140625f //  mv/step
// _VBAT_mV_To_ADC_Factor_      0.016962f ==> _VBAT_10mV_To_ADC_Factor_    0.16962f  ==> VBAT_mV_To_ADC_Factor * 10
//VBAT_ADC_OFFSET          (signed char)(-5)   //實際值-理論值 
// 理論值 = 實際值 - Offset
////////////////////////////////////////////////////////////////
#define _ADC_2ND_BATTERY_OV_PROTECTION_       933 // 55V; unit: 10mV; 2bytes; 2nd level BATTERY OV PROTECTION
#define _ADC_2ND_BATTERY_OV_RELEASE_          916 // 54V; unit: 10mV; 2bytes; 2nd level BATTERY OV RELEASE
#define _ADC_2ND_BATTERY_UV_PROTECTION_       678 // 40V; unit: 10mV; 2bytes; 2nd level BATTERY UV PROTECTION
#define _ADC_2ND_BATTERY_UV_RELEASE_          712 // 42V; unit: 10mV; 2bytes; 2nd level BATTERY UV RELEASE 

//=====================================================================================================================
// NTC Thermistor setting by voltage
////////////////////////////////////////////////////////////////
//    VDD (3.3V) |------Resistor1--+---NTC-----|GND     (Resistor1 = 10kR)
//                                 |
//                                 |
//                       thermal voltage output
////////////////////////////////////////////////////////////////
//NTC溫度(C)	NTC (kR)	output (mV)
//0						27.391		2417.43
//3	          24.083    2331.78
//5	          22.132    2272.99
//25					10	  		1650
//40					5.8281		1215.1
//45					4.9183		1087.95
//50					4.1709		971.28
//55					3.5539		865.28
//60					3.0417		769.66
//65					2.6144		683.94
//70					2.2559		607.42
//75					1.9538		539.37
//80					1.698			479
//85					1.4805		425.56
//////////////////////////////////////////////////////////////////
//Thermistor_mV_To_ADC_Factor     0.4096f   // = 1/ADC_Step
//=====================================================================================================================
#define _ADC_DSG_OT_PROTECTION_            249    //unit: 607 mV; 70 Celcius; Over temperature protection for discharging(Low divided voltage)
#define _ADC_DSG_OT_RELEASE_               280    //unit: 684 mV; 65 Celcius; Over temperature release for discharging(Low divided voltage)
#define _ADC_CHG_OT_PROTECTION_            398    //unit: 971 mV; 50 Celcius; Over temperature protection for charging(Low divided voltage)
#define _ADC_CHG_OT_RELEASE_               445    //unit: 1087 mV; 45 Celcius; Over temperature release for charging(Low divided voltage)
#define _ADC_UT_PROTECTION_                990    //unit: 2417 mV; 0 Celcius;  Under temperature protection(Low divided voltage)
#define _ADC_UT_RELEASE_                   931    //unit: 2272 mV; 5 Celcius;  Under temperature release(Low divided voltage)

//////////////////////////////////////////////////////////////////////////////
//=====================================================================================================================
#define _OC_PROTECTION_RELEASE_TIME_          5         // 5 sec; unit: sec.; 1byte; over current protection release time
#define _BUTTON_PRESS_TIME_                   5         // 5 sec; unit: sec.; 1byte; button press delay time
//=====================================================================================================================
#define _NO_DEFINE_                           0         // ; 1byte; 

// Sleep (suspend) Mode Var
#define _WAKE_UP_TIME_FROM_SUSPEND_MODE_          1 // 5 second; unit: sec.; 1byte; Wake Up Times from Suspending mode
#define _FIRST_TO_SUSPEND_MODE_IN_RELEASE_TIME_   60 // second; unit: sec.; 2byte; into SUSPEND_MODE time at first
//=====================================================================================================================
//=====================================================================================================================
#define _SOC_CELL_OV_VOLTAGE_             4250 //mV, PIC/ hw 4250
#define _SOC_CELL_UV_VOLTAGE_             3076 //mV, PIC/ hw 2500

//=====================================================================================================================
// SOC Cycle Count
// time based = 0.5 sec = 0.0001388f hours (1/7200, 1 hour = 60min. = 3600 sec.)
// sum(adc1+adc2+...) / current_mA_to_ADC_factor * time based = Q(mah)
// CYCLECOUNT_CAP_THRESHOLD = 2600*4p*0.9 = 9360 mAh ==> Cycle Count Threshold for total DSG/CHG capacity
// Using in Charging Status
// _ADC_CYCLECOUNT_THRESHOLD_ = 9360 mAh * _CHG_mA_to_ADC_factor_ (0.09280f) / time based(0.0001388f)
//#define _ADC_CYCLECOUNT_THRESHOLD_               11048508        // 2600*4p*0.9 = 9360 mAh; unit: maH.; 4byte; Cycle Count Threshold for total DSG capacity
#define _ADC_CYCLECOUNT_THRESHOLD_                  6257982        // 2600*4p*0.9 = 9360 mAh; unit: maH.; 4byte; Cycle Count Threshold for total CHG capacity
#define _CYCLECOUNT_FOR_CHG_LEVEL_1_                500         // 500 times; unit: times; 2byte; Cycle Count times for charger to set CHG voltage
#define _CYCLECOUNT_FOR_CHG_LEVEL_2_                1000        // 1000 times; unit: times; 2byte; Cycle Count times for charger to set CHG voltage
//=====================================================================================================================
// OCV TABLE Line Current Range, 4 ocv lines
// Battery for a cell capacity ==> 2600 mAH * 4p
// 0.5C DSG = 1300 mA, 1C DSG = 2600 mA, 2C DSG = 5200 mA
// 4p cell==> 0.5C DSG = 5200 mA, 1C DSG = 10400 mA, 2C DSG = 20800 mA
//  _DSG_mA_to_ADC_factor_      0.163839f (參考上面之設定)
// ADC 10 bit==> max adc = 1024
#define _ADC_LOOKUP_1st_OCV_CURRENT_RANGE_     425    // 2600mA; 2bytes;  CURRENT_OF_DSG_STATUS ~ 2600mA for 4p cell==>OCV
#define _ADC_LOOKUP_2nd_OCV_CURRENT_RANGE_     1010    //==>(1010=6164mA) 7800mA; 2bytes; 2600mA ~ 7800mA for 4p cell==>0.5C
#define _ADC_LOOKUP_3rd_OCV_CURRENT_RANGE_     2555   // 15600mA; 2bytes; 7800mA ~ 15600mA 以上 for 4p cell==1C
//=====================================================================================================================


#define SERIAL_NUMBER_offset                              0 	// 2 bytes
#define NUMBER_OF_PARALLEL_CELLS_offset                   2 	// 1 bytes
#define NUMBER_OF_SERIES_CELLS_offset                     3 	// 1 bytes
#define ADC_DETECT_CURRENT_OF_DSG_STATUS_offset           4 	// 2 bytes
#define ADC_DETECT_CURRENT_OF_CHG_STATUS_offset           6 	// 2 bytes
#define ADC_DOC_PROTECTION_offset                         8 	// 2 bytes
#define ADC_COC_PROTECTION_offset                         10 	// 2 bytes
#define ADC_2ND_BATTERY_OV_PROTECTION_offset              12 	// 2 bytes
#define ADC_2ND_BATTERY_OV_RELEASE_offset                 14 	// 2 bytes
#define ADC_2ND_BATTERY_UV_PROTECTION_offset              16 	// 2 bytes
#define ADC_2ND_BATTERY_UV_RELEASE_offset                 18 	// 2 bytes
#define ADC_DSG_OT_PROTECTION_offset                      20 	// 2 bytes
#define ADC_DSG_OT_RELEASE_offset                         22 	// 2 bytes
#define ADC_CHG_OT_PROTECTION_offset                      24 	// 2 bytes
#define ADC_CHG_OT_RELEASE_offset                         26 	// 2 bytes
#define ADC_UT_PROTECTION_offset                          28 	// 2 bytes
#define ADC_UT_RELEASE_offset                             30 	// 2 bytes
#define OC_PROTECTION_RELEASE_TIME_offset                 32 	// 1 bytes
#define BUTTON_PRESS_TIME_offset                          33 	// 1 bytes
#define NO_DEFINE_offset                                  34 	// 1 bytes
#define WAKE_UP_TIME_FROM_SUSPEND_MODE_offset             35 	// 1 bytes
#define FIRST_TO_SUSPEND_MODE_IN_RELEASE_TIME_offset      36 	// 2 bytes

#define SOC_CELL_OV_VOLTAGE_offset                        38 	// 2 bytes
#define SOC_CELL_UV_VOLTAGE_offset                        40 	// 2 bytes

#define ADC_CYCLECOUNT_THRESHOLD_offset                   42 	// 4 bytes
#define CYCLECOUNT_FOR_CHG_LEVEL_1_offset                 46 	// 2 bytes
#define CYCLECOUNT_FOR_CHG_LEVEL_2_offset                 48 	// 2 bytes
#define ADC_LOOKUP_1st_OCV_CURRENT_RANGE_offset           50 	// 2 bytes
#define ADC_LOOKUP_2nd_OCV_CURRENT_RANGE_offset           52 	// 2 bytes
#define ADC_LOOKUP_3rd_OCV_CURRENT_RANGE_offset           54 	// 2 bytes



#define SERIAL_NUMBER                             *((unsigned int *)(CONFIG_SEGMENT + SERIAL_NUMBER_offset                        ))  // 2 bytes
#define NUMBER_OF_PARALLEL_CELLS                  *((unsigned char *)(CONFIG_SEGMENT + NUMBER_OF_PARALLEL_CELLS_offset             ))  // 1 bytes
#define NUMBER_OF_SERIES_CELLS                    *((unsigned char *)(CONFIG_SEGMENT + NUMBER_OF_SERIES_CELLS_offset               ))  // 1 bytes
#define ADC_DETECT_CURRENT_OF_DSG_STATUS          *((unsigned int *)(CONFIG_SEGMENT + ADC_DETECT_CURRENT_OF_DSG_STATUS_offset     ))  // 2 bytes
#define ADC_DETECT_CURRENT_OF_CHG_STATUS          *((unsigned int *)(CONFIG_SEGMENT + ADC_DETECT_CURRENT_OF_CHG_STATUS_offset     ))  // 2 bytes
#define ADC_DOC_PROTECTION                        *((unsigned int *)(CONFIG_SEGMENT + ADC_DOC_PROTECTION_offset                   ))  // 2 bytes
#define ADC_COC_PROTECTION                        *((unsigned int *)(CONFIG_SEGMENT + ADC_COC_PROTECTION_offset                   ))  // 2 bytes
#define ADC_2ND_BATTERY_OV_PROTECTION             *((unsigned int *)(CONFIG_SEGMENT + ADC_2ND_BATTERY_OV_PROTECTION_offset        ))  // 2 bytes
#define ADC_2ND_BATTERY_OV_RELEASE                *((unsigned int *)(CONFIG_SEGMENT + ADC_2ND_BATTERY_OV_RELEASE_offset           ))  // 2 bytes
#define ADC_2ND_BATTERY_UV_PROTECTION             *((unsigned int *)(CONFIG_SEGMENT + ADC_2ND_BATTERY_UV_PROTECTION_offset        ))  // 2 bytes
#define ADC_2ND_BATTERY_UV_RELEASE                *((unsigned int *)(CONFIG_SEGMENT + ADC_2ND_BATTERY_UV_RELEASE_offset           ))  // 2 bytes
#define ADC_DSG_OT_PROTECTION                     *((unsigned int *)(CONFIG_SEGMENT + ADC_DSG_OT_PROTECTION_offset                ))  // 2 bytes
#define ADC_DSG_OT_RELEASE                        *((unsigned int *)(CONFIG_SEGMENT + ADC_DSG_OT_RELEASE_offset                   ))  // 2 bytes
#define ADC_CHG_OT_PROTECTION                     *((unsigned int *)(CONFIG_SEGMENT + ADC_CHG_OT_PROTECTION_offset                ))  // 2 bytes
#define ADC_CHG_OT_RELEASE                        *((unsigned int *)(CONFIG_SEGMENT + ADC_CHG_OT_RELEASE_offset                   ))  // 2 bytes
#define ADC_UT_PROTECTION                         *((unsigned int *)(CONFIG_SEGMENT + ADC_UT_PROTECTION_offset                    ))  // 2 bytes
#define ADC_UT_RELEASE                            *((unsigned int *)(CONFIG_SEGMENT + ADC_UT_RELEASE_offset                       ))  // 2 bytes
#define OC_PROTECTION_RELEASE_TIME                *((unsigned char *)(CONFIG_SEGMENT + OC_PROTECTION_RELEASE_TIME_offset           ))  // 1 bytes
#define BUTTON_PRESS_TIME                         *((unsigned char *)(CONFIG_SEGMENT + BUTTON_PRESS_TIME_offset                    ))  // 1 bytes
#define NO_DEFINE                                 *((unsigned char *)(CONFIG_SEGMENT + NO_DEFINE_offset                            ))  // 1 bytes
#define WAKE_UP_TIME_FROM_SUSPEND_MODE            *((unsigned char *)(CONFIG_SEGMENT + WAKE_UP_TIME_FROM_SUSPEND_MODE_offset       ))  // 1 bytes
#define FIRST_TO_SUSPEND_MODE_IN_RELEASE_TIME     *((unsigned int *)(CONFIG_SEGMENT + FIRST_TO_SUSPEND_MODE_IN_RELEASE_TIME_offset))  // 2 bytes

#define SOC_CELL_OV_VOLTAGE                       *((unsigned int *)(CONFIG_SEGMENT + SOC_CELL_OV_VOLTAGE_offset                  ))  // 2 bytes
#define SOC_CELL_UV_VOLTAGE                       *((unsigned int *)(CONFIG_SEGMENT + SOC_CELL_UV_VOLTAGE_offset                  ))  // 2 bytes

#define ADC_CYCLECOUNT_THRESHOLD                  *((unsigned long *)(CONFIG_SEGMENT + ADC_CYCLECOUNT_THRESHOLD_offset             ))  // 4 bytes
#define CYCLECOUNT_FOR_CHG_LEVEL_1                *((unsigned int *)(CONFIG_SEGMENT + CYCLECOUNT_FOR_CHG_LEVEL_1_offset           ))  // 2 bytes
#define CYCLECOUNT_FOR_CHG_LEVEL_2                *((unsigned int *)(CONFIG_SEGMENT + CYCLECOUNT_FOR_CHG_LEVEL_2_offset           ))  // 2 bytes

#define ADC_LOOKUP_1st_OCV_CURRENT_RANGE          *((unsigned int *)(CONFIG_SEGMENT + ADC_LOOKUP_1st_OCV_CURRENT_RANGE_offset     ))  // 2 bytes
#define ADC_LOOKUP_2nd_OCV_CURRENT_RANGE          *((unsigned int *)(CONFIG_SEGMENT + ADC_LOOKUP_2nd_OCV_CURRENT_RANGE_offset     ))  // 2 bytes
#define ADC_LOOKUP_3rd_OCV_CURRENT_RANGE          *((unsigned int *)(CONFIG_SEGMENT + ADC_LOOKUP_3rd_OCV_CURRENT_RANGE_offset     ))  // 2 bytes




//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
// Define LEV System  CALIBRATION_SEGMENT Data
// Used by SystemConfigInitialData.s43 to save into Flash Memory while downloading code into MCU
//////////////////////////////////////////////////////////////////////////////////////
#define _VERSION_		                          Version
#define _MINOR_VERSION_		                    MinorVersion
#define _MANUFACTURE_DATE_                    (MANUFACTURE_DATE_YEAR - 1980) * 512 + MANUFACTURE_DATE_MONTH * 32 + MANUFACTURE_DATE_DAY

#define _CHG_mA_To_ADC_Factor_          0.09280f        // 4 bytes;
#define _DSG_mA_To_ADC_Factor_          0.163839f       // 4 bytes;
#define _VBAT_mV_To_ADC_Factor_         0.016962f       // 4 bytes;
#define _Thermistor_mV_To_ADC_Factor_   0.4096f         // 4 bytes; = 1/ADC_Step
#define _CHG_OP_ADC_OFFSET_             0      //1byte ; 實際值-理論值 (signed char)
#define _DSG_OP_ADC_OFFSET_             0      //1byte ; 實際值-理論值 (signed char)
#define _VBAT_ADC_OFFSET_               0      //1byte ; 實際值-理論值 (signed char)
#define _NTC_ADC_OFFSET_                0      //1byte ; 實際值-理論值 Thermistor (signed char)
#define _CURRENT_CYCLECOUNT_            0      //; 2 bytes	
#define _CURRENT_ADC_ACCUMULATING_Q_    0      //; 4 bytes	



#define VERSION_offset                       		  0  //; 2 bytes	
#define MINOR_VERSION_offset                 		  2  //; 2 bytes	
#define MANUFACTURE_DATE_offset              			4  //; 2 bytes	
#define CHG_mA_To_ADC_Factor_offset         			6  //;	4 bytes
#define DSG_mA_To_ADC_Factor_offset         			10 //;	4 bytes
#define VBAT_mV_To_ADC_Factor_offset        			14 //;	4 bytes
#define Thermistor_mV_To_ADC_Factor_offset  			18 //;	4 bytes
#define CHG_OP_ADC_OFFSET_offset           				22 //;	1 byte
#define DSG_OP_ADC_OFFSET_offset           				23 //;	1 byte
#define VBAT_ADC_OFFSET_offset             				24 //;	1 byte
#define NTC_ADC_OFFSET_offset              				25 //;	1 byte

#define CURRENT_CYCLECOUNT_offset                 26  //; 2 bytes	
#define CURRENT_ADC_ACCUMULATING_Q_offset         28  //; 4 bytes	



////////////////////////////////////////////////////////////
//  LEV CALIBRATION Information definition
////////////////////////////////////////////////////////////
#define  VERSION                              *((unsigned int *)(CALIBRATION_SEGMENT + VERSION_offset                          ))
#define  MINOR_VERSION                        *((unsigned int *)(CALIBRATION_SEGMENT + MINOR_VERSION_offset                    ))
#define  MANUFACTURE_DATE                     *((unsigned int *)(CALIBRATION_SEGMENT + MANUFACTURE_DATE_offset                 ))
#define  CHG_mA_To_ADC_Factor                 *((float *)(CALIBRATION_SEGMENT + CHG_mA_To_ADC_Factor_offset         	   ))
#define  DSG_mA_To_ADC_Factor                 *((float *)(CALIBRATION_SEGMENT + DSG_mA_To_ADC_Factor_offset         	   ))
#define  VBAT_mV_To_ADC_Factor                *((float *)(CALIBRATION_SEGMENT + VBAT_mV_To_ADC_Factor_offset        	   ))
#define  Thermistor_mV_To_ADC_Factor          *((float *)(CALIBRATION_SEGMENT + Thermistor_mV_To_ADC_Factor_offset  	   ))
#define  CHG_OP_ADC_OFFSET                    *((signed char *)(CALIBRATION_SEGMENT + CHG_OP_ADC_OFFSET_offset           	   ))
#define  DSG_OP_ADC_OFFSET                    *((signed char *)(CALIBRATION_SEGMENT + DSG_OP_ADC_OFFSET_offset           	   ))
#define  VBAT_ADC_OFFSET                      *((signed char *)(CALIBRATION_SEGMENT + VBAT_ADC_OFFSET_offset             	   ))
#define  NTC_ADC_OFFSET                       *((signed char *)(CALIBRATION_SEGMENT + NTC_ADC_OFFSET_offset              	   ))

#define  CURRENT_CYCLECOUNT                 *((unsigned int *)(CALIBRATION_SEGMENT + CURRENT_CYCLECOUNT_offset   ))
#define  CURRENT_ADC_ACCUMULATING_Q         *((unsigned long *)(CALIBRATION_SEGMENT + CURRENT_ADC_ACCUMULATING_Q_offset   ))


