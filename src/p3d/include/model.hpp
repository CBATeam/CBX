/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read model data. Originally developed for the ACE3 and ACRE2 projects.
 */
#pragma once

#include "animation.hpp"
#include "lod_info.hpp"
#include "model_info.hpp"
#include "read_helpers.hpp"
#include "skeleton.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>

namespace cbx {
    namespace p3d {
        class FaceSettings {
        public:
            FaceSettings() :
                num_faces(0), icon_color(0), special(0), or_hint(0),
                skeleton_to_subskeleton_empty(false), num_points(0),
                face_area(0) {}
            FaceSettings(std::istream &stream_, uint32_t version_ = 73) {
                (void)version_;

                stream_.read((char *)&num_faces, sizeof(uint32_t));
                stream_.read((char *)&icon_color, sizeof(uint32_t));
                stream_.read((char *)&special, sizeof(uint32_t));
                stream_.read((char *)&or_hint, sizeof(uint32_t));
                skeleton_to_subskeleton_empty = read_bool(stream_);
                stream_.read((char *)&num_points, sizeof(uint32_t));
                stream_.read((char *)&face_area, sizeof(uint32_t));
            }

            uint32_t num_faces;
            uint32_t icon_color;
            uint32_t special;
            uint32_t or_hint;
            bool skeleton_to_subskeleton_empty;
            uint32_t num_points;
            uint32_t face_area;
        };
        using face_settings_p = std::shared_ptr<FaceSettings>;

        class Model {
        public:
            Model() : use_face_defaults(nullptr), info(nullptr) {};
            Model(std::istream &stream_, const std::string &filename_ = "");
            ~Model();

            // LOD info
            bool                                *use_face_defaults;
            std::vector<Face>                   default_faces;
            std::vector<lod_p>                  lods;

            std::streampos                      size;
            model_info_p                        info;
            skeleton_p                          skeleton;

            bool                                has_animations;
            std::vector<animation_p>            animations;

            std::vector<uint32_t>               start_lod;
            std::vector<uint32_t>               end_lod;

            // data root fields
            std::string                         filename;
            uint32_t                            lod_count;
            uint32_t                            filetype;
            uint32_t                            version;
            std::string                         prefix_name;
        };
        using model_p = std::shared_ptr<Model>;
    }
}
