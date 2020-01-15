#include "stdafx.h"
#include "Script_SchemeXRWalker.h"

namespace Cordis
{
namespace Scripts
{
Script_SchemeXRWalker::Script_SchemeXRWalker(const xr_string& action_name, DataBase::Storage_Scheme& storage) : inherited_scheme(nullptr, action_name, storage) {}

Script_SchemeXRWalker::~Script_SchemeXRWalker(void) {}

void Script_SchemeXRWalker::initialize(void) {}

void Script_SchemeXRWalker::execute(void) {}

void Script_SchemeXRWalker::finalize(void) {}

void Script_SchemeXRWalker::activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object) {}

void Script_SchemeXRWalker::update(const float delta) {}

} // namespace Scripts
} // namespace Cordis
