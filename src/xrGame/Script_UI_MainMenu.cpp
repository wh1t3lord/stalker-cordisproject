#include "stdafx.h"
#include "Script_UI_MainMenu.h"
#include "ScriptXMLInit.h"

namespace Cordis
{
	namespace Scripts
	{
		Script_UI_MainMenu::Script_UI_MainMenu(void) : inherited(), m_boxmode(false), m_message_box()
		{
			this->SetWndRect(Frect().set(0,0,1024,768));
			CScriptXmlInit xml = CScriptXmlInit();
			xml.ParseFile("ui_mm_main.xml");
			xml.InitStatic("background", this);
			this->m_shniaga = xml.InitMMShniaga("shniaga_wnd", this);
			this->Register(&this->m_message_box, "msg_box");

		//	CUIStatic* _version = xml.InitStatic("static_version", this);
			CMainMenu* mm = MainMenu();
// 			xr_string version_name = "ver.";
// 			version_name += mm->GetGSVer();
// 			_version->TextItemControl()->SetText(version_name.c_str());
		}

		Script_UI_MainMenu::~Script_UI_MainMenu(void)
		{
			delete this->m_shniaga;
		}


		void Script_UI_MainMenu::Show(bool value)
		{
			this->m_shniaga->SetVisibleMagnifier(value);
		}
	}
}