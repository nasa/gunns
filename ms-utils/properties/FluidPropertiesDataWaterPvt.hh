#ifndef FluidPropertiesDataWaterPvt_EXISTS
#define FluidPropertiesDataWaterPvt_EXISTS

/**
@file
@brief  WATER_PVT Fluid Properties Data declarations

@defgroup  TSM_UTILITIES_PROPERTIES_FLUID_DATA_PVT_H2O    WATER_PVT Fluid Properties Data
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the class containing the fluid properties data of WATER_PVT.)

REQUIREMENTS:
- ()

REFERENCE:
- (NIST Chemistry WebBook)

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
- ((FluidPropertiesDataWaterPvt.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2016-06) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    WATER_PVT Fluid Properties Data.
///
/// @details  This is a bare-naked static data class.  It holds the density of liquid water as a
///           lookup table in temperature and pressure, and the temperature & pressure scales.
///           This is a subset of the hi-fi properties data currently under development.  Data comes
///           from the NIST Chemistry WebBook.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FluidPropertiesDataWaterPvt
{
    TS_MAKE_SIM_COMPATIBLE(FluidPropertiesDataWaterPvt);
    public:
        /// @brief  Enumeration of the scale length for all data.
        enum DataSizes {
            P_SCALE_SIZE = 39, ///< Defines the length of the pressure scale.
            T_SCALE_SIZE = 27  ///< Defines the length of the temperature scale.
        };
        static const double mScaleTemperature[T_SCALE_SIZE];          /**< (K)     Temperature scale breakpoints. */
        static const double mScalePressure[P_SCALE_SIZE];             /**< (kPa)   Pressure scale breakpoints. */
        static const double mDensityTable[T_SCALE_SIZE*P_SCALE_SIZE]; /**< (kg/m3) Density table data. */
        /// @brief  Default constructs this WATER_PVT Fluid Properties Data.
        FluidPropertiesDataWaterPvt();
        /// @brief  Default destructs this WATER_PVT Fluid Properties Data.
        virtual ~FluidPropertiesDataWaterPvt();
        /// @brief  Returns the pressure scale length.
        int getPscaleSize() const;
        /// @brief  Returns the temperature scale length.
        int getTscaleSize() const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        FluidPropertiesDataWaterPvt(const FluidPropertiesDataWaterPvt&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        FluidPropertiesDataWaterPvt& operator =(const FluidPropertiesDataWaterPvt&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int  (--)  Length of the pressure scale.
///
/// @details  Returns the length of the pressure scale.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int FluidPropertiesDataWaterPvt::getPscaleSize() const
{
    return P_SCALE_SIZE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int  (--)  Length of the temperature scale.
///
/// @details  Returns the length of the temperature scale.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int FluidPropertiesDataWaterPvt::getTscaleSize() const
{
    return T_SCALE_SIZE;
}

#endif
