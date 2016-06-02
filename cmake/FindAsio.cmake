# Locate Asio library
# This module defines
#   ASIO_FOUND
#   ASIO_INCLUDE_DIR

find_path(ASIO_INCLUDE_DIR NAMES asio.hpp)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASIO DEFAULT_MSG ASIO_INCLUDE_DIR)

mark_as_advanced(ASIO_INCLUDE_DIR)
