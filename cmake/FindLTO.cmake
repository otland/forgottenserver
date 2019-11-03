if(__FIND_LTO_CMAKE__)
    return()
endif()
set(__FIND_LTO_CMAKE__ TRUE)

include(CheckCXXCompilerFlag)
enable_language(CXX)

check_cxx_compiler_flag("-flto" COMPILER_KNOWS_LTO)
if(COMPILER_KNOWS_LTO)
    add_compile_options(-flto)
endif()
