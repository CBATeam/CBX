/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Parse P3D information. Originally developed for the ACE3 and ACRE2 projects.
 */

#include "parser.hpp"

#include <easylogging++.h>
#include <fstream>
#include <memory>

using namespace cbx;

p3d::model_p p3d::parser::load(const std::string &filepath_) {
    std::fstream filestream;
    model_p _model = nullptr;

    filestream.open(filepath_, std::ios::binary | std::ios::in);
    if (!filestream.good()) {
        LOG(ERROR) << "Cannot open file";
        return _model;
    }

    _model = std::make_shared<Model>(filestream, filepath_);

    return _model;
}
