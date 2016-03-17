cmake_minimum_required(VERSION 2.8.3)
project(nostop_gazebo_sensor)

ENABLE_LANGUAGE(CXX)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(catkin REQUIRED COMPONENTS
  gazebo_ros
  roscpp
  base_math
  tf
)

# Depend on system install of Gazebo
find_package(gazebo REQUIRED)

catkin_package(
  INCLUDE_DIRS include/${PROJECT_NAME}/
)

include_directories(include/${PROJECT_NAME}/ ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS} ${SDFormat_INCLUDE_DIRS})

catkin_package(
    DEPENDS
      gazebo_ros
      roscpp
      base_math
      tf
    CATKIN_DEPENDS
    INCLUDE_DIRS
    LIBRARIES
)

set(GazeboSensor_SOURCES
	src/gazebo_localizer.cpp
	src/StaticTransformPublisher.cpp
)

add_executable(${PROJECT_NAME}_node ${GazeboSensor_SOURCES})
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES} base_math lemon)
