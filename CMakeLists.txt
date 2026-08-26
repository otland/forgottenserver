cmake_minimum_required(VERSION 3.10)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(tfs CXX)

add_subdirectory(src)
add_executable(tfs ${tfs_SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set_target_properties(tfs PROPERTIES CXX_STANDARD 23)
set_target_properties(tfs PROPERTIES CXX_STANDARD_REQUIRED ON)

if (NOT WIN32)
    add_compile_options(-Wall -Werror -pipe -fvisibility=hidden)
endif ()

set(CMAKE_CXX_FLAGS_PERFORMANCE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fno-strict-aliasing)
endif ()

# Find packages.
# vcpkg builds Crypto++ with cryptopp-cmake, which installs a CMake config
# exporting cryptopp::cryptopp. Fall back to the bundled find module otherwise.
find_package(cryptopp CONFIG QUIET)
if (cryptopp_FOUND)
    set(Crypto++_LIBRARIES cryptopp::cryptopp)
else ()
    find_package(Crypto++ REQUIRED)
endif ()

# Look for vcpkg-provided libmariadb first
# If we link to the file directly, we might miss its dependencies from vcpkg
find_package(unofficial-libmariadb CONFIG QUIET)
if (unofficial-libmariadb_FOUND)
    set(MYSQL_CLIENT_LIBS unofficial::libmariadb)
else ()
    find_package(MySQL REQUIRED)
endif ()

find_package(Threads REQUIRED)
find_package(PugiXML REQUIRED)

# Selects LuaJIT if user defines or auto-detected
if (DEFINED USE_LUAJIT AND NOT USE_LUAJIT)
    set(FORCE_LUAJIT ${USE_LUAJIT})
else ()
    find_package(LuaJIT)
    set(FORCE_LUAJIT ${LuaJIT_FOUND})
endif ()
option(USE_LUAJIT "Use LuaJIT" ${FORCE_LUAJIT})

if (FORCE_LUAJIT)
    if (APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "-pagezero_size 10000 -image_base 100000000")
    endif ()
else ()
    find_package(Lua REQUIRED)
endif ()

# Only header-only parts of Boost are used (lexical_cast, variant).
find_package(Boost 1.66.0 REQUIRED)

# Standalone (non-Boost) Asio. vcpkg ships a CMake config with an asio::asio
# target; distribution packages (libasio-dev, asio-dev, ...) only install the
# headers.
find_package(asio CONFIG QUIET)
if (asio_FOUND)
    set(ASIO_LIBRARIES asio::asio)
else ()
    find_path(ASIO_INCLUDE_DIR asio.hpp)
    if (NOT ASIO_INCLUDE_DIR)
        message(FATAL_ERROR "Could not find standalone Asio (asio.hpp). Install it or set ASIO_INCLUDE_DIR.")
    endif ()
    set(ASIO_LIBRARIES "")
endif ()

include_directories(${Boost_INCLUDE_DIRS} ${ASIO_INCLUDE_DIR} ${Crypto++_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR} ${PUGIXML_INCLUDE_DIR})
target_link_libraries(tfs PRIVATE
        ${ASIO_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Crypto++_LIBRARIES}
        ${LUA_LIBRARIES}
        ${MYSQL_CLIENT_LIBS}
        ${PUGIXML_LIBRARIES}
        )

### INTERPROCEDURAL_OPTIMIZATION ###
cmake_policy(SET CMP0069 NEW)
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT error)
if (result)
    message(STATUS "IPO / LTO enabled")
    set_target_properties(tfs PROPERTIES INTERPROCEDURAL_OPTIMIZATION True)
else ()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif ()
### END INTERPROCEDURAL_OPTIMIZATION ###

### Git Version ###
# Define the two required variables before including
# the source code for watching a git repository.
option(SKIP_GIT "Skip checking for git updates" OFF)
if(NOT SKIP_GIT)
	set(PRE_CONFIGURE_FILE "cmake/gitmetadata.h.in")
	set(POST_CONFIGURE_FILE "${CMAKE_CURRENT_BINARY_DIR}/gitmetadata.h")
	include(git_watcher)
	if(Git_FOUND)
		add_dependencies(tfs check_git)
		target_include_directories(tfs PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
	endif()
endif()
### END  Git Version ###

# Precompiled header
# note: cotire() must be called last on a target
if (${CMAKE_VERSION} VERSION_GREATER "3.16.0")
    target_precompile_headers(tfs PUBLIC src/otpch.h)
else ()
    include(cotire)
    set_target_properties(tfs PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/otpch.h")
    set_target_properties(tfs PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
    cotire(tfs)
endif ()
