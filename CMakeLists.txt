cmake_minimum_required(VERSION 3.1.0)

if(POLICY CMP0074)
    # find_package() uses <PackageName>_ROOT variables.
    # This policy was introduced in CMake version 3.12.
    cmake_policy(SET CMP0074 NEW)
endif()

project(TimeWheel)

# C++ standard requirements.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CMake 3.1.0+ required.
# See: https://stackoverflow.com/a/29871891
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB SRCS *.cpp *.h)

set(EXE_TARGET_NAME time-wheel)

set(LIBS
    "${CMAKE_THREAD_LIBS_INIT}"
    )

add_executable(${EXE_TARGET_NAME} ${SRCS})
target_link_libraries(${EXE_TARGET_NAME} ${LIBS})
