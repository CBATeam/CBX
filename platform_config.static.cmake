#
# @author Cliff Foster (Nou) <cliff@idi-systems.com>
#
# This is a configuration file for the project platform. Set these values before
# configuration of the project. These values are also accessible via an included
# header file titled "platform_config.h"

# Set this to the name of the project. This will be the resultant binary name.
set(CBX_PROJECT_NAME "template_project")

# Set this to be the vendor namespace
set(CBX_VENDOR_NAMESPACE "cbx")

# Set this to be the primary namespace within the vendor namespace
set(CBX_APP_NAMESPACE "app")

# If CBX_IS_LIBRARY is set to 1 the project will be build as a library.
set(CBX_IS_LIBRARY 1)

# If CBX_IS_DYNAMIC is set to 1 then the project will build as a shared library.
# If it is set to 0 it will build as a static library.
set(CBX_IS_DYNAMIC 0)
