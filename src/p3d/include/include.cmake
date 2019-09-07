#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#

# This is the list of all files that should be included in the component.
# You _should_ include .h/.hpp files here so they properly show up in IDEs.
cbx_add_includes(
    "${CMAKE_CURRENT_LIST_DIR}/animation.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/compressed_base.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/compressed.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/lod_info.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/lod_types.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/model_info.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/model.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/parser.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/read_helpers.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/skeleton.hpp"
)

# This include pulls in public header files. These are files that are declared public
# and should expose the public API used when linking against the component as a dynamic
# library.
include("${CMAKE_CURRENT_LIST_DIR}/public/public.cmake")
