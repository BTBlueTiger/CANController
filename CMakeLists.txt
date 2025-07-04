cmake_minimum_required(VERSION 3.16)

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
    SOURCES
        ${HEADERS_FILES} ${SOURCE_FILES}
        QML_FILES qml/FindDevicesScreen.qml
        SOURCES include/SimpleBTDevice.h
        QML_FILES qml/DeviceControl.qml


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

# Install rules
include(GNUInstallDirs)
install(TARGETS appCANController
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
