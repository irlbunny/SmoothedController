#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(SmoothedControllerConfig,
    CONFIG_VALUE(Enabled, bool, "Enabled", true, "Enable smoothing");
    CONFIG_VALUE(PositionSmoothing, float, "PositionSmoothing", 3.f, "How much to smooth position changes by");
    CONFIG_VALUE(RotationSmoothing, float, "RotationSmoothing", 12.f, "How much to smooth rotation changes by");
    CONFIG_VALUE(SmallMovementThresholdAngle, float, "SmallMovementThresholdAngle", 6.f, "Angle difference to consider a 'small' movement. The less shaky your hands are the lower you probably want to put this. This is probably the primary value you want to tweak");
)