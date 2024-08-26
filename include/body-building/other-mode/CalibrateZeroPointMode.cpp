#include "CalibrateZeroPointMode.h"
#include <body-building/io/Cmd.h>
#include <body-building/io/Option.h>
#include <body-building/io/Servo.h>
#include <cmath>

void CalibrateZeroPointMode::Execute()
{
    if (_is_completed)
    {
        _progress = 2;
        return;
    }

    _progress = 1;
    _cmd->SetSpeed(Option::Instance().ReturnToZeroSpeed());
    _cmd->SetTorque(Option::Instance().ReturnToZeroTorque());

    if (std::abs(Servo::Instance().FeedbackSpeed()) < 20)
    {
        _zero_speed_holding_tick++;
        if (_zero_speed_holding_tick > 1000)
        {
            _zero_speed_holding_tick = 0;
            _cmd->SetSpeed(0);
            _cmd->SetTorque(0);
            _cmd->SendToServo();
            _is_completed = true;
            _progress = 2;
            Servo::Instance().SetCurrentPositonAsZeroPoint();

            // 触发回调
            if (_on_completed)
            {
                _on_completed();
            }
        }
    }
    else
    {
        _zero_speed_holding_tick = 0;
    }
}

bool CalibrateZeroPointMode::IsCompleted() const
{
    return _is_completed;
}

int CalibrateZeroPointMode::Progress()
{
    return _progress;
}

void CalibrateZeroPointMode::ResetCompletionStatus()
{
    _is_completed = false;
    _progress = 1;
}
