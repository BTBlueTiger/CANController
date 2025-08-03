cmake_minimum_required(VERSION 3.16)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
project(CANController VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt modules
find_package(Qt6 REQUIRED COMPONENTS Quick Bluetooth)

qt_standard_project_setup(REQUIRES 6.8)

# Add the executable
qt_add_executable(appCANController
    main.cpp
)

# Define a prefix for include directories
set(INCLUDE_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(QML_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/qml)

file(GLOB SOURCE_FILES
    ${INCLUDE_PREFIX}/*.h
    ${SRC_PREFIX}/*.cpp
)


# Add QML module
qt_add_qml_module(appCANController
    URI CANController
    VERSION 1.0
    QML_FILES
        Main.qml
        qml/FindDevicesScreen.qml
        qml/ServoService.qml
        qml/DeviceControl.qml
    SOURCES
        ${SOURCE_FILES}
        SOURCES include/SDLController.h
        SOURCES src/SDLController.cpp

)

# Set properties for macOS/iOS
set_target_properties(appCANController PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Link required Qt libraries
target_link_libraries(appCANController
    PRIVATE Qt6::Quick Qt6::Bluetooth
)

set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL/")  # z. B. /usr/include/SDL2
include_directories(${SDL2_DIR}/include)
link_directories(${SDL2_DIR}/lib)
target_link_libraries(appCANController PRIVATE SDL2)


# Install rules
include(GNUInstallDirs)
install(TARGETS appCANController
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
