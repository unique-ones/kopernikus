//
// MIT License
//
// Copyright (c) 2024 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef ASCOM_TELESCOPE_H
#define ASCOM_TELESCOPE_H

#include "device.h"

/// Specifies the geometry of the mount
typedef enum AlpacaAlignmentMode {
    /// Altitude-Azimuth alignment
    ALPACA_ALIGNMENT_ALT_AZ = 0,

    /// Polar (equatorial) mount other than German equatorial
    ALPACA_ALIGNMENT_POLAR = 1,

    /// German equatorial mount
    ALPACA_ALIGNMENT_GERMAN_POLAR = 2,
} AlpacaAlignmentMode;

/// Well known telescope tracking rates
typedef enum AlpacaDriveRate {
    /// Sidereal tracking rate (15.041 arcseconds per second)
    ALPACA_DRIVE_SIDEREAL = 0,

    /// Lunar tracking rate (14.685 arcseconds per second)
    ALPACA_DRIVE_LUNAR = 1,

    /// Solar tracking rate (15.0 arcseconds per second)
    ALPACA_DRIVE_SOLAR = 2,

    /// King tracking rate (15.0369 arcseconds per second)
    ALPACA_DRIVE_KING = 3,
} AlpacaDriveRate;

/// Equatorial coordinate systems used by telescopes
typedef enum AlpacaEquatorialType {
    /// Custom or unknown equinox and/or reference frame.
    ALPACA_EQUATORIAL_OTHER = 0,

    /// Topocentric coordinates. Coordinates of the object at the current
    /// date having allowed for annual aberreation, precession and nutation.
    /// This is the most common coordinate type for amateur telescopes.
    ALPACA_EQUATORIAL_TOPOCENTRIC = 1,

    /// J2000 equator/equinox. Coordinates of the object at mid-day on
    /// 1st January 2000, ICRS reference frame.
    ALPACA_EQUATORIAL_J2000 = 2,

    /// J2050 equator/equinox, Coordinates of the object at mid-day on
    /// 1st January 2050, ICRS reference frame.
    ALPACA_EQUATORIAL_J2050 = 3,

    /// B1950 equinox, FK4 reference frame.
    ALPACA_EQUATORIAL_B1950 = 4,
} AlpacaEquatorialType;

/// The direction in which the guide-rate motion is to be made.
typedef enum AlpacaGuideDirection {
    /// North (+ declination/altitude)
    ALPACA_GUIDE_NORTH = 0,

    /// South (- declination/altitude)
    ALPACA_GUIDE_SOUTH = 1,

    /// East (+ right ascension/azimuth)
    ALPACA_GUIDE_EAST = 2,

    /// West (- right ascension/azimuth)
    ALPACA_GUIDE_WEST = 3,
} AlpacaGuideDirection;

/// The pointing state of the mount
typedef enum AlpacaPierSide {
    /// Normal pointing state - Mount on the East side of pier (looking West)
    ALPACA_PIER_EAST = 0,

    /// Through the pole pointing state - Mount on the West side of pier (looking East)
    ALPACA_PIER_WEST = 1,

    /// Unknown or indeterminate
    ALPACA_PIER_UNKNOWN = -1,
} AlpacaPierSide;

/// The axis of the telescope
typedef enum AlpacaTelescopeAxis {
    /// Primary axis (e.g., Right Ascension or Azimuth)
    ALPACA_TELESCOPE_AXIS_PRIMARY = 0,

    /// Secondary axis (e.g., Declination or Altitude)
    ALPACA_TELESCOPE_AXIS_SECONDARY = 1,

    /// Tertiary axis (e.g. imager rotator/de-rotator)
    ALPACA_TELESCOPE_AXIS_TERTIARY = 2,
} AlpacaTelescopeAxis;

/// Describes a range of rates supported by the MoveAxis method
typedef struct AlpacaRate {
    f64 min;
    f64 max;
} AlpacaRate;

/// Tries to retrieve the mount's current altitude (°) above the horizon
/// @param device The telescope device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_telescope_altitude(AlpacaDevice *device, MemoryArena *arena, f64 *value);

/// Tries to retrieve the mount's current azimuth (°)
/// @param device The telescope device
/// @param arena The memory arena for the request
/// @param value The value that will be set
/// @return A result
AlpacaResult alpaca_telescope_azimuth(AlpacaDevice *device, MemoryArena *arena, f64 *value);

/// TODO(elias): unimplemented
/// AlignmentMode
/// ApertureArea
/// ApertureDiameter
/// AtHome
/// AtPark
/// CanFindHome
/// CanPark
/// CanPulseGuide
/// CanSetDeclinationRate
/// CanSetGuideRates
/// CanSetPark
/// CanSetPierSide
/// CanSetRightAscensionRate
/// CanSetTracking
/// CanSlew
/// CanSlewAsync
/// CanSlewAltAz
/// CanSlewAltAzAsync
/// CanSync
/// CanSyncAltAz
/// CanUnpark
/// Declination
/// DeclinationRate
/// DeclinationRate setter
/// DoesRefraction
/// DoesRefraction setter
/// EquatorialSystem
/// FocalLength
/// GuideRateDeclination
/// GuideRateDeclination setter
/// GuideRateAscensiion
/// GuideRateAscensiion setter
/// IsPulseGuiding
/// RightAscension
/// RightAscension setter
/// RightAscensionRate
/// RightAscensionRate setter
/// SideOfPier
/// SideOfPier setter
/// SiderealTime
/// SiteElevation
/// SiteElevation setter
/// SiteLatitude
/// SiteLatitude setter
/// SiteLongitude
/// SiteLongitude setter
/// Slewing
/// SlewSettleTime
/// SlewSettleTime setter
/// TargetDeclination
/// TargetDeclination setter
/// Tracking
/// Tracking setter
/// TrackingRates
/// UTCDate
/// UTCDate setter
/// AxisRates
/// CanMoveAxis
/// DestinationSideOfPier
/// AbortSlew
/// FindHome
/// MoveAxis
/// Park
/// PulseGuide
/// SetPark
/// SlewToAltAz
/// SlewToAltAyAsync
/// SlewToCoordinates
/// SlewToCoordinatesAsync
/// SlewToTarget
/// SlewToTargetAsync
/// SyncToAltAz
/// SyncToCoordinates
/// SyncToTarget
/// Unpark

#endif// ASCOM_TELESCOPE_H
