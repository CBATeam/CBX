/**
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 *
 * CBX constants.
 */

#pragma once

#include "types.hpp"

namespace cbx {
    constexpr float64_t CBX_PI         = 3.141592653589793238462643383279502884;   /*!< PI */
    constexpr float64_t CBX_PI_2       = 1.570796326794896619231321691639751442;   /*!< PI / 2 */
    constexpr float64_t CBX_PI_TIMES_2 = 6.283185307179586476925286766559005768;   /*!< PI * 2 */

    constexpr float64_t CBX_SQRT2      = 1.414213562373095048801688724209698079;       /*!<  sqrt(2.0) */

    constexpr float64_t RAD_TO_DEG  = 360.0 / CBX_PI_TIMES_2; /* Radians to degrees conversion */
    constexpr float64_t SPEED_LIGHT = 299792458.0; /*!< Speed of light in m/s */
}
