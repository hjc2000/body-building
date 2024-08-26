#pragma once

enum class AdditionMode_ModeEnum
{
    GraduallyDisableServo = 1,
    GraduallyEnableServo = 2,
    StallProtectMode = 3,
    ResetCalibrateZeroPointModeCompletionStatus = 10,
    None = 9999,
};

class AdditionMode
{
private:
    AdditionMode() = default;

    AdditionMode_ModeEnum _mode = AdditionMode_ModeEnum::None;

public:
    static AdditionMode &Instance()
    {
        static AdditionMode o;
        return o;
    }

    AdditionMode_ModeEnum Mode() const
    {
        return _mode;
    }

    void SetMode(AdditionMode_ModeEnum value)
    {
        _mode = value;
    }

    void Execute();
};
