cmake_minimum_required(VERSION 3.16)

# Set the project name
project(VideoPlayer)

# Find the Qt6 package
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(VideoPlayer src/main.cpp src/playerWindow.cpp)

# Link the Qt6 Widgets and Multimedia libraries
target_link_libraries(VideoPlayer Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets)
