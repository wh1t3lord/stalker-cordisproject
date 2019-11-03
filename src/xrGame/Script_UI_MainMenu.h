#pragma once

#include "UI/UIScriptWnd.h"
#include "UI/UIMessageBoxEx.h"
#include "UI/UIMMShniaga.h"

namespace Cordis
{
namespace Scripts
{
class Script_UI_MainMenu : public CUIDialogWndEx
{
    using inherited = CUIDialogWndEx;

public:
    Script_UI_MainMenu(void);
    virtual ~Script_UI_MainMenu(void);

    void Show(bool value);
    virtual bool OnKeyboardAction(int dik, EUIMessages keyboard_action);
    inline bool getBoxMode(void) const noexcept { return this->m_boxmode; }
    inline void setBoxMode(bool value) noexcept { this->m_boxmode = value; }
    inline CUIMMShniaga* getShniaga(void) { return this->m_shniaga; }
    //  inline const CUIMessageBoxEx& getMessageBox(void) { return this->m_message_box; }

    inline static void OnButton_new_novice_game(void)
    {
        Console->Execute("g_game_difficulty gd_novice");
        StartGame();
    }

    inline static void OnButton_new_stalker_game(void)
    {
        Console->Execute("g_game_difficulty gd_novice");
        StartGame();
    }

    inline static void OnButton_new_veteran_game(void)
    {
        Console->Execute("g_game_difficulty gd_veteran");
        StartGame();
    }

    inline static void OnButton_new_master_game(void)
    {
        Console->Execute("g_game_difficulty gd_master");
        StartGame();
    }

    inline static void OnButton_options_clicked(void) {}

    inline static void OnButton_load_clicked(void) {}

    inline static void OnButton_save_clicked(void) {}

    inline static void OnButton_quit_clicked(void)
    {
        getMessageBox().InitMessageBox("message_box_quit_windows");
        getMessageBox().ShowDialog(true);
    }

    inline static void OnButton_disconnect_clicked(void) {}

    inline static void OnButton_return_to_game_clicked(void) {}

    inline static void OnButton_last_save_clicked(void)
    {
        if (!ai().get_alife())
        {
            LoadLastSave();
            return;
        }

        if (DataBase::Storage::getInstance().getActor())
        {
            if (!DataBase::Storage::getInstance().getActor()->Alive())
            {
                LoadLastSave();
                return;
            }
        }

        m_boxmode = true;

        getMessageBox().InitMessageBox("message_box_confirm_load_save");
        getMessageBox().ShowDialog(true);
    }

    inline static void OnButton_credits_clicked(void) { Globals::Game::start_tutorial("credits_seq"); }

    inline static void OnMsgOk(void) { m_boxmode = false; }

    inline static void OnMsgCancel(void) { m_boxmode = false; }

    inline static void OnMsgYes(void)
    {
        if (m_boxmode)
            LoadLastSave();

        m_boxmode = false;
    }

    inline static void OnMsgNo(void) { m_boxmode = false; }

    inline static void OnMessageQuitGame(void) 
    { 
        Console->Execute("disconnect");
    }

    inline static void OnMessageQuitWin(void) { Console->Execute("quit"); }

    static void StartGame(void);
    static void LoadLastSave(void);

private:
    inline static CUIMessageBoxEx& getMessageBox(void) noexcept
    {
        static CUIMessageBoxEx instance;
        return instance;
    }

private:
    static bool m_boxmode;
    CUIMMShniaga* m_shniaga;
};
} // namespace Scripts
} // namespace Cordis
