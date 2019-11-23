#include "stdafx.h"
#include "Script_SchemeMobWalker.h"

namespace Cordis
{
namespace Scripts
{
std::uint16_t default_wait_time = 5000;

const std::uint8_t state_moving = 0;
const std::uint8_t state_standing = 1;

Script_SchemeMobWalker::Script_SchemeMobWalker(
    CScriptGameObject* p_client_object, DataBase::Storage_Scheme& storage)
    : inherited_scheme(p_client_object, storage)
{
    this->m_scheme_name = "mob_walker";
}

Script_SchemeMobWalker::~Script_SchemeMobWalker(void) {}
} // namespace Scripts
} // namespace Cordis
