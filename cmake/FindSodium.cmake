# Locate Sodium library
# This module defines
#   Sodium_FOUND
#   Sodium_INCLUDE_DIR
#   Sodium_LIBRARIES

find_path(Sodium_INCLUDE_DIR NAMES sodium.h)
find_library(Sodium_LIBRARY NAMES sodium libsodium)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sodium DEFAULT_MSG Sodium_INCLUDE_DIR Sodium_LIBRARY)

mark_as_advanced(Sodium_INCLUDE_DIR Sodium_LIBRARY)
