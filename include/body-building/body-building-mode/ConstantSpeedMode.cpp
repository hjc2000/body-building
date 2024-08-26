#include "ConstantSpeedMode.h"
#include <cmath>

#pragma region 选项

double ConstantSpeedMode::TorqueGrowRatio()
{
    switch (_sub_mode)
    {
    case 0:
        {
            return SRV_PARA(1, 39) / 1000;
        }
    case 1:
        {
            return 1;
        }
    case 2:
        {
            return 1.45;
        }
    case 3:
        {
            return 2;
        }
    default:
    case 4:
        {
            return 0.6;
        }
    }
}

double ConstantSpeedMode::SpeedLimit()
{
    switch (_sub_mode)
    {
    case 0:
        {
            return SRV_PARA(1, 21) / 100;
        }
    case 1:
        {
            return 1400;
        }
    case 2:
        {
            return 1100;
        }
    case 3:
        {
            return 1;
        }
    default:
    case 4:
        {
            return 1800;
        }
    }
}

int ConstantSpeedMode::Torque()
{
    switch (_sub_mode)
    {
    case 0:
        {
            return SRV_PARA(2, 18);
        }
    case 1:
        {
            return 13;
        }
    case 2:
        {
            return 30;
        }
    case 3:
        {
            return 45;
        }
    default:
    case 4:
        {
            return 6;
        }
    }
}

double ConstantSpeedMode::WindingSpeed()
{
    switch (_sub_mode)
    {
    case 0:
        {
            return SRV_PARA(1, 10) / 100;
        }
    case 1:
        {
            return 600;
        }
    case 2:
        {
            return 400;
        }
    case 3:
        {
            return 300;
        }
    default:
    case 4:
        {
            return 800;
        }
    }
}

int ConstantSpeedMode::FilterGain()
{
    switch (_sub_mode)
    {
    case 0:
        {
            return SRV_PARA(1, 20);
        }
    case 1:
        {
            return 200;
        }
    case 2:
        {
            return 200;
        }
    case 3:
        {
            return 1000;
        }
    default:
    case 4:
        {
            return 500;
        }
    }
}

#pragma endregion

void ConstantSpeedMode::Execute()
{
    if (Option::Instance().BodyBuildingModeChanged())
    {
        _filter = CreateFilter();
    }

    double torque = 0;
    if (Servo::Instance().FeedbackSpeed() > DD(11))
    {
        torque = Servo::Instance().FeedbackSpeed() * TorqueGrowRatio();
        if (torque > SpeedLimit())
        {
            torque = SpeedLimit();
        }

        torque = std::pow(torque, 4) / std::pow(SpeedLimit(), 4) * 180;
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
        torque = Torque();
        _cmd->SetSpeed(WindingSpeed());
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
