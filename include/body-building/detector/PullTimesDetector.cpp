#include "PullTimesDetector.h"

void PullTimesDetector::Input(double released_length_of_line)
{
    // 新的输入，要清除上一次输入产生的事件
    _winding_times_changed = false;
    _unwinding_times_changed = false;

    // 检测收线、放线的有效性
    _hys.Input(released_length_of_line);
    if (_hys.OutputChange() == base::HysteresisElement_OutputChange::Rise)
    {
        _has_effective_unwinding = true;
    }
    else if (_hys.OutputChange() == base::HysteresisElement_OutputChange::Fall)
    {
        _has_effective_winding = true;
    }

    // 检测方向变化的瞬间
    _direction_detecter->Input(released_length_of_line);
    if (_direction_detecter->DirectionChange() == base::DirectionDetecter_DirectionChange::FromRisingToFalling)
    {
        // 从放线切换成收线
        if (_has_effective_unwinding)
        {
            _has_effective_unwinding = false;
            _unwinding_times++;
            _unwinding_times_changed = true;
        }
    }
    else if (_direction_detecter->DirectionChange() == base::DirectionDetecter_DirectionChange::FromFallingToRising)
    {
        // 从收线切换成放线
        if (_has_effective_winding)
        {
            _has_effective_winding = false;
            _winding_times++;
            _winding_times_changed = true;
        }
    }
}
