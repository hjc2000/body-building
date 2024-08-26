#include "P_PI_Switch.h"

void P_PI_Switch::Execute()
{
    switch (Option::Instance().BodyBuildingMode())
    {
    case Option_BodyBuildingMode::Standard:
        {
            if (Servo::Instance().FeedbackPosition() < Option::Instance().ZeroPositionProtectionThreshold())
            {
                Servo::Instance().Use_PI_Control();
            }
            else
            {
                Servo::Instance().Use_P_Control();
            }

            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode1:
    case Option_BodyBuildingMode::ConstantSpeedMode2:
    case Option_BodyBuildingMode::ConstantSpeedMode3:
    case Option_BodyBuildingMode::ConstantSpeedMode4:
    case Option_BodyBuildingMode::ConstantSpeedMode5:
        {
            if (Servo::Instance().FeedbackSpeed() < Option::Instance().IntegralSeparationThreshold())
            {
                Servo::Instance().Use_PI_Control();
            }
            else
            {
                Servo::Instance().Use_P_Control();
            }

            break;
        }
    default:
        {
            Servo::Instance().Use_PI_Control();
            break;
        }
    }
}
