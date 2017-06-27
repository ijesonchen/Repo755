Purpose:

load xml file and show nodes in tree control (wxTreeCtrl)
show selected node¡¯s path in text control (wxTextCtrl)

File -> Open (Alt+O) to open a xml or vcproj file and show.
File -> ExpandChild (Alt+E) to expand or collapse selected item in tree.

Because wxWidgets is statically linked, it should work well with out wxWidgets installed and compiled. MS windows supported only.

Environment:
Visual Studio 2008 with SP1
wxWidgets 3.0.1
tinyxml-2 2.0 (20131105) with modification(see NOTE below)
wxFormBuilder 3.5.0 beta (used to generate and edit gui.fbp, not required)

Configuration:
In Visual Studio 2008 wxWidgets library should be configured.
Suppose wxWidgets is installed in default directory C:\wxWidgets-3.0.1, and is compiled with static lib to default path C:\wxWidgets-3.0.1\lib\vc_lib.
Change project property in Visual Studio 2008:
1. Add include path:
C:\wxWidgets-3.0.1\include
C:\wxWidgets-3.0.1\include\msvc
2. Add lib path:
C:\wxWidgets-3.0.1\lib\vc_lib

How to:
Open solution file wxXmlEditor.sln with Visual Studio 2008, build and run.
Or run bin\run\wxXmlExplorer_Release_Win32.exe.
You can modify UI by editing gui.fbp with wxFormBuilder, generate code and recompile it. 

The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. You can use the 
code or modify freely, but not to violate licence of tinyxml2 and
wxWidgets.
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages 
arising from the use of this software. Use at your own risk.

NOTE!!!
1. The wxWidgets and tinyxml has its own licence.
2. wxWidgets is statically linked without any modification.
3. tinyxml2.h has been modified. All classes XML* is renamed to Ti*,
   and function TiDocument::EncodingType() is added.
