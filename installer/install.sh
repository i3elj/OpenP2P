#!/usr/bin/env bash
currdir="$(pwd)"
appname="openp2p_app"
src="$currdir/../src"
qmldir="$src/gui/qml"

install-linux() {
  rm -rf \
    "$currdir/linux/lib" \
    "$currdir/linux/plugins" \
    "$currdir/linux/qml" \
    "$currdir/linux/translations" \
    "$currdir/linux/AppRun" \
    "$currdir/linux/$appname"

  cp "$currdir/../build/Desktop_Release/$appname" "$currdir/linux/$appname"

  linuxdeployqt "$currdir/linux/$appname" \
    -appimage \
    -qmldir="$qmldir" \
    -bundle-non-qt-libs \
    -unsupported-allow-new-glibc
}

install-linux
