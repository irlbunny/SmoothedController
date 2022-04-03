#pragma once

#include "main.hpp"

DECLARE_CLASS_CODEGEN(SmoothedController, Wrapper, ::Il2CppObject,
    DECLARE_INSTANCE_FIELD(UnityEngine::Vector3, smoothedPosition);
    DECLARE_INSTANCE_FIELD(UnityEngine::Quaternion, smoothedRotation);
    DECLARE_INSTANCE_FIELD(float, angleVelocitySnap);
    DECLARE_CTOR(ctor);
)