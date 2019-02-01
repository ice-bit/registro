Outfile "registro_installer.exe"
InstallDir "$PROGRAMFILES64\Registro"

!define APPNAME "Registro"
!define INFOMSG "Your databases won't be touched"
	
Section "Install"

SetOutPath "$PROGRAMFILES64\Registro"

file /r "Release\*"
CreateShortcut "$DESKTOP\Registro 1.3.1.lnk" "$INSTDIR\registro.exe"

WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

# Confirm uninstallation of the program
function un.onInit

MessageBox MB_OKCANCEL "Uninstall ${APPNAME}? ${INFOMSG}" IDOK next
	Abort
next:
	
functionEnd


Section "Uninstall"

Delete "$INSTDIR\uninstall.exe"

RMDIR /r "$INSTDIR"

Delete "$DESKTOP\Registro 1.3.1.lnk"

SectionEnd
