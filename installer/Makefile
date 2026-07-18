APPNAME := openp2p_app
SRC := $(PWD)/../src
QMLDIR := $(SRC)/gui/qml

install-linux:
        rm -rf $(PWD)/linux/lib $(PWD)/linux/plugins $(PWD)/linux/qml $(PWD)/linux/translations $(PWD)/linux/AppRun $(PWD)/linux/$(APPNAME)
	cp $(PWD)/../build/Desktop_Release/$(APPNAME) $(PWD)/linux/$(APPNAME)
	linuxdeployqt $(PWD)/linux/$(APPNAME) \
		-appimage \
		-qmldir="$(QMLDIR)" \
		-bundle-non-qt-libs \
		-unsupported-allow-new-glibc