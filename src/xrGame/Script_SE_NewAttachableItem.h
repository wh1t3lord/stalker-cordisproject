#pragma once

namespace Cordis
{
namespace Scripts
{

class Script_SE_NewAttachableItem : public CSE_ALifeItem
{
    using inherited = CSE_ALifeItem;
public:
    Script_SE_NewAttachableItem(LPCSTR section_name);
    virtual ~Script_SE_NewAttachableItem(void);
};

}
} // namespace Cordis

