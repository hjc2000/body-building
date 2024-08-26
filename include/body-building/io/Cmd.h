#pragma once
#include <iostream>

/// @brief 最终会发送给伺服的指令。
class Cmd
{
private:
    double _torque = 0;
    double _speed = 0;

public:
    double Torque() const;
    void SetTorque(double value);

    double Speed() const;
    void SetSpeed(double value);

    /// @brief 将命令送给伺服。
    void SendToServo();
};

std::ostream &operator<<(std::ostream &os, Cmd const &cmd);
