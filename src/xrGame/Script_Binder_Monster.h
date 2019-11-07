#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_Binder_Monster : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;

public:
    Script_Binder_Monster(void) = delete;
    Script_Binder_Monster(CScriptGameObject* object);
    virtual ~Script_Binder_Monster(void);
    virtual void reinit(void);
    virtual void reload(LPCSTR section_name);
    virtual bool net_Spawn(SpawnType DC);
    virtual void net_Destroy(void);
    virtual void net_Import(NET_Packet* packet);
    virtual void net_Export(NET_Packet* packet);
    virtual void shedule_Update(std::uint32_t time_delta);
    virtual void save(NET_Packet* output_packet);
    virtual void load(IReader* input_packet);
    virtual bool net_SaveRelevant(void);
    virtual void net_Relcase(CScriptGameObject* object);
};
}
} // namespace Cordis
