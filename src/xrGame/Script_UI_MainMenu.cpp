#include "stdafx.h"
#include "Script_UI_MainMenu.h"
#include "ScriptXMLInit.h"

namespace Cordis
{
namespace Scripts
{
bool Script_UI_MainMenu::m_boxmode = false;

Script_UI_MainMenu::Script_UI_MainMenu(void) : inherited()
{
    this->SetWndRect(Frect().set(0, 0, 1024, 768));
    CScriptXmlInit xml = CScriptXmlInit();
    xml.ParseFile("ui_mm_main.xml");
    xml.InitStatic("background", this);
    this->m_shniaga = xml.InitMMShniaga("shniaga_wnd", this);
    this->Register(&getMessageBox(), "msg_box");

    CUIStatic* _version = xml.InitStatic("static_version", this);
    CMainMenu* mm = MainMenu();
    xr_string version_name = "ver.";
    version_name += mm->GetGSVer();
    _version->TextItemControl()->SetText(version_name.c_str());

    this->AddCallback("btn_novice", BUTTON_CLICKED, OnButton_new_novice_game);
    this->AddCallback("btn_stalker", BUTTON_CLICKED, OnButton_new_stalker_game);
    this->AddCallback("btn_veteran", BUTTON_CLICKED, OnButton_new_veteran_game);
    this->AddCallback("btn_master", BUTTON_CLICKED, OnButton_new_master_game);
    this->AddCallback("btn_options", BUTTON_CLICKED, OnButton_options_clicked);
    this->AddCallback("btn_load", BUTTON_CLICKED, OnButton_load_clicked);
    this->AddCallback("btn_save", BUTTON_CLICKED, OnButton_save_clicked);
    this->AddCallback("btn_quit", BUTTON_CLICKED, OnButton_quit_clicked);
    this->AddCallback("btn_quit_to_mm", BUTTON_CLICKED, OnButton_disconnect_clicked);
    this->AddCallback("btn_ret", BUTTON_CLICKED, OnButton_return_to_game_clicked);
    this->AddCallback("btn_lastsave", BUTTON_CLICKED, OnButton_last_save_clicked);
    this->AddCallback("btn_credits", BUTTON_CLICKED, OnButton_credits_clicked);
    this->AddCallback("msg_box", MESSAGE_BOX_OK_CLICKED, OnMsgOk);
    this->AddCallback("msg_box", MESSAGE_BOX_CANCEL_CLICKED, OnMsgCancel);
    this->AddCallback("msg_box", MESSAGE_BOX_YES_CLICKED, OnMsgYes);
    this->AddCallback("msg_box", MESSAGE_BOX_NO_CLICKED, OnMsgNo);
    this->AddCallback("msg_box", MESSAGE_BOX_QUIT_GAME_CLICKED, OnMessageQuitGame);
    this->AddCallback("msg_box", MESSAGE_BOX_QUIT_WIN_CLICKED, OnMessageQuitWin);
    // Lord: xr_s.on_main_menu_on() Dobavit!!!!!
}

Script_UI_MainMenu::~Script_UI_MainMenu(void) {}

void Script_UI_MainMenu::Show(bool value)
{
    inherited::Show(value);
    this->m_shniaga->SetVisibleMagnifier(value);
}

bool Script_UI_MainMenu::OnKeyboardAction(int dik, EUIMessages keyboard_action)
{
    inherited::OnKeyboardAction(dik, keyboard_action);

    if (keyboard_action == WINDOW_KEY_PRESSED)
    {
        if (dik == SDL_SCANCODE_ESCAPE)
        {
            if (Globals::Game::level::is_level_present())
            {
                if (DataBase::Storage::getInstance().getActor() &&
                        DataBase::Storage::getInstance().getActor()->Alive() ||
                    !IsGameTypeSingle())
                {
                    OnButton_return_to_game_clicked();
                }
            }
        }

        if (dik == SDL_SCANCODE_Q)
            OnMessageQuitWin();
    }

    return true;
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

void Script_UI_MainMenu::LoadLastSave(void)
{
    Console->Execute("main_menu off");
    Console->Execute("load_last_save");
}

} // namespace Scripts
} // namespace Cordis
