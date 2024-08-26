#pragma once
#include <base/math/InertialElement.h>
#include <body-building/io/Option.h>
#include <body-building/io/Servo.h>
#include <cmath>

/// @brief 睡眠控制器。
/// @note 会自动禁用和使能伺服。
class SleepControler
{
private:
    SleepControler() = default;

    base::InertialElement _speed_filter{
        base::InertialElement_TimeConstant{0.025},
        base::InertialElement_SampleInterval{0.002},
    };

    int64_t _auto_sleep_delay_tick = 0;

public:
    static SleepControler &Instance()
    {
        static SleepControler o;
        return o;
    }

    void Execute();
};
