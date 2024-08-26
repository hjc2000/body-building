#pragma once
#include <base/math/HysteresisElement.h>
#include <chrono>

/// @brief 风扇控制器。
class FanControler
{
private:
    FanControler() = default;

    std::chrono::seconds TurnOnFanDelay();
    std::chrono::seconds TurnOffFanDelay();
    int TurnOnDelayTick();
    int TurnOffDelayTick();

    base::HysteresisElement _hysteresis_element{
        base::HysteresisElement_RisingThreshold{static_cast<double>(TurnOnDelayTick())},
        base::HysteresisElement_FallenThreshold{static_cast<double>(TurnOnDelayTick() - TurnOffDelayTick())},
    };

    /// @brief 基于计时的控制方式。
    void ControlByTick();

public:
    static FanControler &Instance()
    {
        static FanControler o;
        return o;
    }

    void Execute();
};
