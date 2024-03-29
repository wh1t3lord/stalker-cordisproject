#include "stdafx.h"
#include "Script_SchemeSRSilenceZone.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeSRSilenceZone::Script_SchemeSRSilenceZone(
    CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRSilenceZone* storage)
    : inherited_scheme(p_client_object, storage), m_p_storage(storage)
{ 
    this->m_scheme_name = "sr_silence";
}
Script_SchemeSRSilenceZone::~Script_SchemeSRSilenceZone(void) {}
} // namespace Scripts
} // namespace Cordis
