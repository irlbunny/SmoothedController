#include "main.hpp"
#include "SmoothedController.hpp"
#include "SmoothedControllerConfig.hpp"

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
    
    getSmoothedControllerConfig().Init(modInfo);
    getLogger().info("Completed SmoothedController setup!");
}

extern "C" void load() {
    il2cpp_functions::Init();

    getLogger().info("Installing SmoothedController...");
    SmoothedController::Install();
    getLogger().info("Installed SmoothedController!");
}