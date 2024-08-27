#pragma once
#include <base/IExecutable.h>
#include <base/math/LinearInterpolator.h>
#include <body-building/io/Cmd.h>
#include <memory>

namespace body
{
    /// @brief 信息获取器。其中的信息应该是实时更新的。
    class IStandardMode_InformationGetter
    {
    public:
        virtual double Option_Tension_kg() = 0;
        virtual double Option_WindingSpeed_rpm() = 0;
        virtual double Option_TorqueRatio() = 0;
    };

    class StandardMode :
        public base::IExecutable
    {
    private:
        std::shared_ptr<Cmd> _cmd;
        std::shared_ptr<IStandardMode_InformationGetter> _infos;
        std::shared_ptr<base::LinearInterpolator> _tension_linear_interpolator;
        double _current_tension_kg = 0;

    public:
        StandardMode(std::shared_ptr<Cmd> cmd,
                     std::shared_ptr<IStandardMode_InformationGetter> infos);

        void Execute();
    };

    void Test_StandardMode();

} // namespace body
