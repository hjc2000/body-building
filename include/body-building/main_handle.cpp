#include "main_handle.h"
#include <body-building/detector/PullTimesDetector.h>
#include <body-building/io/Cmd.h>
#include <body-building/io/Option.h>
#include <body-building/io/Servo.h>
#include <body-building/other-mode/AdditionMode.h>
#include <body-building/other-mode/CalibrateZeroPointMode.h>
#include <body-building/progress/ModeSelector.h>
#include <body-building/progress/SleepControler.h>

/// @brief 更新外界输入信息和基于它们计算出来的信息。
/// @note 顺序很重要。信息之间有依赖关系。
void Refresh()
{
    Servo::Instance().Refresh_FeedbackPosition();
    Option::Instance().Refresh_AdditionalMode();
    Option::Instance().Refresh_BodyBuildingMode();
    Option::Instance().Refresh_Tension_kg();
}

void Initialize()
{
    Option::Instance().Clear_AdditionalMode();
    Servo::Instance().Use_PI_Control();
    ServoFan::Instance().TurnOff();
}

void RecodeStatus()
{
}

void main_handle()
{
    Initialize();

    CalibrateZeroPointMode::Instance()._on_completed = [&]()
    {
        if (Option::Instance().AutoDisableServoAfterReturningToZero())
        {
            AdditionMode::Instance().SetMode(AdditionMode_ModeEnum::GraduallyDisableServo);
        }
    };

    std::shared_ptr<Cmd> cmd{new Cmd{}};
    ModeSelector mod_selector{cmd};

    while (true)
    {
        if (true) // 假设已经检查定时时间到了
        {
            // 重置定时器
            // ResetTimer()

            Refresh();
            mod_selector.Execute();
            SleepControler::Instance().Execute();
            RecodeStatus();
            cmd->SendToServo();
        }
    }
}
