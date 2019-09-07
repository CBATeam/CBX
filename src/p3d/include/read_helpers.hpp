/**
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read helpers. Originally developed for the ACE3 and ACRE2 projects.
 */
#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

namespace cbx {
    namespace p3d {
        inline bool read_bool(std::istream &stream_) {
            uint8_t temp;
            stream_.read((char *)&temp, 1);
            return temp != 0;
        }

        inline std::string read_string(std::istream &stream_) {
            std::stringstream ss;
            for (uint_fast16_t x = 0; x < 2056; x++) {
                uint8_t byte = 0;
                stream_.read((char *)&byte, 1);
                if (byte == 0x00) {
                    break;
                }
                ss << byte;
            }
            return ss.str();
        }
    }
}
