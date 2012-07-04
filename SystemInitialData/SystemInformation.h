
/* ----------------------- Defines ------------------------------------------*/
/*! \brief If  5 LEDs and Thick TYPE is enabled. 
    «p«¬ 48V/36V 5LEDs  should be '1'.
    Á¡«¬ 36V  4LEDs     should be '0'.                      */
#define _5LEDs_Thick_TYPE_                              (  1 )
/*! \brief If  5 LEDs TYPE and LEV 48V should be enabled , if 36V should be set '0'*/
#define _LEV_48_                                        (  0 )
/*! \brief If  Suspend Mode in Main Function should be enabled */
#define _SUSPEND_ENABLE_                                (  1 )
/*! \brief If  burn-in CALIBRATION data to Flash_segment_C should be enabled */
#define _BURN_IN_CALIBRATION_SEGMENT_Flash_segment_C_   (  0 )
/*! \brief If  Erease CALIBRATION data at Flash_segment_C should be enabled */
#define _Erase_InformationMemory_At_Flash_segment_C_    (  0 )
/*! \brief Initial Delay 2 sec should be enabled at PRG beginning */ 
#define _INITIAL_2_Sec_Delay_                           (  0 )

/*! \brief If Set 5 LEDs and Thick TYPE is enabled then 
    _5LEDs_No_Separate_Display_ set as '1', use 5 LEDs to display
    _5LEDs_No_Separate_Display_ set as '0', use 4 green LEDs and 1 red Leds */
#define _5LEDs_No_Separate_Display_Temp_                 (  1 )


/////////////////////////////////////////////////////////
#if _5LEDs_Thick_TYPE_ > 0
#if _5LEDs_No_Separate_Display_Temp_  >   0
#define _5LEDs_No_Separate_Display_     1
#else
#define _5LEDs_No_Separate_Display_     0
#endif
#if _LEV_48_ > 0
#include "SystemInformation_Thick_48V.h"
#else
#include "SystemInformation_Thick_36V.h"
#endif  //#ifdef _LEV_48_

#else

#define _5LEDs_No_Separate_Display_     0
#include "SystemInformation_ThinType36V.h"

#endif  //#ifdef _5LEDs_Thick_TYPE_