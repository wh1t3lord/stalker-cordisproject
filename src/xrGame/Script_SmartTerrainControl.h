#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SmartTerrainControl
{
public:
    Script_SmartTerrainControl(Script_SE_SmartTerrain* smart, const CInifile& ini, const xr_string& section);
    ~Script_SmartTerrainControl(void);
};
}
} // namespace Cordis

