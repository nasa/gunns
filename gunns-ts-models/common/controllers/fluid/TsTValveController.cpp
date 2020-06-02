/**
@file
@brief    TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((common/controllers/fluid/TsValveController.o))
*/

#include "GenericMacros.hh"

#include "TsTValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  aPath  (--)  A path valve controller configuration data.
/// @param[in]  bPath  (--)  B path valve controller configuration data.
///
/// @details    Default constructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///             Controller Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsTValveControllerConfigData::TsTValveControllerConfigData(const TsValveControllerConfigData& aPath,
                                                           const TsValveControllerConfigData& bPath)
    :
    mAPath(aPath),
    mBPath(bPath)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details    Copy constructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///             Controller Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsTValveControllerConfigData::TsTValveControllerConfigData(const TsTValveControllerConfigData& that)
    :
    mAPath(that.mAPath),
    mBPath(that.mBPath)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///             Controller Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsTValveControllerConfigData::~TsTValveControllerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  position  (--)  Position selection input data (NONE, A_PATH or B_PATH).
/// @param[in]  aPath     (--)  A path valve controller input data.
/// @param[in]  bPath     (--)  B path valve controller input data.
///
/// @details    Default constructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///             Controller Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsTValveControllerInputData::TsTValveControllerInputData(const TsTValveController::PositionType& position,
                                                         const TsValveControllerInputData&       aPath,
                                                         const TsValveControllerInputData&       bPath)
    :
    mPosition(position),
    mAPath(aPath),
    mBPath(bPath)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///             Controller Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsTValveControllerInputData::TsTValveControllerInputData(const TsTValveControllerInputData& that)
    :
    mPosition(that.mPosition),
    mAPath(that.mAPath),
    mBPath(that.mBPath)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///             Controller Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsTValveControllerInputData::~TsTValveControllerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///             Controller Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsTValveController::TsTValveController()
    :
    mAPath(),
    mBPath(),
    mName(),
    mPosition(),
    mInitialized(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///             Controller Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsTValveController::~TsTValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config        (--)  Object configuration data.
/// @param[in]  input         (--)  Object input data.
/// @param[in]  name          (--)  Object name (non-empty).
///
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Initializes this TS21 Fluid Controller Manual T-Valve (3 way, 3 position)
///             Controller Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsTValveController::initialize(const TsTValveControllerConfigData& config,
                                    const TsTValveControllerInputData&  input,
                                    const std::string&                  name)
{
    /// - Reset the initialization complete flag.
    mInitialized = false;

    /// - Allocate and initialize the object name or throw a TsInitializationException exception if empty.
    TS_GENERIC_NAME_ERREX("TsTValveController");

    /// - Initialize the position from the input data.
    mPosition    = input.mPosition;

    /// - Initialize the A and B path valve controllers from the configuration and input data.
    mAPath.initialize(config.mAPath, input.mAPath, name + ".mAPath");
    mBPath.initialize(config.mBPath, input.mBPath, name + ".mBPath");

    /// - Initialize the outputs.
    update(0.0);

    /// - Set the initialization complete flag.
    mInitialized = mAPath.isInitialized() and mBPath.isInitialized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)   Time step.
///
/// @return     void
///
/// @details    Updates this TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller
///             Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsTValveController::update(const double dt)
{
    switch(mPosition) {
        case TsTValveController::NONE:
            /// - On NONE position selected, set the A path valve manual position closed and the B path valve manual position closed.
            mAPath.setManualPosition(true, mAPath.getMinPosition());
            mBPath.setManualPosition(true, mBPath.getMinPosition());
            break;
        case TsTValveController::A_PATH:
            /// - On A position selected,    set the A path valve manual position open   and the B path valve manual position closed.
            mAPath.setManualPosition(true, mAPath.getMaxPosition());
            mBPath.setManualPosition(true, mBPath.getMinPosition());
            break;
        case TsTValveController::B_PATH:
            /// - On B position selected,    set the A path valve manual position closed and the B path valve manual position open.
            mAPath.setManualPosition(true, mAPath.getMinPosition());
            mBPath.setManualPosition(true, mBPath.getMaxPosition());
            break;
        default:
            /// - On invalid source select, just issue an H&S warning.
            TS_GENERIC_WARNING("invalid position " << static_cast<int>(mPosition) << " selected, not changing valve positions.");
            break;
    }

    /// - In any case, update the A and B path valve controllers.
    mAPath.update(dt);
    mBPath.update(dt);
}
