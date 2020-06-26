add_executable(raid-xmltolua ${CMAKE_CURRENT_LIST_DIR}/raid_xmltolua.cpp)
target_link_libraries(raid-xmltolua Boost::filesystem Boost::program_options ${PUGIXML_LIBRARIES})
