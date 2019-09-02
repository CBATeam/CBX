#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#

#####################################################################
# GLOBAL COMPILER OPTIONS                                           #
#####################################################################
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()


#####################################################################
# CORE LIBRARY                                                      #
#####################################################################
set(CBX_MAIN_TARGET "${CBX_PROJECT_NAME}")
set(CBX_CORE "cbx_core")
if(CBX_IS_LIBRARY AND CBX_IS_DYNAMIC)
    set(CBX_CORE "${CBX_MAIN_TARGET}")
    add_library("${CBX_CORE}" SHARED "")
else()
    add_library("${CBX_CORE}" STATIC "")
endif()
target_compile_features("${CBX_CORE}" PRIVATE cxx_std_17)
set_target_properties("${CBX_CORE}" PROPERTIES LINKER_LANGUAGE CXX)

#####################################################################
# CORE COMPONENTS                                                   #
#####################################################################

# List core components below via add_subdirectory
include("${CMAKE_CURRENT_LIST_DIR}/components.cmake")

if(DO_TEMPLATE_COMPONENT_TEST)
    add_subdirectory("unit_test_component")
endif()

#####################################################################
# MAIN TARGET                                                       #
#####################################################################
if((CBX_IS_LIBRARY AND CBX_IS_DYNAMIC) OR (NOT CBX_IS_LIBRARY))
    if(CBX_IS_DYNAMIC)
        set_target_properties("${CBX_MAIN_TARGET}" PROPERTIES CXX_VISIBILITY_PRESET hidden)
        set_target_properties("${CBX_MAIN_TARGET}" PROPERTIES C_VISIBILITY_PRESET hidden)

        target_sources(
            "${CBX_MAIN_TARGET}"
            PRIVATE
            # EDIT LIST FILES BELOW HERE
            "${CMAKE_CURRENT_LIST_DIR}/main/dll_main.cpp"
        )
        install(TARGETS "${CBX_MAIN_TARGET}"
                LIBRARY)
    else()
        add_executable("${CBX_MAIN_TARGET}" "")
        target_sources(
            "${CBX_MAIN_TARGET}"
            PRIVATE
            # EDIT LIST FILES BELOW HERE
            "${CMAKE_CURRENT_LIST_DIR}/main/main.cpp"
        )
        target_link_libraries("${CBX_MAIN_TARGET}" "${CBX_CORE}")
        target_compile_features("${CBX_MAIN_TARGET}" PRIVATE cxx_std_17)
        set_target_properties("${CBX_MAIN_TARGET}" PROPERTIES LINKER_LANGUAGE CXX)
        install(TARGETS "${CBX_MAIN_TARGET}"
            RUNTIME)
    endif()
    add_subdirectory("main")
    get_target_property(CBX_MAIN_TARGET_SOURCES "${CBX_MAIN_TARGET}" SOURCES)
        source_group(TREE ${CMAKE_CURRENT_LIST_DIR}
    FILES ${CBX_MAIN_TARGET_SOURCES})
else()
    install(TARGETS "${CBX_CORE}"
        LIBRARY)
endif()
