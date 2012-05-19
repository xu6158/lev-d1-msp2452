

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
#define MANUFACTURE_DATE_MONTH  1
#define MANUFACTURE_DATE_DAY    6
#define PRODUCT_NAME            "LEV"
#define PRODUCT_NAME_LENGTH     3


//////////////////////////////////////////////////////////////////////////////////////////////////
// LEV System Initial config Data
// Used by SystemConfigInitialData.s43 to save into Flash Memory while downloading code into MCU
//////////////////////////////////////////////////////////////////////////////////////////////////

#define _VERSION_		                          1
#define _MINOR_VERSION_		                    0
#define _SERIAL_NUMBER_                       1
#define _MANUFACTURE_DATE_                    (MANUFACTURE_DATE_YEAR - 1980) * 512 + MANUFACTURE_DATE_MONTH * 32 + MANUFACTURE_DATE_DAY

#define _DETECT_CURRENT_OF_DSG_STATUS_        500       //500mA; unit: mA; if current > the define, in discharging status
#define _DETECT_CURRENT_OF_CHG_STATUS_        500       //500mA; unit: mA; if current > the define, in charging status
#define _DOC_PROTECTION_                      3000      // 30A; unit: 10mA; discharging current protection is positive
#define _COC_PROTECTION_                      500       // 5A; unit: 10mA; charging current protection
#define __2ND_BATTERY_OV_PROTECTION_          5460      // 54.6V; unit: 10mV; 2nd level BATTERY OV PROTECTION
#define __2ND_BATTERY_UV_PROTECTION_          3900      // 39.0V; unit: 10mV; 2nd level BATTERY UV PROTECTION


//=====================================================================================================================
// NTC Thermistor setting by voltage
////////////////////////////////////////////////////////////////
//    VDD (3.3V) |------Resistor1--+---NTC-----|GND     (Resistor1 = 10kR)
//                                 |
//                                 |
//                       thermal voltage output
////////////////////////////////////////////////////////////////
//NTC·Å«×(C)	NTC (kR)	output (mV)
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
//=====================================================================================================================
#define  _DSG_OT_PROTECTION_VOLTAGE_                  865         //unit: mV; 55 Celcius; Over temperature protection for discharging
#define  _DSG_OT_RELEASE_VOLTAGE_                     1087        //unit: mV; 45 Celcius; Over temperature release for discharging
#define  _CHG_OT_PROTECTION_VOLTAGE_                  865         //unit: mV; 55 Celcius; Over temperature protection for charging
#define  _CHG_OT_RELEASE_VOLTAGE_                     1087        //unit: mV; 45 Celcius; Over temperature release for charging
#define  _UT_PROTECTION_VOLTAGE_                      2417        //unit: mV; 0 Celcius;  Under temperature protection
#define  _UT_RELEASE_VOLTAGE_                         2272        //unit: mV; 5 Celcius;  Under temperature release
//=====================================================================================================================
#define _OC_PROTECTION_RELEASE_TIME_          5         // 5 sec; unit: sec.; 1byte; over current protection release time
#define _BUTTON_PRESS_TIME_                   5         // 5 sec; unit: sec.; 1byte; button press delay time
//=====================================================================================================================
#define _CHG_CV_MODE_LIMIT_VOLTAGE_         5000      // 50V; unit: 10mV; CV_Mode in CHG for High Limit Voltage
#define _CHG_CV_MODE_RELEASE_WAIT_TIME_     10        // 10 sec; unit: sec.; 2byte; CV_Mode in CHG for voltage release waiting time
#define _CHG_CV_MODE_REPEATING_CYCLE_       10        // 10 times/cycle; unit: times; 2byte; CV_Mode in CHG
//=====================================================================================================================
#define _WAKE_UP_TIME_FROM_SHIP_MODE_      5        // 10 second; unit: sec.; 2byte; Wake Up Times from Shipping mode






//////////////////////////////////////////////////////////////////////////////////////
//  Define flash Data address offset (range: 0~ 32 bytes)
//  the offset order below is followed by SystemConfigInitialData.s43 data order
//////////////////////////////////////////////////////////////////////////////////////
#define VERSION_offset                          0 
#define MINOR_VERSION_offset                    2 
#define SERIAL_NUMBER_offset                    4 
#define MANUFACTURE_DATE_offset                 6 
#define DETECT_CURRENT_OF_DSG_STATUS_offset     8 
#define DETECT_CURRENT_OF_CHG_STATUS_offset     10
#define DOC_PROTECTION_offset                   12
#define COC_PROTECTION_offset                   14
#define _2ND_BATTERY_OV_PROTECTION_offset       16
#define _2ND_BATTERY_UV_PROTECTION_offset       18
#define DSG_OT_PROTECTION_VOLTAGE_offset        20
#define DSG_OT_RELEASE_VOLTAGE_offset           22
#define CHG_OT_PROTECTION_VOLTAGE_offset        24
#define CHG_OT_RELEASE_VOLTAGE_offset           26
#define UT_PROTECTION_VOLTAGE_offset            28
#define UT_RELEASE_VOLTAGE_offset               30
#define OC_PROTECTION_RELEASE_TIME_offset       32  //;1byte
#define BUTTON_PRESS_TIME_offset                33  //;1byte
#define CHG_CV_MODE_LIMIT_VOLTAGE_offset        34  //;2bytes
#define CHG_CV_MODE_RELEASE_WAIT_TIME_offset    36  //;2bytes
#define CHG_CV_MODE_REPEATING_CYCLE_offset      38  //;2bytes
#define WAKE_UP_TIME_FROM_SHIP_MODE_offset      40  //;2bytes


////////////////////////////////////////////////////////////
//  LEV System Information
////////////////////////////////////////////////////////////
#define VERSION                           *((unsigned int *)(CONFIG_SEGMENT + VERSION_offset                     ))
#define MINOR_VERSION                     *((unsigned int *)(CONFIG_SEGMENT + MINOR_VERSION_offset               ))
#define SERIAL_NUMBER                     *((unsigned int *)(CONFIG_SEGMENT + SERIAL_NUMBER_offset               ))
#define MANUFACTURE_DATE                  *((unsigned int *)(CONFIG_SEGMENT + MANUFACTURE_DATE_offset            ))
#define DETECT_CURRENT_OF_DSG_STATUS      *((unsigned int *)(CONFIG_SEGMENT + DETECT_CURRENT_OF_DSG_STATUS_offset))
#define DETECT_CURRENT_OF_CHG_STATUS      *((unsigned int *)(CONFIG_SEGMENT + DETECT_CURRENT_OF_CHG_STATUS_offset))
#define DOC_PROTECTION                    *((unsigned int *)(CONFIG_SEGMENT + DOC_PROTECTION_offset              ))
#define COC_PROTECTION                    *((unsigned int *)(CONFIG_SEGMENT + COC_PROTECTION_offset              ))
#define _2ND_BATTERY_OV_PROTECTION        *((unsigned int *)(CONFIG_SEGMENT + _2ND_BATTERY_OV_PROTECTION_offset  ))
#define _2ND_BATTERY_UV_PROTECTION        *((unsigned int *)(CONFIG_SEGMENT + _2ND_BATTERY_UV_PROTECTION_offset  ))
#define DSG_OT_PROTECTION_VOLTAGE         *((unsigned int *)(CONFIG_SEGMENT + DSG_OT_PROTECTION_VOLTAGE_offset   ))
#define DSG_OT_RELEASE_VOLTAGE            *((unsigned int *)(CONFIG_SEGMENT + DSG_OT_RELEASE_VOLTAGE_offset      ))
#define CHG_OT_PROTECTION_VOLTAGE         *((unsigned int *)(CONFIG_SEGMENT + CHG_OT_PROTECTION_VOLTAGE_offset   ))
#define CHG_OT_RELEASE_VOLTAGE            *((unsigned int *)(CONFIG_SEGMENT + CHG_OT_RELEASE_VOLTAGE_offset      ))
#define UT_PROTECTION_VOLTAGE             *((unsigned int *)(CONFIG_SEGMENT + UT_PROTECTION_VOLTAGE_offset       ))
#define UT_RELEASE_VOLTAGE                *((unsigned int *)(CONFIG_SEGMENT + UT_RELEASE_VOLTAGE_offset          ))
#define OC_PROTECTION_RELEASE_TIME        *((unsigned char *)(CONFIG_SEGMENT + OC_PROTECTION_RELEASE_TIME_offset ))
#define BUTTON_PRESS_TIME                 *((unsigned char *)(CONFIG_SEGMENT + BUTTON_PRESS_TIME_offset          ))

#define CHG_CV_MODE_LIMIT_VOLTAGE         *((unsigned int *)(CONFIG_SEGMENT + CHG_CV_MODE_LIMIT_VOLTAGE_offset    ))
#define CHG_CV_MODE_RELEASE_WAIT_TIME     *((unsigned int *)(CONFIG_SEGMENT + CHG_CV_MODE_RELEASE_WAIT_TIME_offset))
#define CHG_CV_MODE_REPEATING_CYCLE       *((unsigned int *)(CONFIG_SEGMENT + CHG_CV_MODE_REPEATING_CYCLE_offset  ))

#define WAKE_UP_TIME_FROM_SHIP_MODE       *((unsigned int *)(CONFIG_SEGMENT + WAKE_UP_TIME_FROM_SHIP_MODE_offset  ))

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
// Define LEV System  CALIBRATION_SEGMENT Data
// Used by SystemConfigInitialData.s43 to save into Flash Memory while downloading code into MCU
//////////////////////////////////////////////////////////////////////////////////////
#define ADC_DETECT_CURRENT_OF_DSG_STATUS_offset     0
#define ADC_DETECT_CURRENT_OF_CHG_STATUS_offset     2
#define ADC_DOC_PROTECTION_offset                   4
#define ADC_COC_PROTECTION_offset                   6
#define ADC_2ND_BATTERY_OV_PROTECTION_offset        8
#define ADC_2ND_BATTERY_UV_PROTECTION_offset        10
#define ADC_DSG_OT_PROTECTION_offset                12
#define ADC_DSG_OT_RELEASE_offset                   14
#define ADC_CHG_OT_PROTECTION_offset                16
#define ADC_CHG_OT_RELEASE_offset                   18
#define ADC_UT_PROTECTION_offset                    20
#define ADC_UT_RELEASE_offset                       22
#define ADC_CHG_CV_MODE_LIMIT_VOLTAGE_offset        24

////////////////////////////////////////////////////////////
//  LEV CALIBRATION Information definition
////////////////////////////////////////////////////////////
#define  ADC_DETECT_CURRENT_OF_DSG_STATUS    *((unsigned int *)(CALIBRATION_SEGMENT + ADC_DETECT_CURRENT_OF_DSG_STATUS_offset))
#define  ADC_DETECT_CURRENT_OF_CHG_STATUS    *((unsigned int *)(CALIBRATION_SEGMENT + ADC_DETECT_CURRENT_OF_CHG_STATUS_offset))
#define  ADC_DOC_PROTECTION                  *((unsigned int *)(CALIBRATION_SEGMENT + ADC_DOC_PROTECTION_offset              ))
#define  ADC_COC_PROTECTION                  *((unsigned int *)(CALIBRATION_SEGMENT + ADC_COC_PROTECTION_offset              ))
#define  ADC_2ND_BATTERY_OV_PROTECTION       *((unsigned int *)(CALIBRATION_SEGMENT + ADC_2ND_BATTERY_OV_PROTECTION_offset   ))
#define  ADC_2ND_BATTERY_UV_PROTECTION       *((unsigned int *)(CALIBRATION_SEGMENT + ADC_2ND_BATTERY_UV_PROTECTION_offset   ))
#define  ADC_DSG_OT_PROTECTION               *((unsigned int *)(CALIBRATION_SEGMENT + ADC_DSG_OT_PROTECTION_offset           ))
#define  ADC_DSG_OT_RELEASE                  *((unsigned int *)(CALIBRATION_SEGMENT + ADC_DSG_OT_RELEASE_offset              ))
#define  ADC_CHG_OT_PROTECTION               *((unsigned int *)(CALIBRATION_SEGMENT + ADC_CHG_OT_PROTECTION_offset           ))
#define  ADC_CHG_OT_RELEASE                  *((unsigned int *)(CALIBRATION_SEGMENT + ADC_CHG_OT_RELEASE_offset              ))
#define  ADC_UT_PROTECTION                   *((unsigned int *)(CALIBRATION_SEGMENT + ADC_UT_PROTECTION_offset               ))
#define  ADC_UT_RELEASE                      *((unsigned int *)(CALIBRATION_SEGMENT + ADC_UT_RELEASE_offset                  ))
#define  ADC_CHG_CV_MODE_LIMIT_VOLTAGE       *((unsigned int *)(CALIBRATION_SEGMENT + ADC_CHG_CV_MODE_LIMIT_VOLTAGE_offset   ))

