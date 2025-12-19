cmake_minimum_required(VERSION 3.30)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Ensure to pick up the default triplet from the environment if any. This helps
# driving the vcpkg triplet in the same way either when starting vcpkg directly,
# or when letting CMake start vcpkg at configure/generate time.
# Note: this logic must happen before PROJECT command.
if (DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "The vcpkg triplet")
endif()

set(VCPKG_MANIFEST_NO_DEFAULT_FEATURES ON)

set(BOOST_REQUIRED_VERSION 1.71.0)

option(ENABLE_HTTP "Enable HTTP support" ON)
if (ENABLE_HTTP)
    list(APPEND VCPKG_MANIFEST_FEATURES "http")
    set(BOOST_REQUIRED_VERSION 1.75.0)
    add_compile_definitions(HTTP)
endif()

option(USE_LIBMYSQL "Use libmysql instead of libmariadb (vcpkg only)" OFF)
if (USE_LIBMYSQL)
    list(APPEND VCPKG_MANIFEST_FEATURES "libmysql")
else()
    list(APPEND VCPKG_MANIFEST_FEATURES "libmariadb")
endif()

option(BUILD_TESTING "Build the testing tree" OFF)
if (BUILD_TESTING)
    list(APPEND VCPKG_MANIFEST_FEATURES "unit-tests")
endif()

option(ENABLE_ASAN "Enable address sanitizer" OFF)
if (ENABLE_ASAN)
    if (MSVC)
        message(WARNING "ASAN is experimental on Windows/MSVC")
        add_compile_options(/fsanitize=address)
        add_link_options(/fsanitize=address)
    else ()
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif ()
endif ()

project(tfs CXX)

if (BUILD_TESTING)
    include(CTest)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT WIN32)
    # zlib 1.3.1 has an old-style cast
    add_compile_options(-Wall -Wextra -Wnon-virtual-dtor -Wno-error=old-style-cast -pedantic -Werror -pipe -fvisibility=hidden)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fno-strict-aliasing)

    if (APPLE)
        # MacOS SDK is not compatible with GCC
        # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=116827
        set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
    endif ()
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wimplicit-fallthrough -Wmove)
endif ()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Lua REQUIRED)
find_package(OpenSSL 3.0.0 REQUIRED COMPONENTS Crypto)
find_package(PugiXML CONFIG REQUIRED)
find_package(simdutf CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

# Look for vcpkg-provided packages first
# If we link to the file directly, we might miss its dependencies from vcpkg
if (USE_LIBMYSQL)
    find_package(unofficial-libmysql CONFIG)
    if (unofficial-libmysql_FOUND)
        set(MYSQL_CLIENT_LIBS "unofficial::libmysql::libmysql")
    endif ()
else ()
    find_package(unofficial-libmariadb CONFIG)
    if (unofficial-libmariadb_FOUND)
        set(MYSQL_CLIENT_LIBS "unofficial::libmariadb")
    endif ()
endif ()

if (NOT MYSQL_CLIENT_LIBS)
    message(STATUS "Using MySQL client library from system")
    find_package(MySQL REQUIRED)
endif ()

if (APPLE)
    find_package(Iconv REQUIRED)
endif()

set(BOOST_REQUIRED_COMPONENTS system iostreams)
if (ENABLE_HTTP)
    list(APPEND BOOST_REQUIRED_COMPONENTS json)
endif ()
if (BUILD_TESTING)
    list(APPEND BOOST_REQUIRED_COMPONENTS unit_test_framework)
endif ()
find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED COMPONENTS ${BOOST_REQUIRED_COMPONENTS})

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR} ${PUGIXML_INCLUDE_DIR})

option(ENABLE_UNITY_BUILD "Enable unity build" ON)
set(CMAKE_UNITY_BUILD ${ENABLE_UNITY_BUILD})

include(CheckIPOSupported)
check_ipo_supported(
    RESULT CMAKE_INTERPROCEDURAL_OPTIMIZATION
    OUTPUT INTERPROCEDURAL_OPTIMIZATION_ERROR
    LANGUAGES CXX
)
if (NOT CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    message(STATUS "IPO is not supported: ${INTERPROCEDURAL_OPTIMIZATION_ERROR}")
endif ()

add_subdirectory(src)
