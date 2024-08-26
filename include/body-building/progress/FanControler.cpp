#include "FanControler.h"
#include <body-building/io/Option.h>
#include <body-building/io/Servo.h>
#include <cmath>

std::chrono::seconds FanControler::TurnOnFanDelay()
{
    return std::chrono::seconds{SRV_PARA(3, 92)};
}

std::chrono::seconds FanControler::TurnOffFanDelay()
{
    return std::chrono::seconds{SRV_PARA(3, 93)};
}

int FanControler::TurnOnDelayTick()
{
    // 脚本定时器周期是 2ms，所以乘上 500，每一秒对应 500 个计数。
    return TurnOnFanDelay().count() * 500;
}

int FanControler::TurnOffDelayTick()
{
    // 脚本定时器周期是 2ms，所以乘上 500，每一秒对应 500 个计数。
    return TurnOffFanDelay().count() * 500;
}

void FanControler::ControlByTick()
{
    _hysteresis_element.ChangeThreshold(base::HysteresisElement_RisingThreshold{static_cast<double>(TurnOnDelayTick())},
                                        base::HysteresisElement_FallenThreshold{static_cast<double>(TurnOnDelayTick() - TurnOffDelayTick())});

    if (Servo::Instance().FeedbackPosition() >= Option::Instance().ZeroPositionProtectionThreshold())
    {
        if (_hysteresis_element.CurrentInput() < _hysteresis_element.RisingThreshold())
        {
            /* 这里通过判断对输入进行限幅
             * 输入刚好等于迟滞环节阈值时也会触发上升或下降
             */
            _hysteresis_element.Input(_hysteresis_element.CurrentInput() + 1);
        }
    }
    else
    {
        if (_hysteresis_element.CurrentInput() > _hysteresis_element.FallenThreshold())
        {
            /* 这里通过判断对输入进行限幅
             * 输入刚好等于迟滞环节阈值时也会触发上升或下降
             */
            _hysteresis_element.Input(_hysteresis_element.CurrentInput() - 1);
        }
    }

    if (_hysteresis_element.OutputChange() == base::HysteresisElement_OutputChange::Rise)
    {
        ServoFan::Instance().TurnOn();
    }
    else if (_hysteresis_element.OutputChange() == base::HysteresisElement_OutputChange::Fall)
    {
        ServoFan::Instance().TurnOff();
    }
}

void FanControler::Execute()
{
    // TODO: 看一下这些参数是什么
    /*
     --- 风扇控制
    function FanControl()
        if (Option_CurrentWorkMode() == 5) then
            --待机模式
            Servo_TurnOffFan()
        elseif (SRV_PARA(3, 81) == 1) then
            if (SRV_MON(21) < 35) then
                Servo_TurnOffFan()
            else
                --温控风扇
                SRV_PARA(2, 76, 4, 0)
            end
        else
            --风扇控制
            FanControlBasedOnTick()
        end
    end
    */

    if (Option::Instance().BodyBuildingMode() == Option_BodyBuildingMode::Standby)
    {
        ServoFan::Instance().TurnOff();
        return;
    }

    ControlByTick();
}
