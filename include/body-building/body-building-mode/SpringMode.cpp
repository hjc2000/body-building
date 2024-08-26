#include "SpringMode.h"
#include <format>

SpringMode::SpringMode(std::shared_ptr<Cmd> cmd,
                       std::shared_ptr<ISpringMode_InfomationGetter> infos)
{
    _cmd = cmd;
    _infos = infos;

    _current_tension_kg = _infos->Option_Tension_kg();

    _tension_linear_interpolator = std::shared_ptr<base::LinearInterpolator>{
        new base::LinearInterpolator{
            base::LinearInterpolator_StartVlaue{0},
            base::LinearInterpolator_EndVlaue{_current_tension_kg},
            base::LinearInterpolator_StepLength{0.03},
        },
    };
}

void SpringMode::Execute()
{
    _cmd->SetSpeed(_infos->Option_WindingSpeed_rpm());
    _current_tension_kg = _infos->Option_Tension_kg();
    int feedback_position = _infos->Servo_FeedbackPosition();
    int one_meter_position = _infos->Option_OneMeterPosition();
    double torque_ratio = _infos->Option_TorqueRatio();
    double spring_ratio = _infos->Option_SpringRatio();

    double tension = ++(*_tension_linear_interpolator);
    if (tension < 4)
    {
        tension = 4;
    }

    double torque = tension * torque_ratio;

    if (feedback_position < one_meter_position)
    {
        torque = torque * feedback_position / one_meter_position;
        std::cout << std::format("1m 内，当前位置：{}", feedback_position) << std::endl;
    }
    else
    {
        int delta_position = feedback_position - one_meter_position;

        // 每转一圈转矩增大 spring_ratio
        torque = torque + (delta_position * spring_ratio / 30000);
        std::cout << std::format("1m 外，当前位置：{}", feedback_position) << std::endl;
    }

    torque = _filter->Input(torque);
    if (torque < 5)
    {
        torque = 5;
    }

    _cmd->SetTorque(torque);
}

void Test_SpringMode()
{
    class Getter :
        public ISpringMode_InfomationGetter
    {
    public:
        virtual double Option_Tension_kg() override
        {
            return 10;
        }

        virtual double Option_TorqueRatio() override
        {
            return 1.85;
        }

        virtual double Option_WindingSpeed_rpm() override
        {
            return 50;
        }

        virtual int Option_OneMeterPosition() override
        {
            return 30000;
        }

        /// @brief 弹簧劲度系数。每转一圈转矩要增大的百分比。
        /// @return
        virtual double Option_SpringRatio() override
        {
            return 10;
        }

        int _feedback_position = 0;
        bool _is_rising = true;

        virtual int Servo_FeedbackPosition() override
        {
            if (_feedback_position >= 1000 * 100)
            {
                _is_rising = false;
            }
            else if (_feedback_position < 1000)
            {
                _is_rising = true;
            }

            if (_is_rising)
            {
                _feedback_position += 1000;
            }
            else
            {
                _feedback_position -= 1000;
            }

            return _feedback_position;
        }
    };

    std::shared_ptr<Cmd> cmd{new Cmd{}};
    std::shared_ptr<Getter> getter{new Getter{}};
    SpringMode mode{cmd, getter};
    for (int i = 0; i < 500; i++)
    {
        mode.Execute();
        std::cout << (*cmd) << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}
