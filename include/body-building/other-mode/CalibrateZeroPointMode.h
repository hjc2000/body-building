#pragma once
#include <body-building/io/Cmd.h>
#include <functional>
#include <memory>

/// @brief 零点校准模式
class CalibrateZeroPointMode
{
private:
    bool _is_completed = false;
    int _progress = 0;
    int _zero_speed_holding_tick = 0;
    std::shared_ptr<Cmd> _cmd;

public:
    static CalibrateZeroPointMode &Instance()
    {
        static CalibrateZeroPointMode o;
        return o;
    }

    void Execute();

    /// @brief 零点校准完成。
    /// @return
    bool IsCompleted() const;

    /// @brief 校准零点的进度。
    /// @return 返回 0 表示没有使用过零点校准模式。返回 1 表示正在校准。返回 2 表示校准完成。
    int Progress();

    /// @brief 重置零点校准模式完成状态。这会导致下轮主循环进入零点校准模式。
    void ResetCompletionStatus();

    /// @brief 零点校准完成时触发的回调。
    std::function<void()> _on_completed = nullptr;
};
