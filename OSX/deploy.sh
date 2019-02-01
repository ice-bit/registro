#!/bin/bash
# Script used to create an OS X app image
# Be sure to install Apple Developer Tools
# and Qt and to add them to your system PATH
# Call this script from the build directory

# Define App name
APPNAME="registro"
QTDEPLOY="/Users/marco/Qt/5.12.0/clang_64/bin/macdeployqt"

# Check if script was launched as root user, in this case exit
if [[ $EUID == 0 ]]; then
	echo -e "Do not use root user!"
	exit 1
fi

# Remote any old App image called 'registro.app'
rm -rf ${APPNAME}.app

# Create an empty 'App' directory
mkdir -p ${APPNAME}.app/Contents/MacOS

# Then copy the executable
cp -R registro ${APPNAME}.app/Contents/MacOS/${APPNAME}
chmod +x ${APPNAME}.app/Contents/MacOS/${APPNAME}

# Copy the Qt libraries inside the App Image
${QTDEPLOY} registro.app

echo -e "App Created!"
