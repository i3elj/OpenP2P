cmake_minimum_required(VERSION 3.16)

project(p2pcom VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appp2pcom
    main.cpp
)

qt_add_qml_module(appp2pcom
    URI p2pcom
    QML_FILES
        Main.qml
        SOURCES gui/addrlabel.h gui/addrlabel.cpp
        SOURCES ipv6addrresolver.h ipv6addrresolver.cpp
        SOURCES
        SOURCES server.h server.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appp2pcom PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appp2pcom
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appp2pcom
    PRIVATE Qt6::Quick
)
target_link_libraries(appp2pcom PRIVATE Qt6::Core)
target_link_libraries(appp2pcom PRIVATE Qt6::Core)
target_link_libraries(appp2pcom PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appp2pcom
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
