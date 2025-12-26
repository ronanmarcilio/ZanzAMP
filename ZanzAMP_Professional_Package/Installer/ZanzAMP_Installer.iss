; Script do Inno Setup para ZanzAMP VST3
#define MyAppName "ZanzAMP"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "ZanzAudio"
#define MyAppURL "https://www.zanzaudio.com"
#define MyPluginFile "ZanzAMP.vst3"

[Setup]
AppId={{ZANZAMP-VST3-INSTALLER-ID}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppProvider={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
DefaultDirName={commoncf64}\VST3\ZanzAudio
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir=..\build\Installer
OutputBaseFilename=Setup_ZanzAMP
Compression=lzma
SolidCompression=yes
ArchitectureAllowed=x64
ArchitecturesInstallIn64BitMode=x64
; Assinatura digital (deixar pronto para o usuário)
; SignTool=mysigntool $f

[Languages]
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"

[Files]
; O arquivo .vst3 deve estar na pasta build após a compilação
Source: "..\build\ZanzAMP_artefacts\Release\VST3\{#MyPluginFile}"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Messages]
brazilianportuguese.WelcomeLabel1=Bem-vindo ao Instalador do {#MyAppName}
brazilianportuguese.WelcomeLabel2=Este programa instalará o plugin VST3 {#MyAppName} no seu computador.

[Code]
function InitializeSetup(): Boolean;
begin
  Result := True;
end;
