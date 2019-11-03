#include "stdafx.h"
#include "Script_UI_MainMenu.h"
#include "ScriptXMLInit.h"

namespace Cordis
{
namespace Scripts
{
Script_UI_MainMenu::Script_UI_MainMenu(void) : inherited(), m_boxmode(false), m_message_box()
{
    this->SetWndRect(Frect().set(0, 0, 1024, 768));
    CScriptXmlInit xml = CScriptXmlInit();
    xml.ParseFile("ui_mm_main.xml");
    xml.InitStatic("background", this);
    this->m_shniaga = xml.InitMMShniaga("shniaga_wnd", this);
    this->Register(&this->m_message_box, "msg_box");

    CUIStatic* _version = xml.InitStatic("static_version", this);
    CMainMenu* mm = MainMenu();
    xr_string version_name = "ver.";
    version_name += mm->GetGSVer();
    _version->TextItemControl()->SetText(version_name.c_str());

    this->AddCallback("btn_novice", BUTTON_CLICKED, OnButton_new_novice_game);
    this->AddCallback("btn_stalker", BUTTON_CLICKED, OnButton_new_stalker_game);
    this->AddCallback("btn_veteran", BUTTON_CLICKED, OnButton_new_veteran_game);
    this->AddCallback("btn_master", BUTTON_CLICKED, OnButton_new_master_game);
    // Lord: xr_s.on_main_menu_on() Dobavit!!!!! 
}

Script_UI_MainMenu::~Script_UI_MainMenu(void) {}

void Script_UI_MainMenu::Show(bool value)
{
    inherited::Show(value);
    this->m_shniaga->SetVisibleMagnifier(value);
}

void Script_UI_MainMenu::StartGame(void)
{
    if (ai().get_alife())
    {
        Console->Execute("disconnect");
    }

    Globals::set_device_paused(false);
    Console->Execute("start server(all/single/alife/new) client(localhost)");
    Console->Execute("main_menu off");
}

} // namespace Scripts
} // namespace Cordis
