cmake_minimum_required(VERSION 3.16)

project(p2pcom VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Core)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(openp2p main.cpp)

qt_add_qml_module(openp2p
  URI p2pcom
  QML_FILES gui/qml/Main.qml
  QML_FILES gui/qml/MessageHandler.qml
  QML_FILES gui/qml/PeerConnection.qml
  SOURCES gui/addrlabel.h gui/addrlabel.cpp
  SOURCES gui/peerlistmodel.h gui/peerlistmodel.cpp
  SOURCES gui/chatlistmodel.h gui/chatlistmodel.cpp
  SOURCES ipv6addrresolver.h ipv6addrresolver.cpp
  SOURCES server.h server.cpp
  SOURCES peer.h peer.cpp
  SOURCES peerid.h
  RESOURCES todo.org
  SOURCES sessionmanager.h sessionmanager.cpp
  SOURCES typedefs.h
  SOURCES self.h self.cpp
  SOURCES message.h message.cpp
  SOURCES crypto.h crypto.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(openp2p PRIVATE QT_QML_DEBUG)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(openp2p PROPERTIES
  #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.openp2p
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)

target_link_libraries(openp2p PRIVATE
  Qt6::Quick
  Qt6::Core
  Qt6::QuickControls2
  OpenSSL::SSL
  OpenSSL::Crypto
)

include(GNUInstallDirs)
install(TARGETS openp2p
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
  TARGET openp2p
  OUTPUT_SCRIPT deploy_script
  NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
