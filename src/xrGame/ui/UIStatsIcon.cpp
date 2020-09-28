#include "StdAfx.h"
#include "UIStatsIcon.h"
#include "xrUICore/XML/UITextureMaster.h"
#include "UIInventoryUtilities.h"

#include "UIShader.h"

//CUIStatsIcon::TEX_INFO CUIStatsIcon::m_tex_info[MAX_DEF_TEX][2];

CUIStatsIcon::CUIStatsIcon()
{
    SetStretchTexture(true);
    InitTexInfo();
}

void CUIStatsIcon::InitTexInfo()
{
    if (CUIStatsIconMaterials::getInstance().m_tex_info[RANK_0][0].sh->inited())
        return;
    // ranks
    string128 rank_tex;
    for (int i = RANK_0; i <= RANK_4; i++)
    {
        xr_sprintf(rank_tex, "ui_hud_status_green_0%d", i + 1);
        CUITextureMaster::GetTextureShader(rank_tex, CUIStatsIconMaterials::getInstance().m_tex_info[i][0].sh);
        CUIStatsIconMaterials::getInstance().m_tex_info[i][0].rect = CUITextureMaster::GetTextureRect(rank_tex);

        xr_sprintf(rank_tex, "ui_hud_status_blue_0%d", i + 1);
        CUITextureMaster::GetTextureShader(rank_tex, CUIStatsIconMaterials::getInstance().m_tex_info[i][1].sh);
        CUIStatsIconMaterials::getInstance().m_tex_info[i][1].rect = CUITextureMaster::GetTextureRect(rank_tex);
    }

    // artefact
    LPCSTR artefact_name = pSettings->r_string("artefacthunt_gamedata", "artefact");
    float fGridWidth = pSettings->r_float(artefact_name, "inv_grid_width");
    float fGridHeight = pSettings->r_float(artefact_name, "inv_grid_height");
    float fXPos = pSettings->r_float(artefact_name, "inv_grid_x");
    float fYPos = pSettings->r_float(artefact_name, "inv_grid_y");

    CUIStatsIconMaterials::getInstance().m_tex_info[ARTEFACT][0].sh = InventoryUtilities::GetEquipmentIconsShader();
    CUIStatsIconMaterials::getInstance().m_tex_info[ARTEFACT][0].rect.set(fXPos * INV_GRID_WIDTH, fYPos * INV_GRID_HEIGHT,
        fXPos * INV_GRID_WIDTH + fGridWidth * INV_GRID_WIDTH, fYPos * INV_GRID_HEIGHT + fGridHeight * INV_GRID_HEIGHT);

    CUIStatsIconMaterials::getInstance().m_tex_info[ARTEFACT][1] = CUIStatsIconMaterials::getInstance().m_tex_info[ARTEFACT][0];

    // death
    CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].sh->create("hud" DELIMITER "default", "ui" DELIMITER "ui_mp_icon_kill");
    CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][1] = CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0];
    CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].rect.x1 = 32;
    CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].rect.y1 = 202;
    CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].rect.x2 = CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].rect.x1 + 30;
    CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].rect.y2 = CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].rect.y1 + 30;
}

void CUIStatsIcon::FreeTexInfo()
{
    // ranks
    for (int i = RANK_0; i <= RANK_4; i++)
    {
        CUIStatsIconMaterials::getInstance().m_tex_info[i][0].sh->destroy();
        CUIStatsIconMaterials::getInstance().m_tex_info[i][1].sh->destroy();
    }
    CUIStatsIconMaterials::getInstance().m_tex_info[ARTEFACT][0].sh->destroy();
    CUIStatsIconMaterials::getInstance().m_tex_info[ARTEFACT][1].sh->destroy();
    CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].sh->destroy();
    CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][1].sh->destroy();
}

void CUIStatsIcon::SetValue(LPCSTR str)
{
    if (0 == str[0])
    {
        SetVisible(false);
        return;
    }
    else
        SetVisible(true);

    if (strstr(str, "status"))
    {
        int team = 1;
        if (strstr(str, "green"))
            team = 0;

        int rank = atoi(strstr(str, "0")) - 1;

        SetShader(CUIStatsIconMaterials::getInstance().m_tex_info[rank][team].sh);
        SetTextureRect(CUIStatsIconMaterials::getInstance().m_tex_info[rank][team].rect);
    }
    else if (0 == xr_strcmp(str, "death"))
    {
        SetShader(CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].sh);
        SetTextureRect(CUIStatsIconMaterials::getInstance().m_tex_info[DEATH][0].rect);
    }
    else if (0 == xr_strcmp(str, "artefact"))
    {
        SetShader(CUIStatsIconMaterials::getInstance().m_tex_info[ARTEFACT][0].sh);
        SetTextureRect(CUIStatsIconMaterials::getInstance().m_tex_info[ARTEFACT][0].rect);
    }
    else
    {
        InitTexture(str);
    }
}
