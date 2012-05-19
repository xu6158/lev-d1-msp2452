
#include "..\SystemInitialData\SystemInformation.h"




/***********************************************
 * not put the data in memory
 * this is for SOC function to use
 ***********************************************/
//#define SOC_BATTERY_OV_VOLTAGE     __2ND_BATTERY_OV_PROTECTION_     //5600
//#define SOC_BATTERY_UV_VOLTAGE     __2ND_BATTERY_UV_PROTECTION_     //3700
//#define SOC_1st_CELL_OV_VOLTAGE     4250 //mV
//#define SOC_1st_CELL_UV_VOLTAGE     2500 //mV


#define OCV_TABLE_POINTS 21
#define TERMINAL_VOLTAGE_INDEX 0
#define RESIDUAL_CAPACITY_INDEX 1

/*
; TERMINAL_VOLTAGE units is mV
; RESIDUAL_CAPACITY units is percentage * 100
; Please sort from largest to smallest
// 2600mAh cell, 25 degree, 0.5C OCV Table
*/
const int RealOCVTable[OCV_TABLE_POINTS][2] =
{
  {  4087,  9994  },    //index=00,  1st	 
  {  3997,  9496  },    //index=01,  2nd	 
  {  3941,  8998  },    //index=02,  3rd	 
  {  3890,  8499  },    //index=03,  4th	 
  {  3843,  8000  },    //index=04,  5th	 
  {  3800,  7501  },    //index=05,  6th  
  {  3761,  7002  },    //index=06,  7th  
  {  3727,  6504  },    //index=07,  8th  
  {  3695,  6005  },    //index=08,  9th  
  {  3666,  5507  },    //index=09,  10th 
  {  3641,  5009  },    //index=10,  11th 
  {  3618,  4510  },    //index=11,  12th 
  {  3596,  4012  },    //index=12,  13th 
  {  3572,  3513  },    //index=13,  14th 
  {  3544,  3015  },    //index=14,  15th 
  {  3511,  2515  },    //index=15,  16th 
  {  3473,  2016  },    //index=16,  17th 
  {  3432,  1518  },    //index=17,  18th 
  {  3381,  1019  },    //index=18,  19th 
  {  3299,  521   },    //index=19,  20th 
  {  2748,  0     }     //index=20,  21th 
};



