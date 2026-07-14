DIR := $(PWD)/linux
APPNAME := openp2p_app
SRC := $(PWD)/../src
QMLDIR := $(SRC)/gui/qml
GENFILES := $(DIR)/lib $(DIR)/plugins $(DIR)/qml $(DIR)/translations $(DIR)/AppRun $(DIR)/$(APPNAME)

install-linux:
	rm -rf $(GENFILES)
	cp $(PWD)/../build/Desktop_Release/$(APPNAME) $(DIR)/$(APPNAME)
	linuxdeployqt $(DIR)/$(APPNAME) \
		-appimage \
		-qmldir="$(QMLDIR)" \
		-bundle-non-qt-libs \
		-unsupported-allow-new-glibc