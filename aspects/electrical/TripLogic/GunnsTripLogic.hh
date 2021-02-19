#ifndef GunnsTripLogic_EXISTS
#define GunnsTripLogic_EXISTS

/**
@file
@brief    GUNNS Trip Logic Model declarations

@defgroup  GUNNS_ELECTRICAL_TRIPS_LOGIC    Trip Logic Model
@ingroup   GUNNS_ELECTRICAL_TRIPS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Trip Logic Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsTripLogic.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-12) (Initial))

@{
*/

#include "core/GunnsBasicLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Trip Logic Model base class.
///
/// @details  Provides generic trip logic functions for a non-linear GUNNS link.  This base class
///           provides most of the functionality, however derived classes must implement the actual
///           logical relationship between the trip parameter and its limit (greater or less than,
///           etc.)
///
/// @note  This class is pure virtual and cannot be directly instantiated.  Users should instantiate
///        the derived classes, defined below.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsTripLogic
{
    TS_MAKE_SIM_COMPATIBLE(GunnsTripLogic);
    public:
        bool         mMalfInhibitTrip; /**< (--) Resets current trip and disables further trips. */
        bool         mMalfForceTrip;   /**< (--) Forces immediate trip. */
        /// @brief  Default destructor.
        virtual ~GunnsTripLogic();
        /// @brief  Initializes this GUNNS Trip Logic.
        void initialize(const float        limit,
                        const unsigned int priority,
                        const bool         isTripped);
        /// @brief  Checks for the trip condition and set the trip state.
        bool checkForTrip(GunnsBasicLink::SolutionResult& result,
                          const float param, const int convergedStep);
        /// @brief  Returns the current trip state of this GUNNS Trip Logic.
        bool isTripped() const;
        /// @brief  Resets the trip state to false.
        void resetTrip();
        /// @brief  Sets the trip limit to the given value.
        void setLimit(const float limit);
        /// @brief  Returns the the trip limit value.
        float getLimit() const;

    protected:
        float        mLimit;     /**<    (--) trick_chkpnt_io(**) Trip limit value to check against, zero disables checks. */
        unsigned int mPriority;  /**<    (--) trick_chkpnt_io(**) Network converged step trip priority. */
        bool         mIsTripped; /**<    (--)                     Trip has occurred. */
        /// @brief  Constructor is protected since this class cannot be directly instantiated.
        GunnsTripLogic();
        /// @brief  Pure virtual method to check the model parameter against the trip limit.
        virtual bool isTripCondition(const float param) = 0;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsTripLogic(const GunnsTripLogic& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsTripLogic& operator =(const GunnsTripLogic&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Trip Greater Than Logic Model class.
///
/// @details  Implements the "greater than" trip logic, which triggers the trip when the model
///           parameter is greater than the limit value.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsTripGreaterThan : public GunnsTripLogic
{
    TS_MAKE_SIM_COMPATIBLE(GunnsTripGreaterThan);
    public:
        /// @brief  Default constructs this GUNNS Trip Greater Than Logic.
        GunnsTripGreaterThan();
        /// @brief  Default destructs this GUNNS Trip Greater Than Logic.
        virtual ~GunnsTripGreaterThan();

    protected:
        /// @brief  Implements the greater-than value check.
        virtual bool isTripCondition(const float param);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsTripGreaterThan(const GunnsTripGreaterThan& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsTripGreaterThan& operator =(const GunnsTripGreaterThan&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Trip Less Than Logic Model class.
///
/// @details  Implements the "less than" trip logic, which triggers the trip when the model
///           parameter is less than the limit value.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsTripLessThan : public GunnsTripLogic
{
    TS_MAKE_SIM_COMPATIBLE(GunnsTripLessThan);
    public:
        /// @brief  Default constructs this GUNNS Trip Less Than Logic.
        GunnsTripLessThan();
        /// @brief  Default destructs this GUNNS Trip Less Than Logic.
        virtual ~GunnsTripLessThan();

    protected:
        /// @brief  Implements the less-than value check.
        virtual bool isTripCondition(const float param);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsTripLessThan(const GunnsTripLessThan& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsTripLessThan& operator =(const GunnsTripLessThan&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if this GUNNS Trip Logic is currently tripped, false otherwise.
///
/// @details  Returns the mIsTripped attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsTripLogic::isTripped() const
{
    return mIsTripped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the mIsTripped attribute to false.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsTripLogic::resetTrip()
{
    mIsTripped = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  limit  (--)  The new limit value to use.
///
/// @details  Sets the mLimit attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsTripLogic::setLimit(const float limit)
{
    mLimit = limit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  float  (--)  The trip limit value.
///
/// @details  Returns the value of the mLimit attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline float GunnsTripLogic::getLimit() const
{
    return mLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] param (--) The value to be checked against the trip limit.
///
/// @returns  bool (--) True if the sensed parameter value is greater than the trip limit.
///
/// @details  Updates the sensor and returns if its sensed value is greater than the trip limit.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsTripGreaterThan::isTripCondition(const float param)
{
    return param > mLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] param (--) The value to be checked against the trip limit.
///
/// @returns  bool (--) True if the sensed parameter value is less than the trip limit.
///
/// @details  Updates the sensor and returns if its sensed value is less than the trip limit.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsTripLessThan::isTripCondition(const float param)
{
    return param < mLimit;
}

#endif
