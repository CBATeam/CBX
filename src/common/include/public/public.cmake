#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#

# This is the list of all files that should be included in the core build.
# You _should_ include .h/.hpp files here so they properly show up in IDEs.
# They should be marked as INTERFACE so that other libraries consuming the core
# can access them.
cbx_add_public_includes(
    "${CMAKE_CURRENT_LIST_DIR}/export.h"
    "${CMAKE_CURRENT_LIST_DIR}/version.h"
    EXTERNAL
    "${CMAKE_BINARY_DIR}/configured_templates/platform_config.h"
    INTERNAL_CONFIGURE
    "${CMAKE_CURRENT_LIST_DIR}/__version.h"
)

configure_file(${CMAKE_SOURCE_DIR}/templates/platform_config.in.h ${CMAKE_BINARY_DIR}/configured_templates/platform_config.h)
configure_file(${CMAKE_SOURCE_DIR}/cbx_version.h ${CMAKE_BINARY_DIR}/configured_templates/cbx_version.h)
target_include_directories("${CURRENT_LIBRARY_NAME}" PUBLIC "${CMAKE_BINARY_DIR}/configured_templates")
target_include_directories("${CBX_CORE}" PUBLIC "${CMAKE_BINARY_DIR}/configured_templates")
target_include_directories("${CBX_CORE}" INTERFACE "${CMAKE_BINARY_DIR}/configured_templates")
