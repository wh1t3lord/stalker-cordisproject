#pragma once
class SDK_Resource
{
private:
    SDK_Resource(void) = default;

public:
    inline static SDK_Resource& GetInstance(void)
    { 
        static SDK_Resource instance;
        return instance;
    }

    SDK_Resource(const SDK_Resource&) = delete;
    SDK_Resource& operator=(const SDK_Resource&) = delete;
    SDK_Resource(SDK_Resource&&) = delete;
    SDK_Resource& operator=(SDK_Resource&&) = delete;

    void Initialize(void);
    ~SDK_Resource(void);

private:

};
