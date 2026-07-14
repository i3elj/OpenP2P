cmake_minimum_required(VERSION 3.16)

project(openp2p VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)
find_package(OpenSSL REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(openp2p_app src/main.cpp)

qt_add_qml_module(openp2p_app
	URI openp2p
	VERSION 1.0
	QML_FILES
		src/gui/qml/Main.qml
		src/gui/qml/MessageHandler.qml
		src/gui/qml/PeerConnection.qml
	SOURCES
		src/gui/addrlabel.hpp
		src/gui/addrlabel.cpp
		src/gui/peerlistmodel.hpp
		src/gui/peerlistmodel.cpp
		src/gui/chatlistmodel.hpp
		src/gui/chatlistmodel.cpp
		src/ipv6addrresolver.hpp
		src/ipv6addrresolver.cpp
		src/server.hpp
		src/server.cpp
		src/peer.hpp
		src/peer.cpp
		src/sessionmanager.hpp
		src/sessionmanager.cpp
		src/typedefs.hpp
		src/self.hpp
		src/self.cpp
		src/message.hpp
		src/message.cpp
		src/crypto.hpp
		src/crypto.cpp
)

target_link_libraries(openp2p_app PRIVATE
	Qt6::Quick
	Qt6::QuickControls2
	OpenSSL::SSL
	OpenSSL::Crypto
)

set_target_properties(openp2p_app PROPERTIES
	# MACOSX_BUNDLE_GUI_IDENTIFIER io.i3elj.openp2p
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(openp2p_app PRIVATE QT_QML_DEBUG)
endif()

include(GNUInstallDirs)

install(TARGETS openp2p_app
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
	TARGET openp2p_app
	OUTPUT_SCRIPT deploy_script
)

install(SCRIPT ${deploy_script})
