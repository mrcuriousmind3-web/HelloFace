#define MyAppName "HelloFace Preview"
#define MyAppVersion "0.1.0"
#define MyAppPublisher "HelloFace Open Source"
#define MyAppExeName "HelloFace.exe"

[Setup]
AppId={{8E4D55C2-1AB8-4E70-92E4-2A5B0F7A90A1}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\HelloFace Preview
DefaultGroupName=HelloFace Preview
OutputDir=dist
OutputBaseFilename=HelloFace-Setup
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64
PrivilegesRequired=lowest
UninstallDisplayIcon={app}\{#MyAppExeName}
SetupIconFile=assets\helloface.ico

[Files]
Source: "build\Release\HelloFace.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "assets\helloface-mark.svg"; DestDir: "{app}\assets"; Flags: ignoreversion

[Icons]
Name: "{group}\HelloFace Preview"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\HelloFace Preview"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional icons:"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Launch HelloFace Preview"; Flags: postinstall nowait skipifsilent
