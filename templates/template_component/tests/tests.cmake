#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#

# This is the list of all tests that should be included as unit test builds.
cbx_component_test(@__cbx_new_component_name@ "${CMAKE_CURRENT_LIST_DIR}/test.cpp")
cbx_component_test_public(@__cbx_new_component_name@ "${CMAKE_CURRENT_LIST_DIR}/test_public.cpp")
