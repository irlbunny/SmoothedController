#include "types/Wrapper.hpp"

DEFINE_TYPE(SmoothedController, Wrapper);

void SmoothedController::Wrapper::ctor() {
    using namespace UnityEngine;

    smoothedPosition = Vector3::get_zero();
    smoothedRotation = Quaternion::get_identity();
    angleVelocitySnap = 1.0f;
}