#include "AssistanceMode.h"

void AssistanceMode::Prepare()
{
    _tension_linear_interpolator->SetEndValue(_current_tension);
    if (_pull_times_detecter->UnwindingTimesChanged())
    {
        // 一次出绳已经完成，此时已经处在回绳的方向
        if (_pull_times_detecter->UnwindingTimes() == 2)
        {
            _reference_time = _unwinding_tick;
            _end_point_line_length = _pull_times_detecter->TurningPoint();
        }
        else if (_pull_times_detecter->UnwindingTimes() == 3)
        {
            _reference_time += _unwinding_tick;
            _reference_time = _reference_time / 2 + 500;

            _end_point_line_length += _pull_times_detecter->TurningPoint();
            _end_point_line_length = _end_point_line_length / 2 * 0.9;

            _pull_times_detecter->SetEffectiveUnwindingThreshold(_end_point_line_length);
            _pull_times_detecter->SetEffectiveWindingThreshold(_starting_point_line_length);
            _is_preparing = false;
        }

        _unwinding_tick = 0;
    }
    else if (_pull_times_detecter->WindingTimesChanged())
    {
        // 一次回绳已经完成，此时已经处在出绳的方向
        _starting_point_line_length = _pull_times_detecter->TurningPoint();
    }
}

void AssistanceMode::Work()
{
    if (_pull_times_detecter->UnwindingTimesChanged())
    {
        // 一次出绳已经完成，此时已经处在回绳的方向
        _tension_linear_interpolator->SetEndValue(_current_tension);
        _unwinding_tick = 0;
        _is_pulling_out = false;
    }
    else if (_pull_times_detecter->WindingTimesChanged())
    {
        // 一次回绳已经完成，此时已经处在出绳的方向
        _is_pulling_out = true;
    }

    if (_is_pulling_out)
    {
        double reduced_tension = 0;
        if (_unwinding_tick > _reference_time)
        {
            /* 出绳时间太长了，需要助力。
             * 需要每 3 秒减小 20% 的拉力。
             * 因为程序每 2ms 执行一次，所以 3s 对应 1500 次 tick。
             * 每多一次 tick，就减去 (1 / 1500 * _current_tension * 0.2) 的拉力。
             */
            reduced_tension = static_cast<double>(_unwinding_tick - _reference_time) / 1500 * _current_tension * 0.2;
        }

        double output_tension = _current_tension - reduced_tension;
        if (output_tension < 1)
        {
            output_tension = 1;
        }

        // 加 0.5 是为了四舍五入
        DD(14, static_cast<int>(output_tension * 10 + 0.5));
        _tension_linear_interpolator->SetEndValue(output_tension);
    }
}

AssistanceMode::AssistanceMode(std::shared_ptr<Cmd> cmd,
                               std::shared_ptr<IAssistanceMode_InfomationGetter> infos)
{
    _cmd = cmd;
    _infos = infos;

    _current_tension = _infos->Option_Tension_kg();

    _tension_linear_interpolator = std::shared_ptr<base::LinearInterpolator>{
        new base::LinearInterpolator{
            base::LinearInterpolator_StartVlaue{0},
            base::LinearInterpolator_EndVlaue{_current_tension},
            base::LinearInterpolator_StepLength{0.03},
        },
    };
}

void AssistanceMode::Execute()
{
    _cmd->SetSpeed(_infos->Option_WindingSpeed_rpm());
    _current_tension = _infos->Option_Tension_kg();
    if (_infos->Servo_FeedbackSpeed() > 10)
    {
        _unwinding_tick++;
    }

    _pull_times_detecter->Input(_infos->RleasedLengthOfLine());
    if (_is_preparing)
    {
        Prepare();
    }
    else
    {
        Work();
    }

    double tension = ++(*_tension_linear_interpolator);
    double torque = tension * _infos->Option_TorqueRatio();
    _cmd->SetTorque(torque);
}
