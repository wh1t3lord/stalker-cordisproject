#pragma once

namespace Cordis
{
namespace Scripts
{
namespace GulagGenerator
{
inline static xr_string& getLtx(void) noexcept
{
    static xr_string instance = "";
    return instance;
}

bool load_job(Script_SE_SmartTerrain* smart) 
{

}

} // namespace GulagGenerator
} // namespace Scripts
} // namespace Cordis
