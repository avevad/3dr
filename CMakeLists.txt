cmake_minimum_required(VERSION 3.30)
project(3dr)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)
find_package(nlohmann_json 3.11.0 REQUIRED)
pkg_check_modules(EVDEV libevdev REQUIRED)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(-DTDR_DEBUG_BUILD=1)
else ()
    add_compile_definitions(-DTDR_DEBUG_BUILD=0)
endif ()

# Static library
add_library(3dr-shared SHARED
        src/ppm.cpp
        src/scene.cpp
        src/util.cpp
        src/camera.cpp
        src/renderer.cpp
        src/geom.cpp
        src/argparse.cpp
)
target_include_directories(3dr-shared PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(3dr-shared PUBLIC nlohmann_json::nlohmann_json)
set_target_properties(3dr-shared PROPERTIES OUTPUT_NAME 3dr)

# Main executable
add_executable(3dr src/main.cpp)
target_include_directories(3dr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(3dr PUBLIC 3dr-shared)

# Camera movement capture executable
add_executable(3dr-ccapture src/main_ccapture.cpp)
target_include_directories(3dr-ccapture PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(3dr-ccapture PUBLIC ${EVDEV_INCLUDE_DIRS})
target_link_libraries(3dr-ccapture PUBLIC 3dr-shared)
target_link_libraries(3dr-ccapture PUBLIC ${EVDEV_LIBRARIES})

install(TARGETS 3dr-shared 3dr 3dr-ccapture)
