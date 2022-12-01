cmake_minimum_required(VERSION 3.21)

project(CppHttpServer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(SRC_FILES
	${SRC_ROOT_DIR}/main.cpp
	${SRC_ROOT_DIR}/http_server/async_http_server.cpp
	${SRC_ROOT_DIR}/http_server/async_connection.cpp
	${SRC_ROOT_DIR}/plugin_base/plugin_mgr.cpp
)

find_package(Boost COMPONENTS system program_options filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} Boost::system Boost::program_options Boost::filesystem)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)