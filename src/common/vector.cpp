/**
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Vector of length 3 operations. Originally developed for the ACE3 and ACRE2 projects.
 */

#include "types.hpp"
#include "vector.hpp"

#include <cmath>

namespace cbx {
    template <> float32_t acos(float32_t n) { return std::acosf(n); }
    template <> float64_t acos(float64_t n) { return std::acos(n); }
    template <> float32_t cos(float32_t n) { return std::cosf(n); }
    template <> float64_t cos(float64_t n) { return std::cos(n); }
    template <> float32_t sin(float32_t n) { return std::sinf(n); }
    template <> float64_t sin(float64_t n) { return std::sin(n); }
}
