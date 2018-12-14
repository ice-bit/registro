#!/bin/bash
# Script to create an AppImage using linuxdeployqt
# First, be sure to have this program on your PATH
# download from here(https://github.com/probonopd/linuxdeployqt)
# Call this script from the build directory

# Check if script was launched with root permissions, in this case exit
if [[ $EUID == 0 ]]; then
    echo -e "Do not use root user!"
    exit 1
fi

# Delete any previous directory and other files
rm -rf usr *.desktop *.png

# Create a basic folder structure
mkdir -p usr/{bin,lib,share} usr/share/{applications,icons/hicolor/256x256/apps}

# Copy Registro icon
cp -R ../resources/images/window_icon.png usr/share/icons/hicolor/256x256/apps/registro.png

# Copy the executable
cp -R registro usr/bin

# Create a basic desktop file
cat > usr/share/applications/registro.desktop << EOF
[Desktop Entry]
Type=Application
Name=Registro
Comment=Registro is a multi-platform application to save and organize your school marks.
Exec=registro
Icon=registro
Terminal=false
Categories=Utility;
EOF

# Now create the AppImage using linuxdeployqt
# In this script i assume that linuxdeployqt is located in PATH and it is called 'linuxdeployqt'
linuxdeployqt usr/share/applications/registro.desktop -appimage

echo -e "AppImage created!"