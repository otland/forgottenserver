add_executable(raid-xmltolua ${CMAKE_CURRENT_LIST_DIR}/raid_xmltolua.cpp)
target_link_libraries(raid-xmltolua Boost::program_options ${PUGIXML_LIBRARIES})

set_target_properties(raid-xmltolua PROPERTIES CXX_STANDARD 17)
set_target_properties(raid-xmltolua PROPERTIES CXX_STANDARD_REQUIRED ON)
