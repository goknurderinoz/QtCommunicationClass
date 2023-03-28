cmake_minimum_required(VERSION 3.10)

# Set the project name
project(QtCommunicationClasses)

# Find Qt5 and Qt6 packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# Add the executable
add_executable(QtCommunicationClasses 
	main.cpp
	mainwindow.cpp
	config.cpp)

# Link to the Qt Core library
target_link_libraries(QtCommunicationClasses Qt${QT_VERSION_MAJOR}::Core)
