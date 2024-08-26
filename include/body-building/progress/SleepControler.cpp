#include "SleepControler.h"

void SleepControler::Execute()
{
    double filter_out_speed = _speed_filter.Input(std::abs(Servo::Instance().FeedbackSpeed()));

    if (filter_out_speed < 10 &&
        Servo::Instance().FeedbackPosition() < Option::Instance().ZeroPositionProtectionThreshold() &&
        Servo::Instance().TorqueLimit() < Option::Instance().MinTorque())
    {
        _auto_sleep_delay_tick++;
        if (_auto_sleep_delay_tick > 1000)
        {
            _auto_sleep_delay_tick = 1000;
            Servo::Instance().Disable();
        }
    }
    else
    {
        _auto_sleep_delay_tick = 0;
        Servo::Instance().Enable();
    }
}
