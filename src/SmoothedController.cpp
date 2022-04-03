#include "SmoothedController.hpp"
#include "SmoothedControllerConfig.hpp"

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    using namespace UnityEngine;

    if (firstActivation) {
        GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(self->get_transform());
        
        AddConfigValueToggle(container->get_transform(), getSmoothedControllerConfig().Enabled);
        AddConfigValueIncrementFloat(container->get_transform(), getSmoothedControllerConfig().PositionSmoothing, 2, .5f, 0, 20);
        AddConfigValueIncrementFloat(container->get_transform(), getSmoothedControllerConfig().RotationSmoothing, 2, .5f, 0, 20);
        AddConfigValueIncrementFloat(container->get_transform(), getSmoothedControllerConfig().SmallMovementThresholdAngle, 2, .5f, 0.f, 15);
    }
}

void SmoothedController::Install() {
    custom_types::Register::AutoRegister();
    
    QuestUI::Init();
    QuestUI::Register::RegisterMainMenuModSettingsViewController(modInfo, DidActivate);
    QuestUI::Register::RegisterModSettingsViewController(modInfo, DidActivate);
    
    SmoothedController::Hooks::VRController();
}