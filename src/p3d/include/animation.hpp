/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read animations. Originally developed for the ACE3 and ACRE2 projects.
 */
#pragma once

#include "types.hpp"
#include "vector.hpp"

#include <memory>
#include <vector>
#include <string>

namespace cbx {
    namespace p3d {
        class AnimateBone {
        public:
            AnimateBone() : index(-1), lod(0) {}
            int32_t index;
            uint32_t lod;
            vec3_fp32_t axis_direction;
            vec3_fp32_t axis_position;
        };
        using animate_bone_p = std::shared_ptr<AnimateBone>;

        class Animation {
        public:
            Animation() {};
            Animation(std::istream &stream_, const uint32_t version_ = 73);
            ~Animation();

            uint32_t    type;
            std::string name;   // "RightDoor"
            std::string source; // "rotor"
            float32_t   min_value;
            float32_t   max_value;
            float32_t   min_phase;
            float32_t   max_phase;

            uint32_t    junk;
            uint32_t    junk2;
            uint32_t    source_address;
            float32_t   offset0;
            float32_t   offset1;
            float32_t   angle0;
            float32_t   angle1;

            float32_t   hide_value;

            vec3_fp32_t direct_axis_pos;
            vec3_fp32_t direct_axis_dir;

            float32_t direct_angle;
            float32_t direct_axis_offset;

            std::vector<animate_bone_p> bones;
        };
        using animation_p = std::shared_ptr<Animation>;
    }
}
