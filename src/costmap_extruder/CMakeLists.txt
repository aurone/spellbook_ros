add_library(costmap_extruder CostmapExtruder.cpp)
target_link_libraries(costmap_extruder ${catkin_LIBRARIES} ${PCL_LIBRARIES})

add_executable(costmap_extruder_node costmap_extruder.cpp)
target_link_libraries(
    costmap_extruder_node
    costmap_extruder
    msg_utils
    ${catkin_LIBRARIES})

install(
    TARGETS costmap_extruder costmap_extruder_node
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
