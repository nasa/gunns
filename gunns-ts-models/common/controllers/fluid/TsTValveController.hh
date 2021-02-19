#ifndef TsTValveController_EXISTS
#define TsTValveController_EXISTS

/**
@file
@brief     TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model declarations.

@defgroup  TSM_CONTROLLER_FLUID_T_VALVE_CONTROLLER  Manual T-Valve (3 way, 3 position) Controller Model
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model.)

@details
REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
- ((TsTValveController.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2013-09))

@{
*/

#include "common/controllers/fluid/TsValveController.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model Configuration Data.
///
/// @details  The sole purpose of this class is to provide a data structure for the TS21 Fluid
///           Controller Manual T-Valve (3 way, 3 position) Controller Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsTValveControllerConfigData
{
    public:
        TsValveControllerConfigData       mAPath;        /**< (--)  trick_chkpnt_io(**) A path valve controller. */
        TsValveControllerConfigData       mBPath;        /**< (--)  trick_chkpnt_io(**) B path valve controller. */
        /// @brief    Default constructs this Manual T-Valve Controller Model configuration data.
        TsTValveControllerConfigData(const TsValveControllerConfigData& aPath = TsValveControllerConfigData(),
                                     const TsValveControllerConfigData& bPath = TsValveControllerConfigData());
        /// @brief    Copy constructs this Manual T-Valve Controller Model configuration data.
        TsTValveControllerConfigData(const TsTValveControllerConfigData& that);
        /// @brief    Default destructs this Manual T-Valve Controller Model configuration data.
        ~TsTValveControllerConfigData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsTValveControllerConfigData& operator =(const TsTValveControllerConfigData&);
};

class TsTValveControllerInputData;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model.
///
/// @details  Provides the TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model.
///           - This controller has a pair of TsValveControllers, each of which is associated with
///             a GunnsFluidValve in the fluid aspect, sending them position, to allow flow through
///             one or the other or neither of the two paths.
///           - The individual valve positions may be either fully opened or fully closed, but not
///             at an intermediate position.
/// @verbatim
///            ___________________________________________
///           | Selected    | Path A Valve | Path B Valve |
///           | Position    | Position     | Position     |
///           |-------------|--------------|--------------|
///           | NONE        | Closed       | Closed       |
///           | A           | Open         | Closed       |
///           | B           | Closed       | Open         |
///           |_____________|______________|______________|
/// @endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsTValveController
{
    TS_MAKE_SIM_COMPATIBLE(TsTValveController);
    public:
        enum PositionType {
            NONE   = 0,  ///< Neither path selected (A valve closed and B valve closed)
            A_PATH = 1,  ///< A path selected       (A valve open   and B valve closed)
            B_PATH = 2   ///< B path selected       (A valve closed and B valve open)
        };
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        TsValveController                 mAPath;        /**< (--)  A path valve controller. */
        TsValveController                 mBPath;        /**< (--)  B path valve controller. */
        /// @}
        /// @brief    Default constructs this Manual T-Valve Controller Model.
        TsTValveController();
        /// @brief    Default destructs this Manual T-Valve Controller Model.
        virtual ~TsTValveController();
        /// @brief    Initializes this Manual T-Valve Controller Model.
        void initialize(const TsTValveControllerConfigData& config,
                        const TsTValveControllerInputData&  input,
                        const std::string&                  name);
        /// @brief    Updates this Manual T-Valve Controller Model.
        void update(const double dt);
        /// @brief    Sets this Manual T-Valve Controller model valve position selection.
        void setPosition(const TsTValveController::PositionType& position);
        /// @brief    Gets this Manual T-Valve Controller model valve position selection.
        TsTValveController::PositionType getPosition() const;
        /// @brief    Is this Manual T-Valve Controller Model initialized?
        bool isInitialized() const;
    protected:
        std::string                      mName;        /**< ** (--) trick_chkpnt_io(**) Object name (non-empty). */
        TsTValveController::PositionType mPosition;    /**<    (--)                     Valve position selection (NONE, A_PATH or B_PATH). */
        bool                             mInitialized; /**<    (--) trick_chkpnt_io(**) Initialization complete flag. */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsTValveController(const TsTValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsTValveController& operator=(const TsTValveController&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model Input Data.
///
/// @details  The sole purpose of this class is to provide a data structure for the TS21 Fluid
///           Controller Manual T-Valve (3 way, 3 position) Controller Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsTValveControllerInputData
{
    public:
        TsTValveController::PositionType  mPosition;     /**< (--)  Valve position selection (NONE, A_PATH or B_PATH). */
        TsValveControllerInputData        mAPath;        /**< (--)  A path valve controller. */
        TsValveControllerInputData        mBPath;        /**< (--)  B path valve controller. */
        /// @brief    Default constructs this Manual T-Valve Controller Model input data.
        TsTValveControllerInputData(const TsTValveController::PositionType& position = TsTValveController::NONE,
                                    const TsValveControllerInputData&       aPath    = TsValveControllerInputData(),
                                    const TsValveControllerInputData&       bPath    = TsValveControllerInputData());
        /// @brief    Copy constructs this Manual T-Valve Controller Model input data.
        TsTValveControllerInputData(const TsTValveControllerInputData& that);
        /// @brief    Default destructs this Manual T-Valve Controller Model input data.
        ~TsTValveControllerInputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsTValveControllerInputData& operator =(const TsTValveControllerInputData&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   position  (--)  Valve position selection (NONE, A_PATH or B_PATH).
///
/// @details  Sets the valve position selection for this TS21 Fluid Controller Manual T-Valve
///           (3 way, 3 position) Controller Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsTValveController::setPosition(const TsTValveController::PositionType& position)
{
    mPosition = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   TsTValveController::PositionType  (--)  Valve position selection (NONE, A_PATH or B_PATH).
///
/// @details  Gets the valve position selection for this TS21 Fluid Controller Manual T-Valve
///           (3 way, 3 position) Controller Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsTValveController::PositionType TsTValveController::getPosition() const
{
    return mPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     bool  (--)  True if initialization completed successfully, false otherwise.
///
/// @details    Gets this TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model
///             initialization complete status flag.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsTValveController::isInitialized() const
{
    return mInitialized;
}

#endif
