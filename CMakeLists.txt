cmake_minimum_required(VERSION 2.8)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(tfs)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "-Wall -Werror")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

include(FindCXX11)

# Find packages.
find_package(GMP REQUIRED)
find_package(MySQL)
find_package(LuaJIT)

if (LUAJIT_FOUND)
    add_definitions(-D__LUAJIT__)
else()
    find_package(Lua)
endif()

find_package(Boost 1.42.0 COMPONENTS system thread REQUIRED)

include(src/CMakeLists.txt)
add_executable(tfs ${tfs_SRC})

include_directories(${MYSQL_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${GMP_INCLUDE_DIR})
target_link_libraries(tfs ${MYSQL_CLIENT_LIBS} ${LUA_LIBRARIES} ${Boost_LIBRARIES} ${GMP_LIBRARIES})
