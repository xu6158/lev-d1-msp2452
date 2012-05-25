
/* ----------------------- Defines ------------------------------------------*/
/*! \brief If  5 LEDs TYPE is enabled. */
#define _5LEDs_TYPE_                                    (  1 )
/*! \brief If  5 LEDs TYPE and LEV 48V should be enabled , if 36V should be set '0'*/
#define _LEV_48_                                        (  1 )
/*! \brief If  Suspend Mode in Main Function should be enabled */
#define _SUSPEND_ENABLE_                                (  0 )
/*! \brief If  burn-in CALIBRATION data to  Flash_segment_C should be enabled */
#define _BURN_IN_CALIBRATION_SEGMENT_Flash_segment_C_   (  0 )
/*! \brief If  Erease CALIBRATION data at Flash_segment_C should be enabled */
#define _Erase_InformationMemory_At_Flash_segment_C_    (  0 )




/////////////////////////////////////////////////////////
#if _5LEDs_TYPE_ > 0

#if _LEV_48_ > 0
#include "SystemInformation_48V.h"
#else
#include "SystemInformation_36V.h"
#endif  //#ifdef _LEV_48_

#else



#endif  //#ifdef _5LEDs_TYPE_