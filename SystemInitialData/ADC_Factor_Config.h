
#define ADC_resolution  1024    // 10 bit ADC
#define ADC_Ref         2500    // mV
//#define ADC_Step         (float)ADC_Ref /  ADC_resolution   // mV
#define ADC_Step        2.44140625f
////////////////////////////////////////////////////////////////
// CHG / DSG currrent
////////////////////////////////////////////////////////////////
//mA to ADC factor 
//Factor = Rsense(R) * op_gain / ADC step(mV) => Multiplicative inverse Factor
////////////////////////////////////////////////////////////////
#define Rsense                  3.3333f    //mR
#define CHG_OP_Gain             68.0f
//#define CHG_mA_to_ADC_factor    (Rsense * CHG_OP_Gain / ADC_Step / 1000)==>小數點6位
#define _CHG_mA_to_ADC_factor_    0.09270f
//#define _CHG_mA_to_ADC_factor_    0.155752f
//#define _CHG_10mA_to_ADC_factor_  1.557524f   //CHG_mA_to_ADC_factor * 10
#define CHG_OP_ADC_OFFSET         (signed char)0    //實際值-理論值 

#define DSG_OP_Gain   68.0f
//#define DSG_mA_to_ADC_factor    (Rsense*DSG_OP_Gain/ADC_Step/1000)==>小數點6位
#define _DSG_mA_to_ADC_factor_      0.0927f
//#define _DSG_mA_to_ADC_factor_      0.026709f
//#define _DSG_10mA_to_ADC_factor_    0.267098f //DSG_mA_to_ADC_factor * 10
#define DSG_OP_ADC_OFFSET         (signed char)0    //實際值-理論值 

////////////////////////////////////////////////////////////////
//    VDD |------Resistor1--+--Resistor2----|GND
//                          |
//                          |
//                   VBAT output voltage
////////////////////////////////////////////////////////////////
//Factor = (R1+R2)/R2 * ADC_Step
#define VBAT_Resistor1            1000 //kR
#define VBAT_Resistor2            43.0f //kR
//#define VBAT_mV_To_ADC_Factor    (float)1/((VBAT_Resistor1+VBAT_Resistor2)/VBAT_Resistor2*ADC_Step)==>小數點6位
//#define ADC_Step        2.44140625f //  mv/step
//#define _VBAT_mV_To_ADC_Factor_     0.016886f
//#define _VBAT_10mV_To_ADC_Factor_   0.168866f   // VBAT_mV_To_ADC_Factor * 10
#define _VBAT_10mV_To_ADC_Factor_   0.1694f   // VBAT_mV_To_ADC_Factor * 10
#define VBAT_ADC_OFFSET          (signed char)(-5)   //實際值-理論值      

////////////////////////////////////////////////////////////////
// NTC Thermistor setting by voltage
////////////////////////////////////////////////////////////////
#define Thermistor_mV_To_ADC_Factor     0.4096f   // = 1/ADC_Step

