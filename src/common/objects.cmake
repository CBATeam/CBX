#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#

# This is the list of all files that should be included in the core build.
# You _should_ include .h/.hpp files here so they properly show up in IDEs.

cbx_add_sources(
    "${CMAKE_CURRENT_LIST_DIR}/logging.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/vector.cpp"
    INTERNAL_CONFIGURE
    "${CMAKE_CURRENT_LIST_DIR}/version.cpp"
)

# This include brings in the explict cross-core includes. These are the APIs that
# should be used from component to component in the core library.
include("${CMAKE_CURRENT_LIST_DIR}/include/include.cmake")
