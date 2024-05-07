set(http_SRC
	${CMAKE_CURRENT_LIST_DIR}/cacheinfo.cpp
	${CMAKE_CURRENT_LIST_DIR}/error.cpp
	${CMAKE_CURRENT_LIST_DIR}/fail.cpp
	${CMAKE_CURRENT_LIST_DIR}/http.cpp
	${CMAKE_CURRENT_LIST_DIR}/listener.cpp
	${CMAKE_CURRENT_LIST_DIR}/login.cpp
	${CMAKE_CURRENT_LIST_DIR}/router.cpp
	${CMAKE_CURRENT_LIST_DIR}/session.cpp
	)

set(http_HDR
	${CMAKE_CURRENT_LIST_DIR}/cacheinfo.h
	${CMAKE_CURRENT_LIST_DIR}/error.h
	${CMAKE_CURRENT_LIST_DIR}/fail.h
	${CMAKE_CURRENT_LIST_DIR}/http.h
	${CMAKE_CURRENT_LIST_DIR}/listener.h
	${CMAKE_CURRENT_LIST_DIR}/login.h
	${CMAKE_CURRENT_LIST_DIR}/router.h
	${CMAKE_CURRENT_LIST_DIR}/session.h
	)

add_library(http OBJECT ${http_SRC})
target_link_libraries(http PRIVATE Boost::json)
set_target_properties(http PROPERTIES UNITY_BUILD ${ENABLE_UNITY_BUILD})

if (ENABLE_IPO)
	set_target_properties(http PROPERTIES INTERPROCEDURAL_OPTIMIZATION True)
endif()

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
