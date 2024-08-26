#pragma once
#include <base/IExecutable.h>
#include <base/math/DirectionDetecter.h>
#include <base/math/InertialElement.h>
#include <base/math/LinearInterpolator.h>
#include <body-building/detector/PullTimesDetector.h>
#include <body-building/io/Cmd.h>
#include <body-building/io/lua_api.h>
#include <memory>

class IBurnOutMode_InfomationGetter
{
public:
    virtual double Option_Tension_kg() = 0;
    virtual double Option_MaxTension_kg() = 0;
    virtual double Option_TorqueRatio() = 0;
    virtual double Option_WindingSpeed_rpm() = 0;

    virtual double Servo_FeedbackSpeed() = 0;
    virtual int Servo_FeedbackPosition() = 0;

    double RleasedLengthOfLine()
    {
        return Servo_FeedbackPosition() * 3.7 * 2 * 3.14 / 12000;
    }
};

class BurnOutMode :
    public base::IExecutable
{
private:
    bool _is_preparing = true;
    int _unwinding_tick = 0;
    double _reference_power = 0;
    double _tension = 0;

    double _starting_point_line_length = 0;
    double _end_point_line_length = 0;
    double _reference_line_length = 0;

    std::shared_ptr<Cmd> _cmd;
    std::shared_ptr<IBurnOutMode_InfomationGetter> _infos;
    std::shared_ptr<PullTimesDetector> _pull_times_detecter{new PullTimesDetector{}};
    std::shared_ptr<base::LinearInterpolator> _tension_linear_interpolator;

    void Prepare();
    void Work();

public:
    BurnOutMode(std::shared_ptr<Cmd> cmd,
                std::shared_ptr<IBurnOutMode_InfomationGetter> infos);

    void Execute();
};
