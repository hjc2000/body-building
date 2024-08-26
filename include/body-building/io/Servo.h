#pragma once
#include <body-building/io/lua_api.h>
#include <stdint.h>
#include <string>

class Servo
{
private:
    Servo() = default;

    double _last_feedback_position = 0;
    double _feedback_position = 0;

public:
    static Servo &Instance()
    {
        static Servo o;
        return o;
    }

    void Disable();
    void Enable();

    /// @brief 伺服版本。
    /// @return
    int Version();

    /// @brief 刷新伺服的反馈位置信息。
    void Refresh_FeedbackPosition();

    /// @brief 伺服当前的反馈位置。有正负。
    /// @return
    int64_t FeedbackPosition();

    /// @brief 伺服上次的反馈位置。
    /// @return
    int64_t LastFeedbackPosition();

    /// @brief 伺服反馈速度。有正负。
    /// @return
    double FeedbackSpeed();

    /// @brief 逆时针方向是正向
    /// @return
    bool CounterClockwiseIsForward();

    /// @brief 设置电机的转速。这里是速率，没有方向，只能为正数。
    /// @param value
    void SetSpeed(double value);

    /// @brief 正反转矩限制值。
    /// @note 是百分比。范围 [0%, 300%]
    /// @return 返回 0 表示 0%，返回 1 表示 1%。
    int TorqueLimit();

    /// @brief 设置正反转矩限制值。
    /// @param value 是百分比。范围 [0%, 300%]
    void SetTorqueLimit(int value);

    /// @brief 过载预报值。
    /// @return
    int OverloadForecast();

    /// @brief 伺服当前的指令转矩。
    /// @note 是百分比，且有正负。
    /// @return
    int32_t CommandTorque();

    /// @brief 使用 P 控制
    void Use_P_Control();

    /// @brief 使用 PI 控制
    void Use_PI_Control();

    /// @brief 通过位置预置将当前位置设置为零点。
    void SetCurrentPositonAsZeroPoint();
};

class ServoFan
{
private:
    ServoFan() = default;

public:
    static ServoFan &Instance()
    {
        static ServoFan o;
        return o;
    }

    void TurnOn()
    {
        SRV_PARA(2, 76, 0, 0);
    }

    void TurnOff()
    {
        SRV_PARA(2, 76, 1, 0);
    }
};
