#pragma once

namespace Cordis
{
    namespace SDK
    {
    class SDK_KeybindingManager
    {
    private:
        SDK_KeybindingManager(void) = default;

    public:
        inline static SDK_KeybindingManager& GetInstance(void) noexcept
        {
            static SDK_KeybindingManager instance;
            return instance;
        }

        ~SDK_KeybindingManager(void);

        void ParseData(void);

    private:
    };

    // KEY_ID ->

    /*
        kMap -> <Action, KEY_ID>
        ParseData() ->
    */

    }
}
