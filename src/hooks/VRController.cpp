#include "SmoothedController.hpp"
#include "SmoothedControllerConfig.hpp"

#include "types/Wrapper.hpp"

#include "GlobalNamespace/IVRPlatformHelper.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/VRControllerTransformOffset.hpp"
#include "System/Math.hpp"
#include "UnityEngine/Mathf.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/XR/XRNode.hpp"

#include <map>

std::map<UnityEngine::XR::XRNode, SafePtr<SmoothedController::Wrapper>> wrappers;

void SmoothController(GlobalNamespace::VRController* instance) {
    using namespace System;
    using namespace System::Collections::Generic;
    using namespace UnityEngine;
    using namespace UnityEngine::XR;

    if (!instance || !getSmoothedControllerConfig().Enabled.GetValue()) {
        return;
    }

    static float posSmoth = 20.f - Mathf::Clamp(getSmoothedControllerConfig().PositionSmoothing.GetValue(), 0.f, 20.f);
    static float rotSmoth = 20.f - Mathf::Clamp(getSmoothedControllerConfig().RotationSmoothing.GetValue(), 0.f, 20.f);

    SafePtr<SmoothedController::Wrapper> wrapperI = nullptr;
    if (wrappers.find(instance->get_node()) == wrappers.end()) {
        wrapperI = CRASH_UNLESS(il2cpp_utils::New<SmoothedController::Wrapper*>());
        wrappers[instance->get_node()] = *wrapperI;
    } else {
        wrapperI = *wrappers[instance->get_node()];
    }

    float angDiff = Quaternion::Angle(wrapperI->smoothedRotation, instance->get_transform()->get_localRotation());
    wrapperI->angleVelocitySnap = Math::Min(wrapperI->angleVelocitySnap + angDiff, 90.f);

    float snapMulti = Mathf::Clamp(wrapperI->angleVelocitySnap / getSmoothedControllerConfig().SmallMovementThresholdAngle.GetValue(), .1f, 2.5f);

    if (wrapperI->angleVelocitySnap > .1f) {
        wrapperI->angleVelocitySnap -= Math::Max(.4f, wrapperI->angleVelocitySnap / 1.7f);
    }

    if (getSmoothedControllerConfig().PositionSmoothing.GetValue() > 0.f) {
        wrapperI->smoothedPosition = Vector3::Lerp(wrapperI->smoothedPosition, instance->get_transform()->get_localPosition(), posSmoth * Time::get_deltaTime() * snapMulti);
        instance->get_transform()->set_localPosition(wrapperI->smoothedPosition);
    }

    if (getSmoothedControllerConfig().RotationSmoothing.GetValue() > 0.f) {
        wrapperI->smoothedRotation = Quaternion::Lerp(wrapperI->smoothedRotation, instance->get_transform()->get_localRotation(), rotSmoth * Time::get_deltaTime() * snapMulti);
        instance->get_transform()->set_localRotation(wrapperI->smoothedRotation);
    }
}

MAKE_HOOK_MATCH(
    VRController_Update,
    &GlobalNamespace::VRController::Update,
    void,
    GlobalNamespace::VRController* self
) {
    using namespace UnityEngine;
    using namespace UnityEngine::XR;
    
    // Because Quest is dumb and we don't have transpilers, we gotta reimplement this entire method :D

    Vector3 lastTrackedPosition;
    Quaternion localRotation;
    if (!self->vrPlatformHelper->GetNodePose(self->node, self->nodeIdx, lastTrackedPosition, localRotation)) {
        if (self->lastTrackedPosition != Vector3::get_zero()) {
            lastTrackedPosition = self->lastTrackedPosition;
        } else if (self->node == XRNode::_get_LeftHand()) {
            lastTrackedPosition = Vector3(-.2f, .05f, .0f);
        } else if (self->node == XRNode::_get_RightHand()) {
            lastTrackedPosition = Vector3(.2f, .05f, .0f);
        }
    } else {
        self->lastTrackedPosition = lastTrackedPosition;
    }

    self->get_transform()->set_localPosition(lastTrackedPosition);
    self->get_transform()->set_localRotation(localRotation);

    if (self->get_gameObject()->get_name().starts_with("Controller")) {
        SmoothController(self);
    }

    if (self->transformOffset != nullptr) {
        self->vrPlatformHelper->AdjustControllerTransform(self->node, self->get_transform(), self->transformOffset->get_positionOffset(), self->transformOffset->get_rotationOffset());
        return;
    }

    self->vrPlatformHelper->AdjustControllerTransform(self->node, self->get_transform(), Vector3::get_zero(), Vector3::get_zero());
}

void SmoothedController::Hooks::VRController() {
    INSTALL_HOOK(getLogger(), VRController_Update);
}