#pragma once
#include <base/IExecutable.h>
#include <base/math/ChXFilter.h>
#include <base/math/InertialElement.h>
#include <base/math/LinearInterpolator.h>
#include <body-building/detector/PullTimesDetector.h>
#include <body-building/io/Cmd.h>
#include <body-building/io/lua_api.h>
#include <memory>

class ISpringMode_InfomationGetter
{
public:
    virtual double Option_Tension_kg() = 0;
    virtual double Option_TorqueRatio() = 0;
    virtual double Option_WindingSpeed_rpm() = 0;
    virtual int Option_OneMeterPosition() = 0;

    /// @brief 弹簧劲度系数。每转一圈转矩要增大的百分比。
    /// @return
    virtual double Option_SpringRatio() = 0;

    virtual int Servo_FeedbackPosition() = 0;
};

/// @brief 弹簧模式
class SpringMode :
    public base::IExecutable
{
private:
    std::shared_ptr<Cmd> _cmd;
    std::shared_ptr<ISpringMode_InfomationGetter> _infos;
    std::shared_ptr<base::LinearInterpolator> _tension_linear_interpolator;
    double _current_tension_kg = 0;

    std::shared_ptr<base::ChXFilter> _filter{
        new base::ChXFilter{
            base::ChXFilter_KError{10},
            base::ChXFilter_FeedbackDiv{40},
        },
    };

public:
    SpringMode(std::shared_ptr<Cmd> cmd,
               std::shared_ptr<ISpringMode_InfomationGetter> infos);

    void Execute() override;
};

void Test_SpringMode();
