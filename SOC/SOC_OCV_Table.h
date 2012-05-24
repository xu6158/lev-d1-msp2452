
//#include "..\SystemInitialData\SystemInformation.h"




/***********************************************
 * not put the data in memory
 * this is for SOC function to use
 ***********************************************/
//#define SOC_BATTERY_OV_VOLTAGE     __2ND_BATTERY_OV_PROTECTION_     //5600
//#define SOC_BATTERY_UV_VOLTAGE     __2ND_BATTERY_UV_PROTECTION_     //3700
//#define SOC_1st_CELL_OV_VOLTAGE     4250 //mV
//#define SOC_1st_CELL_UV_VOLTAGE     2500 //mV


#define OCV_TABLE_POINTS 27
#define NUM_OF_OCV_LINE  4
#define TERMINAL_VOLTAGE_INDEX 0
#define RESIDUAL_OCV_CAPACITY_INDEX 1
#define RESIDUAL_0P5C_CAPACITY_INDEX 2
#define RESIDUAL_1C_CAPACITY_INDEX 3
#define RESIDUAL_2C_CAPACITY_INDEX 4
/*
; TERMINAL_VOLTAGE units is mV
; RESIDUAL_CAPACITY units is percentage * 100
; Please sort from largest to smallest
// 2600mAh cell, 25 degree, 0.5C OCV Table
*/

const int RealOCVTable[OCV_TABLE_POINTS][NUM_OF_OCV_LINE+1] =

{
    { 4185,   10000,  	9994,  	9989,  	9971   },   //index = 00
    { 4087,    9103,  	9987,  	9989,  	9971   },   //index = 01
    { 3997,    8112,  	9496,  	9989,  	9971   },   //index = 02
    { 3941,    7376,  	8991,  	9857,  	9971   },   //index = 03
    { 3890,    6547,  	8499,  	9580,  	9971   },   //index = 04
    { 3843,    5754,  	8000,  	9198,  	9971   },   //index = 05
    { 3800,    5228,  	7501,  	8778,  	9943   },   //index = 06
    { 3761,    2981,  	6994,  	8382,  	9857   },   //index = 07
    { 3727,    2198,  	6511,  	8002,  	9765   },   //index = 08
    { 3695,    1880,  	6005,  	7625,  	9649   },   //index = 09
    { 3666,    1670,  	5499,  	7260,  	9499   },   //index = 10
    { 3641,    1493,  	5009,  	6923,  	9325   },   //index = 11
    { 3618,    1346,  	4510,  	6603,  	9086   },   //index = 12
    { 3596,    1192,  	4012,  	6281,  	8851   },   //index = 13
    { 3572,    1006,  	3513,  	5903,  	8555   },   //index = 14
    { 3544,     733,	  3015,  	5420,  	8230   },   //index = 15
    { 3511,     483,	  2515,  	4807,  	7783   },   //index = 16
    { 3473,     358,	  2009,  	4095,  	7306   },   //index = 17
    { 3432,     248,	  1518,  	3365,  	6744   },   //index = 18
    { 3381,     203,	  1012,  	2560,  	5978   },   //index = 19
    { 3291,     124,	   485, 	1409,  	4350   },   //index = 20
    { 3201,      46,	   271, 	 737, 	2806   },   //index = 21
    { 3111,       0,     169, 	 431, 	1621   },   //index = 22
    { 3021,       0,     103, 	 271, 	 792   },   //index = 23
    { 2931,       0,      59, 	 153, 	 349   },   //index = 24
    { 2841,       0,      22, 	  65, 	 139   },   //index = 25
    { 2748,       0,       0,     0,       0   }    //index = 26 
};

//#define TERMINAL_VOLTAGE_INDEX 0
//#define RESIDUAL_CAPACITY_INDEX 1
//
//const int RealOCVTable[OCV_TABLE_POINTS][2] =
//{
//  {  4087,  9994  },    //index=00,  1st	 
//  {  3997,  9496  },    //index=01,  2nd	 
//  {  3941,  8998  },    //index=02,  3rd	 
//  {  3890,  8499  },    //index=03,  4th	 
//  {  3843,  8000  },    //index=04,  5th	 
//  {  3800,  7501  },    //index=05,  6th  
//  {  3761,  7002  },    //index=06,  7th  
//  {  3727,  6504  },    //index=07,  8th  
//  {  3695,  6005  },    //index=08,  9th  
//  {  3666,  5507  },    //index=09,  10th 
//  {  3641,  5009  },    //index=10,  11th 
//  {  3618,  4510  },    //index=11,  12th 
//  {  3596,  4012  },    //index=12,  13th 
//  {  3572,  3513  },    //index=13,  14th 
//  {  3544,  3015  },    //index=14,  15th 
//  {  3511,  2515  },    //index=15,  16th 
//  {  3473,  2016  },    //index=16,  17th 
//  {  3432,  1518  },    //index=17,  18th 
//  {  3381,  1019  },    //index=18,  19th 
//  {  3299,  521   },    //index=19,  20th 
//  {  2748,  0     }     //index=20,  21th 
//};



