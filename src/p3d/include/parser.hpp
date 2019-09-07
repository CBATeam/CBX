/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Parse P3D information. Originally developed for the ACE3 and ACRE2 projects.
 */
#pragma once

#include "model.hpp"

#include <string>

namespace cbx {
    namespace p3d {
        class parser {
        public:
            parser() {};
            ~parser() {};

            model_p load(const std::string &filepath_);
        };
    }
}
