#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#

include("${CMAKE_CURRENT_LIST_DIR}/objects.cmake")

target_include_directories("${CBX_MAIN_TARGET}" PRIVATE "${CMAKE_CURRENT_LIST_DIR}")

include("${CMAKE_CURRENT_LIST_DIR}/tests/tests.cmake")
