/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read model information. Originally developed for the ACE3 and ACRE2 projects.
 */
#include "model_info.hpp"

#include "compressed.hpp"
#include "read_helpers.hpp"

using namespace cbx;

p3d::ModelInfo::ModelInfo() : raw_resolutions(nullptr), default_indicators(nullptr) { }

p3d::ModelInfo::ModelInfo(std::istream & stream_, const uint32_t lod_count_, uint32_t version_)
: raw_resolutions(nullptr), u_long_2(0), default_indicators(nullptr) {

    raw_resolutions = new float32_t[lod_count_];
    stream_.read((char *)raw_resolutions, sizeof(float32_t) * lod_count_);
    // Get them to parsable int values
    for (uint32_t x = 0; x < lod_count_; x++) {
        resolutions.push_back(raw_resolutions[x]);
    }

    stream_.read((char *)&index, sizeof(uint32_t));
    stream_.read((char *)&bounding_sphere, sizeof(float32_t));
    stream_.read((char *)&geo_lod_sphere, sizeof(float32_t));
    stream_.read((char *)&point_flags, sizeof(uint32_t) * 3);

    aiming_center = vec3_fp32_t(stream_);

    stream_.read((char *)&map_icon_color, sizeof(uint32_t));
    stream_.read((char *)&map_selected_color, sizeof(uint32_t));
    stream_.read((char *)&view_density, sizeof(float32_t));

    bbox_min_pos = vec3_fp32_t(stream_);
    bbox_max_pos = vec3_fp32_t(stream_);

    stream_.read((char *)&lod_density_coef, sizeof(float32_t));
    stream_.read((char *)&draw_importance, sizeof(float32_t));

    bbox_visual_min = vec3_fp32_t(stream_);
    bbox_visual_max = vec3_fp32_t(stream_);
    bounding_center = vec3_fp32_t(stream_);
    geometry_center = vec3_fp32_t(stream_);
    centre_of_mass = vec3_fp32_t(stream_);

    inv_inertia[0] = vec3_fp32_t(stream_);
    inv_inertia[1] = vec3_fp32_t(stream_);
    inv_inertia[2] = vec3_fp32_t(stream_);

    autocenter = read_bool(stream_);
    lock_autocenter = read_bool(stream_);
    can_occlude = read_bool(stream_);
    can_be_occluded = read_bool(stream_);
    if (version_ >= 73) {
        ai_cover = read_bool(stream_);
    }

    // Skeleton stuff
    float32_t tempBool;
    stream_.read((char *)&tempBool, sizeof(float32_t));
    stream_.read((char *)&tempBool, sizeof(float32_t));
    stream_.read((char *)&tempBool, sizeof(float32_t));
    stream_.read((char *)&tempBool, sizeof(float32_t));
    stream_.read((char *)&tempBool, sizeof(float32_t));
    stream_.read((char *)&tempBool, sizeof(float32_t));

    force_not_alpha = read_bool(stream_);
    stream_.read((char *) &source, sizeof(int32_t));

    prefer_shadow_volume = read_bool(stream_);
    stream_.read((char *)&shadow_offset, sizeof(float32_t));

    animated = read_bool(stream_);

    // Parse the full skeletal structure
    skeleton = std::make_shared<Skeleton>(stream_, lod_count_);

    stream_.read((char *)&map_type, sizeof(char));

    uint32_t n_floats;
    stream_.read((char *) &n_floats, sizeof(uint32_t));

    stream_.read((char *)&mass, sizeof(float32_t));
    stream_.read((char *)&mass_reciprocal, sizeof(float32_t));
    stream_.read((char *)&armor, sizeof(float32_t));
    stream_.read((char *)&inv_armor, sizeof(float32_t));

    if (version_ >= 72) {
        stream_.read((char *)&explosion_shielding, sizeof(float32_t));
    }

    stream_.read((char *)&special_lod_indeces, sizeof(uint8_t) * 14);

    stream_.read((char *)&min_shadow, sizeof(uint32_t));
    can_blend = read_bool(stream_);

    class_type = read_string(stream_);
    destruct_type = read_string(stream_);
    property_frequent = read_bool(stream_);

    uint32_t always_0;
    stream_.read((char *)&always_0, sizeof(uint32_t));

    for (uint32_t nLods = 0; nLods < lod_count_; nLods++) {
        uint8_t junk[12];
        stream_.read((char *)&junk, sizeof(uint8_t) * 12);
        std::cout << junk[12] << std::endl;
    }
}

p3d::ModelInfo::~ModelInfo() {
    if (raw_resolutions) {
        delete[] raw_resolutions;
    }
    if (default_indicators) {
        delete[] default_indicators;
    }
}
