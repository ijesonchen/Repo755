#ifndef wxXmlCommon_h__
#define wxXmlCommon_h__

/*
Original code by Jeson Chen (jesonsara@gmail.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/aui/framemanager.h>

#include "..\tinyxml2\tinyxml2.h"

namespace wxXmlCommon
{
	// show XML file strFileName content
	void PrintXml(wchar_t* strFileName);

	// show all content of pe, including child element
	void PrintAllText(const tinyxml2::TiElement* pe);

	// show name, attribute and text of pe, no child.
	void PrintElement(const tinyxml2::TiElement* pe);

	// only add element to tree, without children
	wxTreeItemId AddElmtOnlyToTree(wxTreeCtrl* const pTree, const wxTreeItemId& idParent, const tinyxml2::TiElement* const pe, bool bUtf8 = false);

	// add element and children to tree
	wxTreeItemId AddElmtToTree(wxTreeCtrl* const pTree, const wxTreeItemId& idParent, const tinyxml2::TiElement* const pe, bool bUtf8 = false);

	//////////////////////////////////////////////////////////////////////////
	// test functions 
	void TestTemp(void);
}


#endif // wxXmlCommon_h__
