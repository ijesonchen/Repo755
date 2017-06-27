wxVcprojAuiEditor.txt is readme in Chinese.

****main purpose:
edit SOME VS2008 VC project properties easily.
auto add new dll configurations for project.

****how to:
compile wxVcprojAuiEditor with visual studio 2008 and run it.
or just run compiled\xxx\wxVcprojAuiEditor_xxx.exe

if compile error encountered, see ****environment section below.

****when to use
Q1. created a new vs2008 c++ proj, want change settings of
	OutputDirectory
	IntermediateDirectory
	OutputFile
A1. run bin\run\wxVcprojAuiEditor_Release_Win32.exe, load vcproj file,
	click auto, (check MainSetting) and click save. 
	if want customize setting, change relevant [MAIN/ITEMn] in ini file.
	
Q2. create a new vs2008 wxWidgets proj. want change settings of
	OutputDirectory
	IntermediateDirectory
	OutputFile
	AdditionalLibraryDirectories	
	AdditionalDependencies
A2. run bin\run\wxVcprojAuiEditor_Release_Win32.exe, load vcproj file,
	click auto, (check MainSetting) and click save. 
	if want customize setting, change relevant [MAIN/ITEMn] in ini file.
	will link wxWidgets lib with static link.

Q3. as Q2, but want auto create new configruations with dll.
A3. as A2, but click "New DLL" button.
    then in VS2008, remove the proj, and add again. this will trigger vs to 
		add new configs to "Configration Manager"

Q4. how about setting: 
	Tool [Name=VCPostBuildEventTool]
	CommandLine = dumpbin /dependents $(TargetPath)
A4. This will cause vs2008 to show indepents need when build finished.
	Suggested in dynamic link with libraries. This will suggest you what dll
	should be contained when you publish your program.
	Indepent infomation looks like
...	
1>Dump of file [xxx]\bin\run\wxSimpleThread_Debug-DLL_Win32.exe
1>File Type: EXECUTABLE IMAGE
1>  Image has the following dependencies:
1>    KERNEL32.dll
1>    wxbase30ud_vc_custom.dll
1>    wxmsw30ud_core_vc_custom.dll
1>    wxmsw30ud_aui_vc_custom.dll
1>    MSVCP90D.dll
1>    MSVCR90D.dll
1>  Summary
...
	so 3 wxWidgets dll should be included when publish wxSimpleThread_Debug-DLL_Win32.exe.

****procedure:

1. read config from execname.ini,
or use default parameter when ini not exist.
config info stored in MainFrame::m_configIni

2. load vcproj file, and show item in m_configIni
if "All Config" checked, load first 'confiration' in vcproj file(eg. debug-win32 node)
or, load selected confiration in vcproj.

4. predef:
set xml path $(BasePath)[$(BaseName) = $(ConfigurationName)]/$(Predef_Path)
with $(Predef_AttrName) = $(Predef_AttrValue)

$(name) is config item in exename.ini
$(Predef_Path) means "Path" item in "Predef" section

3. main setting:
set xml path: $(BasePath)[$(BaseName) = $(ConfigurationName)]/$(Path)
with $(AttrName) = $(AttrValue)

****UI
Configrations:
	show configrations in vcproj. 
	selected configration will be modify & saved in "MainSetting" when "All config" seleted.
  Auto:
    auto set "AttrValue" in "MainSetting" with exename.ini setting.
  Save: save modification to file. will backup file first (to *.bak).
  
Predef
	show [Predef] settings.
  AddDef:
	add def string to selected configuration.
	
MainSetting
	show [MAIN/ITEMn] items of "ComboConfig"
  "All Config": when selected, disable "ComboConfig", all setting applyed to
	selected configs in "Configrations".
	when not selected. all setting applied to "ComboConfig"
     
  "DLL settings": when selected, use [DLL] setction in setting file over-write [MAIN/ITEMn]

  "NoChange": when selected, all settings remain unchanged when save. 
	 only "Predef" change will be saved.

****exename.ini
[MAIN/ITEMn]:
	items in "MainSetting" section. every items name is destinct, but no order required.

[Predef]:
	"Predef" setion path and value

[DLL]
Predef: predef default value when "DLL Setting" selected
[DLL/ITEMn]
	act like [MAIN/ITEMn], will over-write items in [MAIN/ITEMn] when "DLL Setting" selected.


****note: 
1. reference:
$(ConfigurationName): enumerated from attribute $(AttrName) properties from $(BasePath)
eg. BasePath: /VisualStudioProject/Configurations/Configuration
    probably has $(ConfigurationName): [Debug|Win32] and [Release|Win32]

$(AttrValue): either user input in UI list control or use $(AttrDefault)

2. xml path is similar as a tree structure but can id a node with one attribute:
/path1/path2 [attrname = attrvalue]/path3

[attrname = attrvalue]: attribute attrname with value attrvalue
eg. /VisualStudioProject/Configurations/Configuration [Debug|Win32]/Tool [VCLinkerTool]

VisualStudioProject also has Name attribute but is not necessary because 
there is only one VisualStudioProject.

****difference with wxVcprojEditor
wxVcprojAuiEditor frame is wxAUI enabled.
add predef edit
add auto new dll configuration button
show different config with combo select

****environment
1. wxWidgets for MSW 3.0.2
  MSVC static (shared) compiled. (%WXPATH3%\lib\vc_lib)

reference:
%WXPATH3%\docs\msw\install.txt
  Building wxWidgets
    Microsoft Visual C++ Compilation
      * From command line using the provided makefiles:

suggestion:
  both debug and release compile built:
    nmake /f makefile.vc BUILD=release
    nmake /f makefile.vc BUILD=debug

2. wxFormBuilder 3.05 (Beta) 
  UI (gui.fbp) editor.
  gui.h & gui.cpp are generated automatically from gui.fbp by wxFormBuilder,
  DO NOT edit it manually.
  
3. tinyxml2 (included in project).
  some user code was added. 
  check the file header for detail.
  
4. visual studio 2008
  configuration:
     AdditionalIncludeDirectories: "$(WXPATH3)\include";"$(WXPATH3)\include\msvc"
     AdditionalLibraryDirectories: $(WXPATH3)\lib\vc_lib
 
  change $(WXPATH3) to your wxWidgets root path 
  or set WXPATH3 in system's environment viriable.
  
  in this project, wxWidgets include directory is set in VS2008 golobal environment,
    so no AdditionalIncludeDirectories set. 

