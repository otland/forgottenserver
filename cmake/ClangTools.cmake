find_program(CLANG_FORMAT NAMES "clang-format")

message(STATUS "clang-format found: ${CLANG_FORMAT}")
add_custom_target(clang-format COMMAND ${CLANG_FORMAT} -i -style=file ${tfs_SRC} ${tfs_HDR})
