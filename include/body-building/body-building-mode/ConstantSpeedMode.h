#pragma once
#include <base/IExecutable.h>
#include <base/math/InertialElement.h>
#include <base/math/LinearInterpolator.h>
#include <body-building/detector/PullTimesDetector.h>
#include <body-building/io/Cmd.h>
#include <body-building/io/lua_api.h>
#include <body-building/io/Option.h>
#include <body-building/io/Servo.h>
#include <memory>

class ConstantSpeedMode :
    public base::IExecutable
{
private:
#pragma region 选项
    /// @brief 限制转速时让转矩增长的比例。越大转矩随着速度变大增长越快。
    /// @return
    double TorqueGrowRatio();

    /// @brief 转速限制值。超过后转矩会快速增大。
    /// @return
    double SpeedLimit();

    /// @brief 恒速模式的收线转矩。
    /// @return
    int Torque();

    /// @brief 恒速模式下的收线转速。
    /// @return
    double WindingSpeed();

    /// @brief ChX 滤波器的前向通道增益
    /// @return
    int FilterGain();
#pragma endregion

    std::shared_ptr<base::InertialElement> _filter = CreateFilter();
    int _sub_mode = 0;
    std::shared_ptr<Cmd> _cmd;

    base::LinearInterpolator _tension_linear_interpolator{
        base::LinearInterpolator_StartVlaue{0},
        base::LinearInterpolator_EndVlaue{Option::Instance().Tension_kg()},
        base::LinearInterpolator_StepLength{0.03},
    };

public:
    ConstantSpeedMode(int sub_mod, std::shared_ptr<Cmd> cmd)
    {
        _sub_mode = sub_mod;
        _cmd = cmd;
    }

    void Execute();
};
