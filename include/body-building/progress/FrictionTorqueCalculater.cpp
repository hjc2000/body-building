#include "FrictionTorqueCalculater.h"
#include <body-building/io/Option.h>
#include <body-building/io/Servo.h>
#include <cmath>

int FrictionTorqueCalculater::WindingFrictionTorque() const
{
    return 4 * Option::Instance().TorqueRatio();
}

int FrictionTorqueCalculater::UnwindingFrictionTorque(double tension_kg) const
{
    double outFcRatio = 15;
    double outFcSet = 0.4;
    return (outFcSet - (tension_kg / outFcRatio)) * Option::Instance().TorqueRatio();
}

int FrictionTorqueCalculater::Calculate(double tension_kg)
{
    double mcl = 2;
    if (Servo::Instance().FeedbackSpeed() >= 0 && Servo::Instance().FeedbackSpeed() < mcl)
    {
        // 低速放线
        double base = UnwindingFrictionTorque(tension_kg);
        double ratio = std::abs(Servo::Instance().FeedbackSpeed()) / mcl;
        _current_friction_torque = base * ratio;
    }
    else if (Servo::Instance().FeedbackSpeed() < 0 && Servo::Instance().FeedbackSpeed() > -mcl)
    {
        // 低速收线
        double base = WindingFrictionTorque();
        double ratio = std::abs(Servo::Instance().FeedbackSpeed()) / mcl;
        _current_friction_torque = base * ratio;
    }
    else if (Servo::Instance().FeedbackSpeed() > mcl)
    {
        // 高速放线
        _current_friction_torque = UnwindingFrictionTorque(tension_kg);
    }
    else
    {
        // 高速收线
        _current_friction_torque = WindingFrictionTorque();
    }

    _current_friction_torque = static_cast<int>(_filter.Input(_current_friction_torque));
    _last_friction_torque = _current_friction_torque;
    return _current_friction_torque;
}
