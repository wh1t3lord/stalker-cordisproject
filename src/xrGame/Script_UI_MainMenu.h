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

    inline bool getBoxMode(void) const noexcept { return this->m_boxmode; }
    inline void setBoxMode(bool value) noexcept { this->m_boxmode = value; }
    inline CUIMMShniaga* getShniaga(void) { return this->m_shniaga; }
    inline const CUIMessageBoxEx& getMessageBox(void) { return this->m_message_box; }

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

    static void StartGame(void);

private:
    bool m_boxmode;
    CUIMMShniaga* m_shniaga;
    CUIMessageBoxEx m_message_box;
};
} // namespace Scripts
} // namespace Cordis
