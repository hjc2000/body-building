#include "AdditionMode.h"
#include <body-building/other-mode/CalibrateZeroPointMode.h>
#include <body-building/progress/GraduallyDisableEnableServo.h>

void AdditionMode::Execute()
{
    switch (_mode)
    {
    case AdditionMode_ModeEnum::GraduallyDisableServo:
        {
            GraduallyDisableEnableServo::Instance().GraduallyDisableServo();
            break;
        }
    case AdditionMode_ModeEnum::GraduallyEnableServo:
        {
            GraduallyDisableEnableServo::Instance().GraduallyEnableServo();
            break;
        }
    case AdditionMode_ModeEnum::StallProtectMode:
        {
            _mode = AdditionMode_ModeEnum::None;
            break;
        }
    case AdditionMode_ModeEnum::ResetCalibrateZeroPointModeCompletionStatus:
        {
            CalibrateZeroPointMode::Instance().ResetCompletionStatus();
            _mode = AdditionMode_ModeEnum::None;
            break;
        }
    default:
    case AdditionMode_ModeEnum::None:
        {
            break;
        }
    }
}
