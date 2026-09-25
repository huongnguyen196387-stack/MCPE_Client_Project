#include "client/Module.h"

#include <utility>

const char* categoryToString(Category category) {
    switch (category) {
        case Category::PVP:         return "PvP";
        case Category::PERFORMANCE: return "Performance";
        case Category::VISUALS:     return "Visuals";
        case Category::SETTINGS:    return "Settings";
    }
    return "Unknown";
}

Module::Module(std::string name, std::string description, Category category)
    : name_(std::move(name)),
      description_(std::move(description)),
      category_(category) {}

void Module::setEnabled(bool value) {
    ModuleContext empty{};
    setEnabled(value, empty);
}

void Module::setEnabled(bool value, const ModuleContext& ctx) {
    if (enabled_ == value) {
        return;
    }
    enabled_ = value;
    if (enabled_) {
        onEnable(ctx);
    } else {
        onDisable(ctx);
    }
}

void Module::toggle() {
    ModuleContext empty{};
    toggle(empty);
}

void Module::toggle(const ModuleContext& ctx) {
    setEnabled(!enabled_, ctx);
}
