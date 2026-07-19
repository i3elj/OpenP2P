@echo off
setlocal EnableExtensions

rem ---- paths / settings ----
set "pwd=%cd%"
set "srcdir=%pwd%\..\src"
set "qmldir=%srcdir%\gui\qml"
set "windowsdir=%pwd%\windows"
set "appname=openp2p_app"

echo Running windeployqt...
pushd "%windowsdir%"
windeployqt6 "%windowsdir%\%appname%.exe" --qmldir="%qmldir%" --release --openssl-root="C:\msys64\ucrt64"
popd