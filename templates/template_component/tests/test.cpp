/**
 * @author Cliff Foster (Nou) <cliff@idi-systems.com>
 */
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#include "@__cbx_new_component_name@.h"

TEST_CASE("Template component test CHANGE ME!!!!!!!!!", "[@__cbx_new_component_name@]") {
    SECTION("Internal API calls.") {
        REQUIRE( @__cbx_vendor_namespace@::@__cbx_app_namespace@::@__cbx_new_component_name@::component_function() == 42 );
    }
}
