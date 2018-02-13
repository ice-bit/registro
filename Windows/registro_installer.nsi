Outfile "registro_installer_v1.2_release_windows.exe"

InstallDir "C:\Program Files\Registro"

Section

SetOutPath "C:\Program Files\Registro"

file /r "registro_final_deploy\*"
CreateShortcut "$DESKTOP\Registro 1.2.lnk" "$INSTDIR\registro.exe"

CreateDirectory "$APPDATA\Registro"

WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

Section "Uninstall"

Delete "$INSTDIR\uninstall.exe"

RMDIR /r "$INSTDIR"

RMDIR /r "$APPDATA\Registro"

Delete "$DESKTOP\Registro 1.2.lnk"

SectionEnd