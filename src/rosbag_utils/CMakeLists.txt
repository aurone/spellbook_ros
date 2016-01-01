add_executable(restamp restamp.cpp)
target_link_libraries(restamp ${catkin_LIBRARIES})

add_executable(rosbag_freeze rosbag_freeze.cpp)
target_link_libraries(rosbag_freeze ${catkin_LIBRARIES})

install(
    TARGETS restamp rosbag_freeze
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
