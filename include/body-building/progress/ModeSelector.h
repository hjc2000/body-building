#pragma once
#include <base/IExecutable.h>
#include <body-building/io/Cmd.h>
#include <memory>

class ModeSelector :
    public base::IExecutable
{
private:
    std::shared_ptr<base::IExecutable> _body_building_executable;
    std::shared_ptr<Cmd> _cmd;

    void HandleAfterBodyBuildingMode();
    void CreateBodyBuildingModeExecutable();

public:
    ModeSelector(std::shared_ptr<Cmd> cmd)
    {
        _cmd = cmd;
    }

    void Execute();
};
