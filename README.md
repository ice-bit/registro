# Registro [![Build Status](https://travis-ci.org/ice-bit/registro.svg?branch=master)](https://travis-ci.org/ice-bit/registro)
Registro is a cross-platform application to save and organize your school marks.  
<img src="https://img.shields.io/badge/os-linux-yellow.svg">
<img src="https://img.shields.io/badge/os-windows-blue.svg">

# Installation
There are two ways to obtain Registro in your computer:
1. By compiling it from the source;
2. By downloading the executable file/installer;

## Compiling it from source
_We assume that you have `CMake`, `Qt`, `git` and a `C++` compiler installed on your computer._

Clone the git repository: `git clone https://github.com/ice-bit/registro.git`
Inside the directory run: `mkdir build && cd build`
And then: `cmake .. && make`
If you are a GNU/Linux user, you can create an [appimage](https://appimage.org/) by launching the `deploy.sh` script from the build directory.  
**NOTE**: this will overwrite any directory called `usr` from `build`. Before launching it, you have to download a copy of [linuxdeployqt](https://github.com/probonopd/linuxdeployqt) and put it into your *PATH* with the name of `linuxdeployqt`.

After that, the _compiler_ will create an executable file called **registro**, you can move it anywhere.

## Downloading the executable file
The other way is to download the executable file and open/install it:  
[Windows x64 exe](https://github.com/ice-bit/registro/raw/master/Windows/registro_installer(x64).exe)  
[Windows x86 exe](https://github.com/ice-bit/registro/raw/master/Windows/registro_installer(x86).exe)  
[GNU/Linux x86_x64](https://github.com/ice-bit/registro/raw/master/Linux/Registro-x86_64.AppImage)  

 Windows System Requirements: `Visual C++ Redistributable per Visual Studio 2015`  
 GNU/Linux System Requirements: `GLIB >= 2.27`

**Note**: OSX executable file isn't ready yet...but it will be delivered soon.

# Usage
To start Registro just _double-click_ on the Application icon or start it from a command line interface by typing: `./registro`.

# License
Registro is released under GPLv3.0, you will obtain a copy of this license by downloading the repository or by visiting [this](https://opensource.org/licenses/GPL-3.0) page.

# Screenshot
![GIF](resources/registro.gif)
