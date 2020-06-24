#pragma once

namespace Cordis
{
namespace Scripts
{
namespace CRD_Wounded
{
inline bool is_wounded(CScriptGameObject* object) { return false; }

inline bool is_heavy_wounded_by_id(const std::uint16_t npc_id) 
{
    if (npc_id == 0 || npc_id == Globals::kUnsignedInt16Undefined)
    {
        MESSAGEWR("passed invalid id!");
        return false;
    }

    if (DataBase::Storage::getInstance().getStorage().find(npc_id) == DataBase::Storage::getInstance().getStorage().end())
    {
        MESSAGEWR("Early calling!");
        return false;
    }

    const DataBase::Storage_Data& storage = DataBase::Storage::getInstance().getStorage().at(npc_id);
    
    if (storage.getSchemes().find("wounded") == storage.getSchemes().end())
    {
        MESSAGEWR("scheme doesn't exist!");
        return false;
    }

    DataBase::Script_ComponentScheme_XRWounded* const p_storage = static_cast<DataBase::Script_ComponentScheme_XRWounded*>(storage.getSchemes().at("wounded"));

    if (p_storage == nullptr)
    {
        MESSAGEWR("invalid storage!");
        return false;
    }

    if (p_storage->getWoundedManager() == nullptr)
    {
        MESSAGEWR("Something wrong wounded manager is nullptr!");
        return false;
    }

    if (p_storage->getWoundedManager()->getStateName().empty() == false && p_storage->getWoundedManager()->getStateName() != "nil")
        return true;

    return false; 
}

} // namespace CRD_Wounded
} // namespace Scripts
} // namespace Cordis
