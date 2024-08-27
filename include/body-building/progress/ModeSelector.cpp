#include "ModeSelector.h"
#include <body-building/body-building-mode/AssistanceMode.h>
#include <body-building/body-building-mode/BurnOutMode.h>
#include <body-building/body-building-mode/CentrifugalMode.h>
#include <body-building/body-building-mode/CentripetalMode.h>
#include <body-building/body-building-mode/ConstantSpeedMode.h>
#include <body-building/body-building-mode/IntelligentMode.h>
#include <body-building/body-building-mode/SpringMode.h>
#include <body-building/body-building-mode/StandardMode.h>
#include <body-building/io/Cmd.h>
#include <body-building/io/Option.h>
#include <body-building/io/Servo.h>
#include <body-building/other-mode/AdditionMode.h>
#include <body-building/other-mode/CalibrateZeroPointMode.h>
#include <body-building/other-mode/StallProtectionMode.h>

void ModeSelector::HandleAfterBodyBuildingMode()
{
}

void ModeSelector::CreateBodyBuildingModeExecutable()
{
    switch (Option::Instance().BodyBuildingMode())
    {
    case Option_BodyBuildingMode::Standard:
        {
            class InfoGetter :
                public body::IStandardMode_InformationGetter
            {
            public:
                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new body::StandardMode{
                    _cmd,
                    std::shared_ptr<InfoGetter>{new InfoGetter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::IntelligentMode:
        {
            class Getter :
                public IIntelligentMode_InfomationGetter
            {
            public:
                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_WindingTorque() override
                {
                    return SRV_PARA(1, 34);
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                double Option_k() override
                {
                    return SRV_PARA(1, 45) / 100.0;
                }

                double Option_b() override
                {
                    return SRV_PARA(1, 46) / 100.0;
                }

                double Option_MaxTorque() override
                {
                    return Option::Instance().MaxTorque();
                }

                double Servo_FeedbackSpeed() override
                {
                    return Servo::Instance().FeedbackSpeed();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new IntelligentMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::CentripetalMode:
        {
            class Getter :
                public ICentripetalMode_InfomationGetter
            {
            public:
                /// @brief 向心比例
                /// @note 大于 1 的值。向心模式下，顺从它向心，拉力会减小。减小拉力是通过将转矩除以本系数。
                /// @return
                double Option_CentripetalRatio() override
                {
                    return SRV_PARA(1, 43);
                }

                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                double Servo_FeedbackSpeed() override
                {
                    return Servo::Instance().FeedbackSpeed();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new CentripetalMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::CentrifugalMode:
        {
            class Getter :
                public ICentrifugalMode_InfomationGetter
            {
            public:
                /// @brief 离心比例
                /// @note 大于 1 的值。离心模式下，顺从它离心，拉力会减小。减小拉力是通过将转矩除以本系数。
                /// @return
                double Option_CentrifugalRatio() override
                {
                    return SRV_PARA(1, 44);
                }

                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                double Servo_FeedbackSpeed() override
                {
                    return Servo::Instance().FeedbackSpeed();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new CentrifugalMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::SpringMode:
        {
            class Getter : public ISpringMode_InfomationGetter
            {
            public:
                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return 50;
                }

                int Option_OneMeterPosition() override
                {
                    return 1000;
                }

                int Servo_FeedbackPosition() override
                {
                    return 100;
                }

                /// @brief 弹簧劲度系数。
                /// @return
                virtual double Option_SpringRatio() override
                {
                    return 1.5;
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new SpringMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter},
                },
            };

            break;
        }
    default:
    case Option_BodyBuildingMode::Standby:
        {
            _body_building_executable = nullptr;
            _cmd->SetSpeed(0);
            _cmd->SetTorque(0);
            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode1:
        {
            class Getter :
                public IConstantSpeedMode_InfomationGetter
            {
            public:
                /// @brief 限制转速时让转矩增长的比例。越大转矩随着速度变大增长越快。
                /// @return
                double Option_TorqueGrowRatio() override
                {
                    return SRV_PARA(1, 39) / 1000;
                }

                /// @brief 转速限制值。超过后转矩会快速增大。
                /// @return
                double Option_SpeedLimit() override
                {
                    return SRV_PARA(1, 21) / 100;
                }

                /// @brief 恒速模式的收线转矩。
                /// @return
                int Option_WindingTorque() override
                {
                    return SRV_PARA(2, 18);
                }

                /// @brief 恒速模式下的收线转速。
                /// @return
                double Option_WindingSpeed_rpm() override
                {
                    return SRV_PARA(1, 10) / 100;
                }

                /// @brief ChX 滤波器的前向通道增益
                /// @return
                double Option_ChXFilterKError() override
                {
                    return SRV_PARA(1, 20);
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new ConstantSpeedMode{
                    0,
                    _cmd,
                    std::shared_ptr<Getter>{new Getter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode2:
        {
            class Getter :
                public IConstantSpeedMode_InfomationGetter
            {
            public:
                /// @brief 限制转速时让转矩增长的比例。越大转矩随着速度变大增长越快。
                /// @return
                double Option_TorqueGrowRatio() override
                {
                    return 1;
                }

                /// @brief 转速限制值。超过后转矩会快速增大。
                /// @return
                double Option_SpeedLimit() override
                {
                    return 1400;
                }

                /// @brief 恒速模式的收线转矩。
                /// @return
                int Option_WindingTorque() override
                {
                    return 13;
                }

                /// @brief 恒速模式下的收线转速。
                /// @return
                double Option_WindingSpeed_rpm() override
                {
                    return 600;
                }

                /// @brief ChX 滤波器的前向通道增益
                /// @return
                double Option_ChXFilterKError() override
                {
                    return 200;
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new ConstantSpeedMode{
                    1,
                    _cmd,
                    std::shared_ptr<Getter>{new Getter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode3:
        {
            class Getter :
                public IConstantSpeedMode_InfomationGetter
            {
            public:
                /// @brief 限制转速时让转矩增长的比例。越大转矩随着速度变大增长越快。
                /// @return
                double Option_TorqueGrowRatio() override
                {
                    return 1.45;
                }

                /// @brief 转速限制值。超过后转矩会快速增大。
                /// @return
                double Option_SpeedLimit() override
                {
                    return 1100;
                }

                /// @brief 恒速模式的收线转矩。
                /// @return
                int Option_WindingTorque() override
                {
                    return 30;
                }

                /// @brief 恒速模式下的收线转速。
                /// @return
                double Option_WindingSpeed_rpm() override
                {
                    return 400;
                }

                /// @brief ChX 滤波器的前向通道增益
                /// @return
                double Option_ChXFilterKError() override
                {
                    return 200;
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new ConstantSpeedMode{
                    2,
                    _cmd,
                    std::shared_ptr<Getter>{new Getter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode4:
        {
            class Getter :
                public IConstantSpeedMode_InfomationGetter
            {
            public:
                /// @brief 限制转速时让转矩增长的比例。越大转矩随着速度变大增长越快。
                /// @return
                double Option_TorqueGrowRatio() override
                {
                    return 2;
                }

                /// @brief 转速限制值。超过后转矩会快速增大。
                /// @return
                double Option_SpeedLimit() override
                {
                    return 1;
                }

                /// @brief 恒速模式的收线转矩。
                /// @return
                int Option_WindingTorque() override
                {
                    return 45;
                }

                /// @brief 恒速模式下的收线转速。
                /// @return
                double Option_WindingSpeed_rpm() override
                {
                    return 300;
                }

                /// @brief ChX 滤波器的前向通道增益
                /// @return
                double Option_ChXFilterKError() override
                {
                    return 1000;
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new ConstantSpeedMode{
                    3,
                    _cmd,
                    std::shared_ptr<Getter>{new Getter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::ConstantSpeedMode5:
        {
            class Getter :
                public IConstantSpeedMode_InfomationGetter
            {
            public:
                /// @brief 限制转速时让转矩增长的比例。越大转矩随着速度变大增长越快。
                /// @return
                double Option_TorqueGrowRatio() override
                {
                    return 0.6;
                }

                /// @brief 转速限制值。超过后转矩会快速增大。
                /// @return
                double Option_SpeedLimit() override
                {
                    return 1800;
                }

                /// @brief 恒速模式的收线转矩。
                /// @return
                int Option_WindingTorque() override
                {
                    return 6;
                }

                /// @brief 恒速模式下的收线转速。
                /// @return
                double Option_WindingSpeed_rpm() override
                {
                    return 800;
                }

                /// @brief ChX 滤波器的前向通道增益
                /// @return
                double Option_ChXFilterKError() override
                {
                    return 500;
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new ConstantSpeedMode{
                    4,
                    _cmd,
                    std::shared_ptr<Getter>{new Getter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::BurnOutMode:
        {
            class Getter :
                public body::IBurnOutMode_InfomationGetter
            {
            public:
                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_MaxTension_kg() override
                {
                    return Option::Instance().MaxTension_kg();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                double Servo_FeedbackSpeed() override
                {
                    return Servo::Instance().FeedbackSpeed();
                }

                int Servo_FeedbackPosition() override
                {
                    return Servo::Instance().FeedbackPosition();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new body::BurnOutMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter{}},
                },
            };

            break;
        }
    case Option_BodyBuildingMode::AssistanceMode:
        {
            class Getter :
                public IAssistanceMode_InfomationGetter
            {
            public:
                double Option_Tension_kg() override
                {
                    return Option::Instance().Tension_kg();
                }

                double Option_TorqueRatio() override
                {
                    return Option::Instance().TorqueRatio();
                }

                double Option_WindingSpeed_rpm() override
                {
                    return Option::Instance().WindingSpeed();
                }

                int Servo_FeedbackPosition() override
                {
                    return Servo::Instance().FeedbackPosition();
                }

                double Servo_FeedbackSpeed() override
                {
                    return Servo::Instance().FeedbackSpeed();
                }
            };

            _body_building_executable = std::shared_ptr<base::IExecutable>{
                new AssistanceMode{
                    _cmd,
                    std::shared_ptr<Getter>{new Getter},
                },
            };

            break;
        }
    }
}

void ModeSelector::Execute()
{
    if (!CalibrateZeroPointMode::Instance().IsCompleted())
    {
        CalibrateZeroPointMode::Instance().Execute();
        return;
    }

    if (Option::Instance().AdditionalModeCodeChanged())
    {
        AdditionMode::Instance().SetMode(static_cast<AdditionMode_ModeEnum>(Option::Instance().AdditionalMode()));
    }

    if (static_cast<int>(AdditionMode::Instance().Mode()) > 0)
    {
        AdditionMode::Instance().Execute();
        return;
    }

    // 失速保护模式
    if (StallProtectionMode::Instance().StallFlag())
    {
        StallProtectionMode::Instance().Execute();
        return;
    }

    // 健身模式
    if (Option::Instance().BodyBuildingModeChanged() || Option::Instance().Tension_kg_Changed())
    {
        CreateBodyBuildingModeExecutable();
    }

    if (_body_building_executable)
    {
        _body_building_executable->Execute();
        HandleAfterBodyBuildingMode();
    }
}
