cmake_minimum_required(VERSION 3.21)

project(gitlog)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(SRC_FILES
	${SRC_ROOT_DIR}/plugins/request_git_log.cpp
)

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} Boost::filesystem)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)