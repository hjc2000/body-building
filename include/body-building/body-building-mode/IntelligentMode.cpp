#include "IntelligentMode.h"

double IntelligentMode::CalculateTorque(double feedback_speed)
{
    if (feedback_speed > 3500)
    {
        feedback_speed = 3500;
    }

    double torque = (1 - std::cos(feedback_speed / 3500 * 3.14159)) / 2 * _infos->Option_k() + _infos->Option_b();
    return torque;
}

void IntelligentMode::Execute()
{
    double feedback_speed = _infos->Servo_FeedbackSpeed();
    double winding_speed = _infos->Option_WindingSpeed_rpm();
    double winding_torque = _infos->Option_WindingTorque();
    double max_torque = _infos->Option_MaxTorque();

    if (feedback_speed > 10)
    {
        // 当前是出绳
        double torque = CalculateTorque(feedback_speed);
        if (torque > _filter->Feedback())
        {
            _filter->ChangeParameter(base::ChXFilter_KError{500},
                                     base::ChXFilter_FeedbackDiv{10000});
        }
        else
        {
            _filter->ChangeParameter(base::ChXFilter_KError{5},
                                     base::ChXFilter_FeedbackDiv{10000});
        }

        _filter->Input(torque);
        _cmd->SetSpeed(0);
    }
    else
    {
        // 当前是收绳
        _filter->SetFeedback(winding_torque);
        _cmd->SetSpeed(winding_speed);
    }

    double torque = _filter->Feedback();
    if (torque < 10)
    {
        torque = 10;
    }
    else if (torque > max_torque)
    {
        torque = max_torque;
    }

    _cmd->SetTorque(torque);
}
