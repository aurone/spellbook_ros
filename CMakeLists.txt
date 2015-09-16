cmake_minimum_required(VERSION 2.8)
project(spellbook_ros)

find_package(Boost REQUIRED system thread)

find_package(Eigen3)
if (${EIGEN3_FOUND})
    set(EIGEN_INCLUDES ${EIGEN3_INCLUDE_DIR})
else()
    find_package(Eigen REQUIRED)
    set(EIGEN_INCLUDES ${Eigen_INCLUDE_DIRS})
endif()

find_package(PkgConfig REQUIRED)

find_package(catkin REQUIRED COMPONENTS
    geometry_msgs
    moveit_msgs
    nav_msgs
    octomap_msgs
    octomap_ros
    roscpp
    rosbag
    sbpl_geometry_utils
    sensor_msgs
    std_msgs
    tf
    trajectory_msgs
    visualization_msgs)

pkg_check_modules(openni REQUIRED libopenni)

catkin_package(
    INCLUDE_DIRS
        include
    LIBRARIES
        msg_utils
    CATKIN_DEPENDS
        sbpl_geometry_utils
    DEPENDS
        Boost
        Eigen)

set(BUILD_SHARED_LIBS On)

# inspect for c++11 flags
if (CMAKE_COMPILER_IS_GNUCXX)
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GNUCXX_VERSION)
    if (GNUCXX_VERSION VERSION_LESS 4.7)
        set(C11_FLAGS "-std=c++0x")
    else()
        set(C11_FLAGS "-std=c++11")
    endif()
else()
    message("Compiler is not g++. You're on your own for C++11 features")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C11_FLAGS}")

set(CMAKE_BUILD_TYPE Release)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${catkin_INCLUDE_DIRS})
include_directories(${EIGEN_INCLUDES})
include_directories(${openni_INCLUDE_DIRS})
include_directories(include)

link_directories(${openni_LIBRARY_DIRS})

add_subdirectory(src)

