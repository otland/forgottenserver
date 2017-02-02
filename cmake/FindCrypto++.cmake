# Locate Crypto++ library
# This module defines
#   Crypto++_FOUND
#   Crypto++_INCLUDE_DIR
#   Crypto++_LIBRARIES

find_path(Crypto++_INCLUDE_DIR NAMES cryptopp/cryptlib.h)
find_library(Crypto++_LIBRARY NAMES cryptopp libcryptopp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Crypto++ DEFAULT_MSG Crypto++_INCLUDE_DIR Crypto++_LIBRARY)

mark_as_advanced(Crypto++_INCLUDE_DIR Crypto++_LIBRARY)
