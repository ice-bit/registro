Outfile "registro_installer.exe"

InstallDir "C:\Program Files\registro"

Section

SetOutPath "C:\Program Files\Registro"

file /r "Release\*"
CreateShortcut "$DESKTOP\Registro 1.3.lnk" "$INSTDIR\registro.exe"

WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

Section "Uninstall"

Delete "$INSTDIR\uninstall.exe"

RMDIR /r "$INSTDIR"

Delete "$DESKTOP\Registro 1.3.lnk"

SectionEnd
