#pragma once
#include <base/IExecutable.h>
#include <base/math/ChXFilter.h>
#include <base/math/InertialElement.h>
#include <base/math/LinearInterpolator.h>
#include <body-building/detector/PullTimesDetector.h>
#include <body-building/io/Cmd.h>
#include <body-building/io/lua_api.h>
#include <body-building/io/Option.h>
#include <body-building/io/Servo.h>
#include <memory>

class IConstantSpeedMode_InfomationGetter
{
public:
    /// @brief 限制转速时让转矩增长的比例。越大转矩随着速度变大增长越快。
    /// @return
    virtual double Option_TorqueGrowRatio() = 0;

    /// @brief 转速限制值。超过后转矩会快速增大。
    /// @return
    virtual double Option_SpeedLimit() = 0;

    /// @brief 恒速模式的收线转矩。
    /// @return
    virtual int Option_WindingTorque() = 0;

    /// @brief 恒速模式下的收线转速。
    /// @return
    virtual double Option_WindingSpeed_rpm() = 0;

    /// @brief ChX 滤波器的前向通道增益
    /// @return
    virtual double Option_ChXFilterKError() = 0;
};

class ConstantSpeedMode :
    public base::IExecutable
{
private:
    std::shared_ptr<base::ChXFilter> _filter;
    int _sub_mode = 0;
    std::shared_ptr<Cmd> _cmd;
    std::shared_ptr<IConstantSpeedMode_InfomationGetter> _infos;

    base::LinearInterpolator _tension_linear_interpolator{
        base::LinearInterpolator_StartVlaue{0},
        base::LinearInterpolator_EndVlaue{Option::Instance().Tension_kg()},
        base::LinearInterpolator_StepLength{0.03},
    };

public:
    ConstantSpeedMode(int sub_mod,
                      std::shared_ptr<Cmd> cmd,
                      std::shared_ptr<IConstantSpeedMode_InfomationGetter> infos)
    {
        _sub_mode = sub_mod;
        _cmd = cmd;
        _infos = infos;
    }

    void Execute();
};
