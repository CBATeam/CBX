/**
 * @author Cliff Foster (Nou) <cliff@idi-systems.com>
 */
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#include "cbx_version.h"
//#include "app_version.h"
#include "version.h"

TEST_CASE("Version numbers are correctly set and returned.", "[common]") {
    SECTION("Public API calls.") {
        REQUIRE( @__cbx_vendor_namespace@_@__cbx_app_namespace@_get_version_major() == CBX_VERSION_MAJOR );
        REQUIRE( @__cbx_vendor_namespace@_@__cbx_app_namespace@_get_version_minor() == CBX_VERSION_MINOR );
        REQUIRE( @__cbx_vendor_namespace@_@__cbx_app_namespace@_get_version_patch() == CBX_VERSION_PATCH );
    }
}
