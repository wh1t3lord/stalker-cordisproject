#include "stdafx.h"
#include "Script_SE_SmartTerrain.h"

namespace Cordis
{
namespace Scripts
{
Script_SE_SmartTerrain::Script_SE_SmartTerrain(LPCSTR section) : inherited(section)
{

}

Script_SE_SmartTerrain::~Script_SE_SmartTerrain(void)
{
    if (this->m_base_on_actor_control)
        delete this->m_base_on_actor_control;
}
void Script_SE_SmartTerrain::read_params(void) {}
} // namespace Scripts
} // namespace Cordis
