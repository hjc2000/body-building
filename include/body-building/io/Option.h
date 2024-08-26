#pragma once
#include <body-building/io/lua_api.h>
#include <chrono>
#include <cmath>
#include <stdint.h>

/// @brief 健身模式
enum class Option_BodyBuildingMode
{
    /// @brief 标准模式
    Standard = 0,

    /// @brief 智能模式
    IntelligentMode = 1,

    /// @brief 向心模式
    CentripetalMode = 2,

    /// @brief 离心模式
    CentrifugalMode = 3,

    /// @brief 弹簧模式
    SpringMode = 4,

    /// @brief 待机模式
    Standby = 5,

    ConstantSpeedMode1 = 6,
    ConstantSpeedMode2 = 7,
    ConstantSpeedMode3 = 8,
    ConstantSpeedMode4 = 9,
    ConstantSpeedMode5 = 10,

    /// @brief 燃尽模式
    BurnOutMode = 11,

    /// @brief 助力模式
    AssistanceMode = 12,
};

enum class Option_AdditionalMode
{
    None,
};

class Option
{
private:
    Option() = default;

    Option_BodyBuildingMode _body_building_mode;
    Option_BodyBuildingMode _last_body_building_mode;

    Option_AdditionalMode _additional_mode;
    Option_AdditionalMode _last_additional_mode;

    double _tension_kg = 0;
    double _last_tension_kg = 0;

public:
    static Option &Instance()
    {
        static Option o;
        return o;
    }

    void Refresh_BodyBuildingMode();
    Option_BodyBuildingMode BodyBuildingMode();
    Option_BodyBuildingMode LastBodyBuildingMode();
    bool BodyBuildingModeChanged();

    void Clear_AdditionalMode();
    void Refresh_AdditionalMode();
    Option_AdditionalMode AdditionalMode();
    Option_AdditionalMode LastAdditionalMode();
    bool AdditionalModeCodeChanged();

    void Refresh_Tension_kg();
    double Tension_kg();
    double LastTension_kg();
    bool Tension_kg_Changed();

    /// @brief 允许的最大拉力。
    /// @return
    double MaxTension_kg();

    /// @brief 允许的最小转矩
    /// @return
    int MinTorque();

    /// @brief 允许的最大转矩
    /// @return
    int MaxTorque();

    /// @brief 收线速度。
    /// @return
    double WindingSpeed();

    /// @brief 转矩限制值 / 拉力_kg
    /// @note 将 拉力_kg 乘上本属性，就可得到转矩限制值。转矩限制值是个百分比。
    /// @return
    double TorqueRatio();

    /// @brief 使用引力包。
    /// @return
    bool UseGravitationPackage();

    /// @brief 零点保护位置阈值。当伺服反馈位置的绝对值小于此值时需要保护措施。
    /// @return
    int ZeroPositionProtectionThreshold();

    /// @brief 最大转矩的更新频率。
    /// @return
    double UpdateFrequencyOfMaxTorque_Hz();

    /// @brief 阻尼阈值速度。超过后会遇到阻尼，速度越大阻力越大。
    /// @return
    double OverspeedDampingCoefficientThreshold();

    /// @brief 超过阈值速度后的阻尼系数。
    /// @return
    double DampingCoefficientWhenOverspeeding();

    int IntelligentModeTensionCompensation();

    /// @brief 大回绳阻力下的回绳速度限制
    /// @return
    double WindingSpeedLimitDuringBigWindingResistance();

    /// @brief 回到零点后自动禁用伺服。
    /// @return
    bool AutoDisableServoAfterReturningToZero();

    /// @brief 零点校准模式下的转矩。
    /// @return
    int ReturnToZeroTorque();

    /// @brief 零点校准模式下的转速。
    /// @return
    double ReturnToZeroSpeed();

    /// @brief 正转时应该减小的转矩值。正转时应该将转矩减去本函数的返回值，反转时不用操作。
    /// @return
    int ReduceTorque();

    /// @brief 什么样的转矩算作大转矩。
    /// @return
    int BigTorque();

    /// @brief 积分分离阈值
    /// @return
    double IntegralSeparationThreshold()
    {
        return 0;
    }
};
