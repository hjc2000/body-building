#pragma once

class GraduallyDisableEnableServo
{
private:
    GraduallyDisableEnableServo() = default;

public:
    static GraduallyDisableEnableServo &Instance()
    {
        static GraduallyDisableEnableServo o;
        return o;
    }

    void GraduallyDisableServo();
    void GraduallyEnableServo();
};
