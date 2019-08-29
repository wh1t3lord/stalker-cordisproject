#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_ZoneVisual : public CSE_ALifeZoneVisual
{
    using inherited = CSE_ALifeZoneVisual;

public:
    Script_SE_ZoneVisual(LPCSTR section_name);
    virtual ~Script_SE_ZoneVisual(void);

    virtual void on_register(void);
    virtual void STATE_Read(NET_Packet& packet, std::uint16_t size);

private:
    float m_artefact_spawn_idle;
    float m_artefact_spawn_random;
};
} // namespace Scripts
} // namespace Cordis
