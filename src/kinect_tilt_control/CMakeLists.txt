add_executable(kinect_motor kinect_motor.cpp KinectMotor.cpp)
target_link_libraries(kinect_motor ${openni_LIBRARIES})

install(
    TARGETS kinect_motor
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
