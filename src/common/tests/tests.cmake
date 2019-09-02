#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#

# This is the list of all files that should be included in the unit test build.
# You _should_ include .h/.hpp files here so they properly show up in IDEs.
cbx_configure_file(CONFIGURED_FILENAME "${CMAKE_CURRENT_LIST_DIR}/test.cpp")
cbx_component_test(common "${CONFIGURED_FILENAME}")
cbx_configure_file(CONFIGURED_FILENAME "${CMAKE_CURRENT_LIST_DIR}/test_public.cpp")
cbx_component_test_public(common "${CONFIGURED_FILENAME}")
