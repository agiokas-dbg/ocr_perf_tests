cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(ocrbindings
    VERSION 0.1.0
    DESCRIPTION "Tesseract OCR bindings"
    LANGUAGES CXX
)

add_subdirectory(src)
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(TESSERACT REQUIRED tesseract)
pkg_search_module(LEPTONICA REQUIRED lept)

include_directories(${TESSERACT_INCLUDE_DIRS})
include_directories(${LEPTONICA_INCLUDE_DIRS})
set(Boost_USE_STATIC_LIBS OFF)
set(BUILD_SHARED_LIBS ON)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${TESSERACT_LIBRARY_DIRS})
link_directories(${LEPTONICA_LIBRARY_DIRS})

include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -funroll-loops")
add_executable(demo src/main.cpp)
target_link_libraries(demo tesseract
                           ${LEPTONICA_LIBRARIES}
                           ${OpenCV_LIBS}
                           ${CMAKE_THREAD_LIBS_INIT}
                           ${Boost_LIBRARIES})
