set(tests_SRC
    ${CMAKE_CURRENT_LIST_DIR}/test_base64.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test_generate_token.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test_matrixarea.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test_rsa.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test_sha1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test_xtea.cpp
    )

foreach(test_src ${tests_SRC})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE tfslib Boost::unit_test_framework)

    if (ENABLE_IPO)
	    set_target_properties(${test_name} PROPERTIES INTERPROCEDURAL_OPTIMIZATION True)
    endif()

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
