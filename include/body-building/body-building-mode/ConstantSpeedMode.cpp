#include "ConstantSpeedMode.h"
#include <cmath>

void ConstantSpeedMode::Execute()
{
    double torque = 0;
    if (Servo::Instance().FeedbackSpeed() > DD(11))
    {
        torque = Servo::Instance().FeedbackSpeed() * _infos->Option_TorqueGrowRatio();
        if (torque > _infos->Option_SpeedLimit())
        {
            torque = _infos->Option_SpeedLimit();
        }

        torque = std::pow(torque, 4) / std::pow(_infos->Option_SpeedLimit(), 4) * 180;
        if (torque < 5)
        {
            torque = 5;
        }
        else if (torque > 180)
        {
            torque = 180;
        }

        _cmd->SetSpeed(0);
    }
    else
    {
        torque = _infos->Option_WindingTorque();
        _cmd->SetSpeed(_infos->Option_WindingSpeed_rpm());
    }

    torque = _filter->Input(torque);
    if (torque < 5)
    {
        torque = 5;
    }
    else if (torque > 150)
    {
        torque = 150;
    }

    _cmd->SetTorque(torque);
}
