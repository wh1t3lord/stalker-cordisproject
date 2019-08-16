#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SE_SmartCover : public CSE_SmartCover
{
public: 
    Script_SE_SmartCover(LPCSTR section_name);
    ~Script_SE_SmartCover(void);

    virtual void STATE_Write(NET_Packet& Packet);
    virtual void STATE_Read(NET_Packet& Packet, std::uint16_t size);

};
}
} // namespace Cordis

