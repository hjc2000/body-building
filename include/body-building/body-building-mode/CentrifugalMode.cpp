#include "CentrifugalMode.h"

CentrifugalMode::CentrifugalMode(std::shared_ptr<Cmd> cmd,
                                 std::shared_ptr<ICentrifugalMode_InfomationGetter> infos)
{
    _cmd = cmd;
    _infos = infos;

    _tension_linear_interpolator = std::shared_ptr<base::LinearInterpolator>{
        new base::LinearInterpolator{
            base::LinearInterpolator_StartVlaue{0},
            base::LinearInterpolator_EndVlaue{_infos->Option_Tension_kg()},
            base::LinearInterpolator_StepLength{0.03},
        },
    };
}

void CentrifugalMode::Execute()
{
    _cmd->SetSpeed(_infos->Option_WindingSpeed_rpm());
    double tension = ++(*_tension_linear_interpolator);
    double torque = tension * _infos->Option_TorqueRatio();

    if (_infos->Servo_FeedbackSpeed() > -50)
    {
        torque = torque / _infos->Option_CentrifugalRatio();
    }

    torque = _filter->Input(torque);
    _cmd->SetTorque(torque);
}
