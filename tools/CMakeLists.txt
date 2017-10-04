set(raid_xmltolua_SRC
	${CMAKE_CURRENT_LIST_DIR}/raid_xmltolua.cpp
	)

add_executable(raid-xmltolua ${raid_xmltolua_SRC})
target_link_libraries(raid-xmltolua ${PUGIXML_LIBRARIES})
