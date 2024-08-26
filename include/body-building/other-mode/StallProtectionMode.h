#pragma once

class StallProtectionMode
{
private:
    StallProtectionMode() = default;

    bool _stall_flag = false;

public:
    static StallProtectionMode &Instance()
    {
        static StallProtectionMode o;
        return o;
    }

    void Execute();

    bool StallFlag() const
    {
        return _stall_flag;
    }

    void SetStallFlag(bool value)
    {
        _stall_flag = value;
    }
};
