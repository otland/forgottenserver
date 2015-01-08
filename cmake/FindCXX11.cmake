if(__FIND_CXX11_CMAKE__)
    return()
endif()
set(__FIND_CXX11_CMAKE__ TRUE)

include(CheckCXXCompilerFlag)
enable_language(CXX)

check_cxx_compiler_flag("-std=c++11" COMPILER_KNOWS_CXX11)
if(COMPILER_KNOWS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    # Tested on Mac OS X 10.8.2 with XCode 4.6 Command Line Tools
    # Clang requires this to find the correct c++11 headers
    check_cxx_compiler_flag("-stdlib=libc++" COMPILER_KNOWS_STDLIB)
    if(APPLE AND COMPILER_KNOWS_STDLIB)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
else()
    check_cxx_compiler_flag("-std=c++0x" COMPILER_KNOWS_CXX0X)
    if(COMPILER_KNOWS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(FATAL_ERROR "Your C++ compiler does not support C++11.")
    endif()
endif()
