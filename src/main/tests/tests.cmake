#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#

# This is the list of all files that should be included in the unit test build.
# You _should_ include .h/.hpp files here so they properly show up in IDEs.

if(CBX_IS_LIBRARY AND CBX_IS_DYNAMIC)
    cbx_component_test_public(main "${CMAKE_CURRENT_LIST_DIR}/dll_test.cpp")
endif()
