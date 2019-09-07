/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read animations. Originally developed for the ACE3 and ACRE2 projects.
 */

#include "read_helpers.hpp"
#include "animation.hpp"

#include <easylogging++.h>

using namespace cbx;

p3d::Animation::Animation(std::istream &stream_, uint32_t version_) {
    (void)version_;
    stream_.read((char*) &type, sizeof(uint32_t));

    name = read_string(stream_);
    source = read_string(stream_);

    stream_.read((char*) &min_value, sizeof(float32_t));
    stream_.read((char*) &max_value, sizeof(float32_t));
    stream_.read((char*) &min_phase, sizeof(float32_t));
    stream_.read((char*) &max_phase, sizeof(float32_t));

    stream_.read((char*) &junk, sizeof(uint32_t));
    stream_.read((char*) &junk2, sizeof(uint32_t));
    stream_.read((char*) &source_address, sizeof(uint32_t));

    // This always adds up to 2*4 more bytes unless its direct apparently
    switch (type) {
        // rotations
        case 0:
        case 1:
        case 2:
        case 3:
            stream_.read((char*) &angle0, sizeof(float32_t));
            stream_.read((char*) &angle1, sizeof(float32_t));
            break;
            // translations
        case 4:
        case 5:
        case 6:
        case 7: // also do the hide here, it'll always be 0
            stream_.read((char*) &offset0, sizeof(float32_t));
            stream_.read((char*) &offset1, sizeof(float32_t));
            break;
        case 8:
            float32_t pos[3];
            stream_.read((char*) &pos, sizeof(float32_t) * 3);
            direct_axis_pos = vec3_fp32_t(pos);

            float32_t dir[3];
            stream_.read((char*) &dir, sizeof(float32_t) * 3);
            direct_axis_dir = vec3_fp32_t(dir);

            stream_.read((char*) &direct_angle, sizeof(float32_t));
            stream_.read((char*) &direct_axis_offset, sizeof(float32_t));
            break;
        case 9: // fucking hides...
            stream_.read((char*) &hide_value, sizeof(float32_t));
            stream_.read((char*) &offset1, sizeof(float32_t)); // this is junk throw it in offset1 for hides
            break;
        default:
            stream_.read((char*) &offset0, sizeof(float32_t));
            stream_.read((char*) &offset1, sizeof(float32_t));
            break;
    }

    LOG(DEBUG) << "Animation loaded: " << name << ", source=" << source;
}
