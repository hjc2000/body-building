#pragma once
#include <base/math/InertialElement.h>

/// @brief 摩擦转矩计算器
class FrictionTorqueCalculater
{
private:
    int _current_friction_torque = 0;
    int _last_friction_torque = 0;

    base::InertialElement _filter{
        base::InertialElement_TimeConstant{0.025},
        base::InertialElement_SampleInterval{0.002},
    };

    FrictionTorqueCalculater() = default;

    /// @brief 收线时的摩擦转矩。
    /// @return
    int WindingFrictionTorque() const;

    /// @brief 放线时的摩擦转矩。
    /// @param tension_kg
    /// @return
    int UnwindingFrictionTorque(double tension_kg) const;

public:
    static FrictionTorqueCalculater &Instance()
    {
        static FrictionTorqueCalculater o;
        return o;
    }

    /// @brief 计算摩擦转矩
    ///
    /// @note 设 Tm 为电机输出转矩，T 为用户拉力换算到电机上的转矩，Tf 为摩擦转矩。
    /// 里的平衡方程为：
    /// 	Tm + T + Tf = 0
    /// 其中
    /// 	Tm = -转矩限制值
    /// 则
    /// 	-转矩限制值 + T + Tf = 0
    /// 	转矩限制值 = T + Tf
    /// T 就是程序流程中进行摩擦补偿之前的命令转矩。所以将命令转矩加上摩擦转矩就能进行摩擦补偿了。
    ///
    /// @param tension_kg 希望输出的拉力大小
    /// @return 摩擦转矩。是个矢量。
    int Calculate(double tension_kg);

    int CurrentFrictionTorque() const
    {
        return _current_friction_torque;
    }

    int LastFrictionTorque() const
    {
        return _last_friction_torque;
    }
};
