# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_library(position SHARED
             src/main/cpp/accelerometer.cpp
             src/main/cpp/math.cpp
             src/main/cpp/position.cpp)

add_library(orientation SHARED
            src/main/cpp/gyroscope.cpp
            src/main/cpp/math.cpp
            src/main/cpp/orientation.cpp)

add_library(renderer SHARED
            src/main/cpp/renderer.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       position
                       orientation
                       renderer
                       android
                       GLESv2

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )
