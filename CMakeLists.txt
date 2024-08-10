cmake_minimum_required(VERSION 3.8)

enable_testing()
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(EXTERN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern)
set(SRC_EXT c)

project(pubq LANGUAGES C VERSION 0.1.0)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILE_FEATURES -g -Wall -Werror -Wextra -O3)

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.${SRC_EXT} ${EXTERN_DIR}/*.${SRC_EXT})
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR} PUBLIC ${EXTERN_DIR})
if (UNIX)
	target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
endif (UNIX)

# ===== Tests =====

file(GLOB_RECURSE TEST_SOURCES ${TEST_DIR}/*.test.${SRC_EXT})

foreach(TEST_FILE ${TEST_SOURCES})
get_filename_component(TEST_NAME ${TEST_FILE} NAME_WLE)
add_executable(${TEST_NAME} ${TEST_FILE} ${SOURCES})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
target_include_directories(${TEST_NAME} PUBLIC ${INC_DIR} PUBLIC ${EXTERN_DIR})
endforeach()