/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Skeleton data. Originally developed for the ACE3 and ACRE2 projects.
 */
#include "model.hpp"
#include "read_helpers.hpp"
#include "skeleton.hpp"
#include "logging.hpp"

#include <sstream>

using namespace cbx;

p3d::Skeleton::Skeleton(std::istream & stream_, const uint32_t lod_count_) : size(0) {
    (void)lod_count_;

    name = read_string(stream_);
    if (name.length() <= 1) {
        return;
    }

    inherited = read_bool(stream_);
    stream_.read((char *)&size, sizeof(uint32_t));

    for (uint32_t x = 0; x < size; x++) {
        std::string _name, _parent;

        _name = read_string(stream_);
        _parent = read_string(stream_);

        all_bones.push_back(std::make_shared<Bone>(_name, _parent));
    }

    for (auto& tbone : all_bones) {
        for (auto& child : all_bones) {
            if (child->parent == tbone->name) {
                tbone->children.push_back(child->name);
            }
        }

        if (tbone->parent.length() < 1) {
            root_bones[tbone->name] = tbone;
        }
    }

    // Skip a byte because!
    //stream_.seekg(1, stream_.cur);
    spd_logging::logger->debug("Skeleton loaded {0}", name);
    char junk;
    stream_.read((char *)&junk, sizeof(uint8_t));
}
