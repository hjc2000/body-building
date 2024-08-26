#pragma once
#include <base/IExecutable.h>
#include <base/math/ChXFilter.h>
#include <base/math/InertialElement.h>
#include <base/math/LinearInterpolator.h>
#include <body-building/detector/PullTimesDetector.h>
#include <body-building/io/Cmd.h>
#include <body-building/io/lua_api.h>
#include <memory>

class ICentripetalMode_InfomationGetter
{
public:
    /// @brief 向心比例
    /// @note 大于 1 的值。向心模式下，顺从它向心，拉力会减小。减小拉力是通过将转矩除以本系数。
    /// @return
    virtual double Option_CentripetalRatio() = 0;

    virtual double Option_Tension_kg() = 0;
    virtual double Option_TorqueRatio() = 0;
    virtual double Option_WindingSpeed_rpm() = 0;

    virtual double Servo_FeedbackSpeed() = 0;
};

/// @brief 向心模式
class CentripetalMode :
    public base::IExecutable
{
private:
    std::shared_ptr<Cmd> _cmd;
    std::shared_ptr<ICentripetalMode_InfomationGetter> _infos;
    std::shared_ptr<base::LinearInterpolator> _tension_linear_interpolator;

    std::shared_ptr<base::ChXFilter> _filter{
        new base::ChXFilter{
            base::ChXFilter_KError{10},
            base::ChXFilter_FeedbackDiv{1000},
        },
    };

public:
    CentripetalMode(std::shared_ptr<Cmd> cmd,
                    std::shared_ptr<ICentripetalMode_InfomationGetter> infos);

    void Execute();
};
