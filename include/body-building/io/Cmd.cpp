#include "Cmd.h"
#include <body-building/io/Servo.h>
#include <math.h>

double Cmd::Torque() const
{
    return _torque;
}

void Cmd::SetTorque(double value)
{
    _torque = std::abs(value);
}

double Cmd::Speed() const
{
    return _speed;
}

void Cmd::SetSpeed(double value)
{
    _speed = std::abs(value);
}

void Cmd::SendToServo()
{
    Servo::Instance().SetTorqueLimit(Torque());
    Servo::Instance().SetSpeed(Speed());
}

std::ostream &operator<<(std::ostream &os, Cmd const &cmd)
{
    os << "速度：" << cmd.Speed() << "  "
       << "转矩：" << cmd.Torque() << "  ";

    return os;
}
