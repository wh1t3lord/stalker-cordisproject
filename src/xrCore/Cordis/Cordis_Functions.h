#pragma once

#include <any>
#include <functional>

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
        std::function<ReturnType(Args...)> myfunction = std::any_cast<std::function<ReturnType(Args...)>>(this->m_any);
        if (!myfunction)
        {
            R_ASSERT2(false, "Can't call your binded function, maybe it doesn't registered");
            return ReturnType();
        }

        ReturnType result;
        try
        {
            result = std::invoke(myfunction, std::forward<Args>(arguments)...);
        }
        catch (...)
        {
            R_ASSERT2(false, "Something goes wrong! Can't invoke binded function, please check your callable arguments, which are passed to operator ()...");
            result = ReturnType();
        }

        return result;
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
