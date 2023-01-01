cmake_minimum_required(VERSION 3.10)

project(GXcar)

FILE(GLOB_RECURSE CURRENT_SOURCES src/*.cpp) 

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
find_package(Threads)
find_package(OpenCV REQUIRED)

add_executable(GXcar ${CURRENT_SOURCES})

target_include_directories(GXcar
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(GXcar ${OpenCV_LIBS})
target_link_libraries(GXcar ${WIRINGPI_LIBRARIES}) 
target_link_libraries(GXcar pthread) 
