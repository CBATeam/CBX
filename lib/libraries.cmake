# Include libraries here.
# Catch2 is included by deault, no need to re-add it here.

#set(build_static_lib ON CACHE BOOL "Build easyloggingpp as a static library")
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/easyloggingpp)

option(GLM_TEST_ENABLE_CXX_17 "Enable C++ 17" ON)
option(GLM_TEST_ENABLE "Build unit tests" OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glm)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/spdlog)
