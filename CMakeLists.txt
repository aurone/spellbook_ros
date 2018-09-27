cmake_minimum_required(VERSION 2.8)
project(spellbook)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Boost REQUIRED system thread unit_test_framework)

find_package(
    catkin REQUIRED
    COMPONENTS
        geometry_msgs
        moveit_msgs
        nav_msgs
        octomap_msgs
        octomap_ros
        pcl_ros
        roscpp
        rosbag
        sensor_msgs
        smpl
        std_msgs
        tf
        trajectory_msgs
        visualization_msgs)

find_package(octomap REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(openni REQUIRED libopenni)

find_package(PCL REQUIRED)

catkin_package(
    INCLUDE_DIRS
        include
        ${EIGEN3_INCLUDE_DIR}
    LIBRARIES
        msg_utils
        costmap_extruder
        spellbook
    CATKIN_DEPENDS
        smpl
    DEPENDS
        Boost
        OCTOMAP)

#####################
# spellbook library #
#####################

add_library(
    spellbook
    src/dubins/dubins.cpp
    src/log/logging.cpp
    src/mapgen/DFSMazeGenerator.cpp
    src/mapgen/RandomMapGenerator.cpp
    src/memory/StackAllocator.cpp
    src/memory/mempool.cpp)

target_compile_options(spellbook PUBLIC -std=c++11)

target_include_directories(spellbook PUBLIC include)

target_include_directories(spellbook SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(spellbook SYSTEM PUBLIC ${smpl_INCLUDE_DIRS})

target_link_libraries(spellbook PUBLIC ${smpl_LIBRARIES})

############################
# costmap_extruder library #
############################

add_library(costmap_extruder src/costmap_extruder/CostmapExtruder.cpp)

target_compile_options(costmap_extruder PUBLIC -std=c++11)

target_include_directories(costmap_extruder PRIVATE include)

target_include_directories(costmap_extruder SYSTEM PUBLIC ${moveit_msgs_INCLUDE_DIRS})
target_include_directories(costmap_extruder SYSTEM PUBLIC ${nav_msgs_INCLUDE_DIRS})
target_include_directories(costmap_extruder SYSTEM PUBLIC ${OCTOMAP_INCLUDE_DIRS})
target_include_directories(costmap_extruder SYSTEM PUBLIC ${octomap_msgs_INCLUDE_DIRS})
target_include_directories(costmap_extruder SYSTEM PUBLIC ${pcl_ros_INCLUDE_DIRS})
target_include_directories(costmap_extruder SYSTEM PUBLIC ${PCL_INCLUDE_DIRS})

target_include_directories(costmap_extruder SYSTEM PRIVATE ${octomap_ros_INCLUDE_DIRS})
target_include_directories(costmap_extruder SYSTEM PRIVATE ${pcl_conversions_INCLUDE_DIRS})

target_link_libraries(costmap_extruder PRIVATE msg_utils spellbook)

target_link_libraries(costmap_extruder PRIVATE ${OCTOMAP_LIBRARIES})
target_link_libraries(costmap_extruder PRIVATE ${octomap_ros_LIBRARIES})

##############################
# costmap_extruder_node node #
##############################

add_executable(costmap_extruder_node src/costmap_extruder/costmap_extruder.cpp)

target_compile_options(costmap_extruder_node PRIVATE -std=c++11)

target_include_directories(costmap_extruder_node PRIVATE include)

target_include_directories(costmap_extruder_node SYSTEM PRIVATE ${nav_msgs_INCLUDE_DIRS})
target_include_directories(costmap_extruder_node SYSTEM PRIVATE ${octomap_msgs_INCLUDE_DIRS})
target_include_directories(costmap_extruder_node SYSTEM PRIVATE ${roscpp_INCLUDE_DIRS})

target_link_libraries(costmap_extruder_node PRIVATE costmap_extruder)
target_link_libraries(costmap_extruder_node PRIVATE ${roscpp_LIBRARIES})

#####################
# kinect_motor node #
#####################

add_executable(
    kinect_motor
    src/kinect_tilt_control/kinect_motor.cpp
    src/kinect_tilt_control/KinectMotor.cpp)

target_compile_options(kinect_motor PRIVATE -std=c++11)

target_include_directories(kinect_motor SYSTEM PRIVATE ${openni_INCLUDE_DIRS})

target_link_libraries(kinect_motor PRIVATE ${openni_LIBRARIES})

#####################
# msg_utils library #
#####################

add_library(msg_utils src/msg_utils/msg_utils.cpp)

target_compile_options(msg_utils PUBLIC -std=c++11)

target_include_directories(msg_utils PRIVATE include)

target_include_directories(msg_utils SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR})
target_include_directories(msg_utils SYSTEM PUBLIC ${geometry_msgs_INCLUDE_DIRS})
target_include_directories(msg_utils SYSTEM PUBLIC ${roscpp_INCLUDE_DIRS})
target_include_directories(msg_utils SYSTEM PUBLIC ${sensor_msgs_INCLUDE_DIRS})
target_include_directories(msg_utils SYSTEM PUBLIC ${tf_INCLUDE_DIRS})
target_include_directories(msg_utils SYSTEM PUBLIC ${std_msgs_INCLUDE_DIRS})
target_include_directories(msg_utils SYSTEM PUBLIC ${trajectory_msgs_INCLUDE_DIRS})
target_include_directories(msg_utils SYSTEM PUBLIC ${visualization_msgs_INCLUDE_DIRS})

target_include_directories(msg_utils SYSTEM PRIVATE ${smpl_INCLUDE_DIRS})

target_link_libraries(msg_utils PUBLIC ${roscpp_LIBRARIES})
target_link_libraries(msg_utils PUBLIC ${tf_LIBRARIES})

target_link_libraries(msg_utils PRIVATE ${smpl_LIBRARIES})

###################
# restamp program #
###################

add_executable(restamp src/rosbag_utils/restamp.cpp)
target_compile_options(restamp PRIVATE -std=c++11)
target_include_directories(restamp SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(restamp SYSTEM PRIVATE ${geometry_msgs_INCLUDE_DIRS})
target_include_directories(restamp SYSTEM PRIVATE ${roscpp_INCLUDE_DIRS})
target_include_directories(restamp SYSTEM PRIVATE ${rosbag_INCLUDE_DIRS})
target_include_directories(restamp SYSTEM PRIVATE ${tf_INCLUDE_DIRS})
target_include_directories(restamp SYSTEM PRIVATE ${nav_msgs_INCLUDE_DIRS})
target_link_libraries(restamp PRIVATE ${Boost_LIBRARIES})
target_link_libraries(restamp PRIVATE ${roscpp_LIBRARIES})
target_link_libraries(restamp PRIVATE ${rosbag_LIBRARIES})
target_link_libraries(restamp PRIVATE ${tf_LIBRARIES})

#########################
# rosbag_freeze program #
#########################

add_executable(rosbag_freeze src/rosbag_utils/rosbag_freeze.cpp)
target_compile_options(rosbag_freeze PRIVATE -std=c++11)
target_include_directories(rosbag_freeze SYSTEM PRIVATE ${roscpp_INCLUDE_DIRS})
target_include_directories(rosbag_freeze SYSTEM PRIVATE ${rosbag_INCLUDE_DIRS})
target_link_libraries(rosbag_freeze PRIVATE ${roscpp_LIBRARIES})
target_link_libraries(rosbag_freeze PRIVATE ${rosbag_LIBRARIES})

install(DIRECTORY include/spellbook DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

install(
    TARGETS costmap_extruder costmap_extruder_node spellbook kinect_motor msg_utils restamp rosbag_freeze
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
