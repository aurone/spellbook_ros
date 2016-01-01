cmake_minimum_required(VERSION 2.8)
project(spellbook_ros)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/spellbook/cmake/Modules")
message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

include(EnableCXX11)
enable_cxx11()

find_package(Boost REQUIRED system thread)

include(LocateEigen)
locate_eigen()

find_package(
    catkin REQUIRED
    COMPONENTS
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

find_package(PkgConfig REQUIRED)
pkg_check_modules(openni REQUIRED libopenni)

find_package(PCL REQUIRED)

catkin_package(
    INCLUDE_DIRS
        include
        extern/spellbook/spellbook/src/include
    LIBRARIES
        msg_utils
        costmap_extruder
    CATKIN_DEPENDS
        sbpl_geometry_utils
    DEPENDS
        Boost
        Eigen)

set(BUILD_SHARED_LIBS On)

add_subdirectory(extern/spellbook)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Eigen_INCLUDE_DIRS})
include_directories(${PCL_INCLUDE_DIRS})
include_directories(${catkin_INCLUDE_DIRS})
include_directories(${openni_INCLUDE_DIRS})
include_directories(extern/spellbook/spellbook/src/include)
include_directories(include)

link_directories(${openni_LIBRARY_DIRS})

add_subdirectory(src)

# NOTE: opting out of CATKIN_PACKAGE_INCLUDE_DESTINATION here, which will rename
# the base include directory to the project name, "spellbook_ros", rather than
# "spellbook"
install(DIRECTORY include/spellbook DESTINATION include)
