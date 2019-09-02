/**
 * @author Cliff Foster (Nou) <cliff@idi-systems.com>
 */

#include "@__cbx_new_component_name@.h"
#include "@__cbx_new_component_name@_public.h"

using namespace @__cbx_vendor_namespace@::@__cbx_app_namespace@;

namespace @__cbx_vendor_namespace@::@__cbx_app_namespace@::@__cbx_new_component_name@ {
    int component_function() {
        return 42;
    }
}

int @__cbx_new_component_name@_component_function() {
    return @__cbx_new_component_name@::component_function();
}
