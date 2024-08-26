#include "Option.h"

void Option::Refresh_BodyBuildingMode()
{
    _last_body_building_mode = _body_building_mode;
    uint32_t mode_code = static_cast<uint32_t>(DD(0)) >> 16;
    _body_building_mode = static_cast<Option_BodyBuildingMode>(mode_code);
}

Option_BodyBuildingMode Option::BodyBuildingMode()
{
    return _body_building_mode;
}

Option_BodyBuildingMode Option::LastBodyBuildingMode()
{
    return _last_body_building_mode;
}

bool Option::BodyBuildingModeChanged()
{
    return _body_building_mode != _last_body_building_mode;
}

void Option::Clear_AdditionalMode()
{
    DD(20, 0);
}

void Option::Refresh_AdditionalMode()
{
    _last_additional_mode = _additional_mode;
    _additional_mode = static_cast<Option_AdditionalMode>(DD(20));
}

Option_AdditionalMode Option::AdditionalMode()
{
    return _additional_mode;
}

Option_AdditionalMode Option::LastAdditionalMode()
{
    return _last_additional_mode;
}

bool Option::AdditionalModeCodeChanged()
{
    return _additional_mode != _last_additional_mode;
}

void Option::Refresh_Tension_kg()
{
    _last_tension_kg = _tension_kg;
    _tension_kg = static_cast<uint16_t>(DD(0));
}

double Option::Tension_kg()
{
    return _tension_kg;
}

double Option::LastTension_kg()
{
    return _last_tension_kg;
}

bool Option::Tension_kg_Changed()
{
    return _tension_kg != _last_tension_kg;
}

double Option::MaxTension_kg()
{
    return 0.0;
}

int Option::MinTorque()
{
    return static_cast<double>(SRV_PARA(1, 29)) / 5;
}

int Option::MaxTorque()
{
    return SRV_PARA(2, 69);
}

double Option::WindingSpeed()
{
    return static_cast<double>(SRV_PARA(1, 41)) / 100;
}

double Option::TorqueRatio()
{
    return static_cast<double>(SRV_PARA(1, 42)) / 100;
}

bool Option::UseGravitationPackage()
{
    return DD(13);
}

int Option::ZeroPositionProtectionThreshold()
{
    return SRV_PARA(2, 14) * 30;
}

double Option::UpdateFrequencyOfMaxTorque_Hz()
{
    double freq = static_cast<double>(DD(9)) / 10;
    if (freq > 500 || freq < 0.1)
    {
        freq = 500;
    }

    return 500;
}

double Option::OverspeedDampingCoefficientThreshold()
{
    return SRV_PARA(2, 33);
}

double Option::DampingCoefficientWhenOverspeeding()
{
    return static_cast<double>(SRV_PARA(2, 34)) / 1000;
}

int Option::IntelligentModeTensionCompensation()
{
    return 0;
}

double Option::WindingSpeedLimitDuringBigWindingResistance()
{
    return 0.0;
}

bool Option::AutoDisableServoAfterReturningToZero()
{
    return false;
}

int Option::ReturnToZeroTorque()
{
    return 0;
}

double Option::ReturnToZeroSpeed()
{
    return 0.0;
}

int Option::ReduceTorque()
{
    return 0;
}

int Option::BigTorque()
{
    return 0;
}
