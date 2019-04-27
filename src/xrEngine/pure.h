#pragma once
#include "xrCommon/xr_vector.h"

// messages
constexpr int REG_PRIORITY_LOW = 0x11111111;
constexpr int REG_PRIORITY_NORMAL = 0x22222222;
constexpr int REG_PRIORITY_HIGH = 0x33333333;
constexpr int REG_PRIORITY_CAPTURE = 0x7fffffff;
constexpr unsigned int REG_PRIORITY_INVALID = 0x80000000; // -2147483648, lowest for int


struct IPure
{
    virtual ~IPure(void) noexcept = default;
    virtual void OnPure(void) = 0;
};

struct pureFrame : IPure
{
    virtual void OnFrame() = 0;

private:
    void OnPure(void) override { OnFrame(); }
};

struct pureFrameEnd : IPure
{
    virtual void OnFrameEnd(void) = 0;

private:
    void OnPure(void) override { OnFrameEnd(); }
};

struct pureRender : IPure
{
    virtual void OnRender(void) = 0;

private:
    void OnPure(void) override { OnRender(); }
};

struct pureAppActivate : IPure
{
    virtual void OnAppActivate(void) = 0;

private:
    void OnPure(void) override { OnAppActivate(); }
};

struct pureAppStart : IPure
{
    virtual void OnAppStart(void) = 0;

private:
    void OnPure(void) override { OnAppStart(); }
};

struct pureAppDeactivate : IPure
{
    virtual void OnAppDeactivate(void) = 0;

private:
    void OnPure(void) override { OnAppDeactivate(); }
};

struct pureAppEnd : IPure
{
    virtual void OnAppEnd(void) = 0;

private:
    void OnPure(void) override { OnAppEnd(); }
};

struct pureDeviceReset : IPure
{
    virtual void OnDeviceReset(void) = 0;

private:
    void OnPure(void) override { OnDeviceReset(); }
};

struct pureUIReset : IPure
{
    virtual void OnUIReset(void) = 0;

private:
    void OnPure(void) override { OnUIReset(); }
};

struct pureScreenResolutionChanged : IPure
{
    virtual void OnScreenResolutionChanged(void) = 0;

private:
    void OnPure(void) override { OnScreenResolutionChanged(); }
};

struct MessageObject
{
    IPure* Object;
    int Prio;
};

template <class T>
class MessageRegistry
{
    bool changed, inProcess;
    xr_vector<MessageObject> messages;

public:
    MessageRegistry() : changed(false), inProcess(false) {}

    void Clear() { messages.clear(); }

    constexpr void Add(T* object, const int priority = REG_PRIORITY_NORMAL) { Add({object, priority}); }

    void Add(MessageObject&& newMessage)
    {
#ifdef DEBUG
        VERIFY(newMessage.Object);
        VERIFY(newMessage.Prio != REG_PRIORITY_INVALID);

        // Verify that we don't already have the same object with valid priority
        for (auto& message : messages)
            VERIFY(!(message.Prio != REG_PRIORITY_INVALID && message.Object == newMessage.Object));
#endif
        messages.emplace_back(newMessage);

        if (inProcess)
            changed = true;
        else
            Resort();
    }

    void Remove(T* object)
    {
        for (auto& it : messages)
        {
            if (it.Object == object)
                it.Prio = REG_PRIORITY_INVALID;
        }

        if (inProcess)
            changed = true;
        else
            Resort();
    }

    void Process()
    {
        if (messages.empty())
            return;

        inProcess = true;

        if (messages[0].Prio == REG_PRIORITY_CAPTURE)
            messages[0].Object->OnPure();
        else
        {
            for (int i = 0; i < messages.size(); ++i)
                if (messages[i].Prio != REG_PRIORITY_INVALID)
                    messages[i].Object->OnPure();
        }

        if (changed)
            Resort();

        inProcess = false;
    }

    void Resort()
    {
        if (!messages.empty())
        {
            std::sort(
                std::begin(messages), std::end(messages), [](const auto& a, const auto& b) { return a.Prio > b.Prio; });
        }

        while (!messages.empty() && messages.back().Prio == REG_PRIORITY_INVALID)
            messages.pop_back();

        if (messages.empty())
            messages.clear();

        changed = false;
    }
};
