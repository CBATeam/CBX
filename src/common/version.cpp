/**
 * @author Cliff Foster (Nou) <cliff@idi-systems.com>
 */

#include "version.hpp"
#include "cbx_version.h"

using namespace @__cbx_vendor_namespace@::@__cbx_app_namespace@;

int common::get_version_major() {
    return CBX_VERSION_MAJOR;
}

int common::get_version_minor() {
    return CBX_VERSION_MINOR;
}

int common::get_version_patch() {
    return CBX_VERSION_PATCH;
}

int @__cbx_vendor_namespace@_@__cbx_app_namespace@_get_version_major() {
    return common::get_version_major();
}

int @__cbx_vendor_namespace@_@__cbx_app_namespace@_get_version_minor() {
    return common::get_version_minor();
}

int @__cbx_vendor_namespace@_@__cbx_app_namespace@_get_version_patch() {
    return common::get_version_patch();
}
