#include "Servo.h"

void Servo::Disable()
{
    SRV_PARA(3, 25, 0, 0);
}

void Servo::Enable()
{
    SRV_PARA(3, 25, 1, 0);
}

int Servo::Version()
{
    return SRV_PARA(3, 99);
}

void Servo::Refresh_FeedbackPosition()
{
    _last_feedback_position = _feedback_position;
    _feedback_position = SRV_MON(6);
}

int64_t Servo::FeedbackPosition()
{
    return _feedback_position;
}

int64_t Servo::LastFeedbackPosition()
{
    return _last_feedback_position;
}

double Servo::FeedbackSpeed()
{
    if (CounterClockwiseIsForward())
    {
        return SRV_MON(33);
    }

    return -SRV_MON(33);
}

bool Servo::CounterClockwiseIsForward()
{
    return SRV_PARA(1, 4) == 0;
}

void Servo::SetSpeed(double value)
{
    AXIS_SPEED(value);
}

int Servo::TorqueLimit()
{
    // SRV_PARA(1, 27)
    // SRV_PARA(1, 28)
    return SRV_PARA(1, 27);
}

void Servo::SetTorqueLimit(int value)
{
    SRV_PARA(1, 27, value, 0);
    SRV_PARA(1, 28, value, 0);
}

int Servo::OverloadForecast()
{
    return SRV_MON(18);
}

int32_t Servo::CommandTorque()
{
    return SRV_MON(2);
}

void Servo::Use_P_Control()
{
    SRV_EI(10, 1);
}

void Servo::Use_PI_Control()
{
    SRV_EI(10, 0);
}

void Servo::SetCurrentPositonAsZeroPoint()
{
    SRV_EI(9, 1);
    DELAY(10);
    SRV_EI(9, 0);
}
