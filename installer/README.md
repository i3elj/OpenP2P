# Installing Instructions
## Installing on Linux
This project relies on an unofficial port of a deployqt script for linux, called [linuxdeployqt](https://github.com/probonopd/linuxdeployqt), which is distributed as an AppImage, therefore every linux distribution can run it. Remember to have a proper working installation of qt development libraries and openssl[^1].

The following command should be run on the installer folder, where `$PWD` returns `$HOME/path/to/OpenP2P/installer/`.
### Installing on Linux
Just run `install.sh` on this folder:
```bash
cd OpenP2P/installer/ # change to installer folder
./install.sh          # run installer
```
### Installing on Windows
Install [InnoSetup](https://jrsoftware.org/isinfo.php), copy the executable on the release build to ./installer/windows, run the deploy.bat file and double-click on `install-script.iss`.

[^1]: On my first build I remember needing only qt6-base, qt6-declarative, qt6-tools and qt6-svg packages on Artix.