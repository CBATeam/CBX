/**
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 *
 * Base type definitions.
 */

#pragma once

#include <cstdint>

namespace cbx {
    using float64_t = double;
    using float32_t = float;

    enum class Result : uint32_t {
        ok,
        invalid_player = 0x00000100,
        not_found = 0x00FF0000,
        invalid_packet = 0xFF000000,
        not_implemented = 0xFFFFFFF0,
        error = 0xFFFFFFFF
    };
}
