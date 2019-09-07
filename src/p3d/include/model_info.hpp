/**
 * @author    Cliff Foster (Nou) <cliff@idi-systems.com>
 * @author    Ferran Obón Santacana (Magnetar) <ferran@idi-systems.com>
 * @author    Jaynus <jaynus@idi-systems.com>
 *
 * Read model information. Originally developed for the ACE3 and ACRE2 projects.
 */
#pragma once

#include "skeleton.hpp"
#include "vector.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace cbx {
    namespace p3d {
        enum class SBSource : uint32_t {
            Visual,
            ShadowVolume,
            Explicit,
            None
        };

        enum class MapType : uint32_t {
            Tree,
            SmallTree,
            Bush,
            Building,
            House,
            ForestBorder,
            ForestTriangle,
            ForestSquare,
            Church,
            Chapel,
            Cross,
            Rock,
            Bunker,
            Fortress,
            Fountain,
            ViewTower,
            Lighthouse,
            Quay,
            Fuelstation,
            Hospital,
            Fence,
            Wall,
            Hide, //default value
            BusStop,
            Road,
            Forest,
            Transmitter,
            Stack,
            Ruin,
            Tourism,
            Watertower,
            Track,
            MainRoad,
            Rocks,
            PowerLines,
            RailWay,
            PowerSolar,
            PowerWave,
            PowerWind,
            Shipwreck,
            NMapType
        };

        class ModelInfo {
        public:
            ModelInfo();
            ModelInfo(std::istream &, const uint32_t lod_count_, uint32_t version_ = 73);
            ~ModelInfo();

            std::vector<float32_t>  resolutions;
            float32_t   *raw_resolutions;      // LodTypes[Header.NoOfLods];// alias resolutions

            uint32_t    index;                 // appears to be a bit flag, 512, 256 eg
            float32_t   bounding_sphere;
            float32_t   geo_lod_sphere;        // mostly same as MemLodSphere
            uint32_t    point_flags[3];        // typically 00 00 00 00  00 00 00 00 00 00 0C 00 eg (last is same as user point flags)
            vec3_fp32_t aiming_center;         // Aiming center (previously offset_1)
            uint32_t    map_icon_color;        // RGBA 32 color
            uint32_t    map_selected_color;    // RGBA 32 color
            float32_t   view_density;          //

            vec3_fp32_t bbox_min_pos;          // minimum coordinates of bounding box
            vec3_fp32_t bbox_max_pos;          // maximum coordinates of bounding box. Generally the complement of the 1st
                                                        // pew.GeometryBounds in Pew is bboxMinPosition-bboxMaxPosition for X and Z
                                                        // pew.ResolutionBounds mostly the same
            float32_t   lod_density_coef;
            float32_t   draw_importance;

            vec3_fp32_t bbox_visual_min;
            vec3_fp32_t bbox_visual_max;
            vec3_fp32_t bounding_center;
            vec3_fp32_t geometry_center;
            vec3_fp32_t centre_of_mass;
            vec3_fp32_t inv_inertia[3];        // for ODOL7 this is a mixture of floats and index values
                                                       //// if Arma3 /////////////////
            ///////////////////////////////
            bool        autocenter;
            bool        lock_autocenter;
            bool        can_occlude;
            bool        can_be_occluded;
     
            bool        ai_cover;
            bool        force_not_alpha;
            SBSource    source;
            bool        prefer_shadow_volume;
            float32_t   shadow_offset;

            bool        animated;
            skeleton_p  skeleton;              //
            MapType             map_type;

            float32_t   mass;
            float32_t   mass_reciprocal;       // see note
            float32_t   armor;                 // see note
            float32_t   inv_armor;             // see note
            float32_t   explosion_shielding;

            uint8_t     special_lod_indeces[14];  // see note generally FF FF FF FF FF FF FF FF FF FF FF FF
            uint32_t    min_shadow;
            bool        can_blend;

            std::string class_type;            // asciiz      ClassType;                // class="House" See Named Properties
            std::string destruct_type;         //DestructType;             // damage="Tent" See Named Properties
            bool        property_frequent;     // rarely true

            uint32_t    number_graphical_lods;

            uint8_t     shadowVolume;
            uint8_t     shadowBuffer;
            uint32_t    shadowVolumeCount;
            uint32_t    shadowBufferCount;
            bool        u_bool_3;              // rarely true
            uint32_t    u_long_2;              //
            uint8_t     *default_indicators;   //default_indicators[NoOfLods][12]; //generally FF FF FF FF FF FF FF FF FF FF FF FF
        };
        using model_info_p = std::shared_ptr<ModelInfo>;
    }
}
