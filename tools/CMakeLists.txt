set(raid_xmltolua_SRC
	${CMAKE_CURRENT_LIST_DIR}/raid_xmltolua.cpp
	)

find_package(Boost 1.53.0 COMPONENTS filesystem program_options REQUIRED)

add_executable(raid-xmltolua ${raid_xmltolua_SRC})
target_link_libraries(raid-xmltolua ${Boost_LIBRARIES} ${PUGIXML_LIBRARIES})
