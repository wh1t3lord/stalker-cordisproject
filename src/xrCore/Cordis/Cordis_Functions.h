#pragma once

#include <any>
#include <functional>
#include "log.h"

namespace Cordis
{
template <typename ReturnType>
struct AnyCallable
{
    AnyCallable(void) = default;
    ~AnyCallable(void) = default;

    template <typename F>
    AnyCallable(F&& func) : AnyCallable(std::function(func))
    {
    }

    template <typename... Args>
    AnyCallable(std::function<ReturnType(Args...)> func) : m_any(func)
    {
    }

    template <typename... Args>
    ReturnType operator()(Args&&... arguments)
    {
        std::function<ReturnType(Args...)> myfunction;

        try
        {
            myfunction = std::any_cast<std::function<ReturnType(Args...)>>(this->m_any);
        }
        catch (...)
        {
            Msg("[Scripts]: ERROR can't cast function!");
            R_ASSERT(false);
            return ReturnType();
        }

        if (!myfunction)
        {
            R_ASSERT2(false, "Can't call your binded function, maybe it doesn't registered");
            return ReturnType();
        }

        return std::invoke(myfunction, std::forward<Args>(arguments)...);
    }

private:
    std::any m_any;
};

template <typename ReturnType, typename... Args>
struct Callback
{
    inline void RegisterCallback(const std::function<ReturnType(Args... arguments)>& myfunction) noexcept
    {
        this->m_callback = myfunction;
    }

    inline bool IsRegistered(void) noexcept { return !!this->m_callback; }

    inline ReturnType Call(Args... arguments)
    {
        if (this->IsRegistered())
            return this->m_callback(arguments...);

        return ReturnType();
    }

private:
    std::function<ReturnType(Args... arguments)> m_callback;
};
} // namespace Cordis
