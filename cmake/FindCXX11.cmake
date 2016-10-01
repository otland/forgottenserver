if(__FIND_CXX11_CMAKE__)
    return()
endif()
set(__FIND_CXX11_CMAKE__ TRUE)

include(CheckCXXCompilerFlag)
enable_language(CXX)

check_cxx_compiler_flag("-std=c++11" COMPILER_KNOWS_CXX11)
if(COMPILER_KNOWS_CXX11)
    add_compile_options(-std=c++11)

    # Tested on Mac OS X 10.8.2 with XCode 4.6 Command Line Tools
    # Clang requires this to find the correct c++11 headers
    check_cxx_compiler_flag("-stdlib=libc++" COMPILER_KNOWS_STDLIB)
    if(APPLE AND COMPILER_KNOWS_STDLIB)
        add_compile_options(-stdlib=libc++)
    endif()
else()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif()
