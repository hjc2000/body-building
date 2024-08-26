#include "StandardMode.h"

StandardMode::StandardMode(std::shared_ptr<Cmd> cmd,
                           std::shared_ptr<IStandardMode_InformationGetter> infos)
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

void StandardMode::Execute()
{
    double winding_speed = _infos->Option_WindingSpeed_rpm();
    _current_tension_kg = _infos->Option_Tension_kg();
    double torque_ratio = _infos->Option_TorqueRatio();

    _cmd->SetSpeed(winding_speed);

    double tension = ++(*_tension_linear_interpolator);
    if (tension < 4)
    {
        tension = 4;
    }

    double torque = tension * torque_ratio;
    _cmd->SetTorque(torque);
}

void Test_StandardMode()
{
    class Getter :
        public IStandardMode_InformationGetter
    {
    public:
        virtual double Option_Tension_kg() override
        {
            return 5;
        }

        virtual double Option_WindingSpeed_rpm() override
        {
            return 20;
        }

        virtual double Option_TorqueRatio() override
        {
            return 1.85;
        }
    };

    std::shared_ptr<Cmd> cmd{new Cmd{}};
    std::shared_ptr<Getter> getter{new Getter{}};
    StandardMode mode{cmd, getter};
    for (int i = 0; i < 200; i++)
    {
        mode.Execute();
        std::cout << (*cmd) << std::endl;
        std::cout << "-------------------" << std::endl;
    }
}
