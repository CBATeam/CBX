#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#

# This is the list of all files that should be included in the core build.
# You _should_ include .h/.hpp files here so they properly show up in IDEs.
cbx_add_includes(
    "${CMAKE_CURRENT_LIST_DIR}/constants.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/logging.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/transform_matrix.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/types.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/vector.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/version.hpp"
    INTERNAL_CONFIGURE
    "${CMAKE_CURRENT_LIST_DIR}/__version.hpp"
)

#target_include_directories("${CURRENT_LIBRARY_NAME}" SYSTEM PUBLIC
#        "${CBX_EXTERNAL_LIB_DIR}/easyloggingpp/src")

target_include_directories("${CURRENT_LIBRARY_NAME}" SYSTEM PUBLIC
        "${CBX_EXTERNAL_LIB_DIR}/spdlog/include")

# This include pulls in public header files. These are files that are declared public
# and should expose the public API used when linking against the core as a dynamic
# library.
include("${CMAKE_CURRENT_LIST_DIR}/public/public.cmake")
