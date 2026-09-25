#pragma once

#include "client/GameBridge.h"
#include "client/InputState.h"

#include <string>

enum class Category {
    PVP,
    PERFORMANCE,
    VISUALS,
    SETTINGS
};

const char* categoryToString(Category category);

struct ModuleContext {
    IGameBridge* game{nullptr};
    const InputState* input{nullptr};
    std::uint64_t nowMs{0};
};

class Module {
public:
    Module(std::string name, std::string description, Category category);
    virtual ~Module() = default;

    const std::string& getName() const { return name_; }
    const std::string& getDescription() const { return description_; }
    Category getCategory() const { return category_; }

    bool isEnabled() const { return enabled_; }
    void setEnabled(bool value);
    void setEnabled(bool value, const ModuleContext& ctx);
    void toggle();
    void toggle(const ModuleContext& ctx);

    virtual void onTick(const ModuleContext& ctx) = 0;
    virtual void onRenderUI(const ModuleContext& ctx) = 0;
    virtual void onRender3D(const ModuleContext& ctx) = 0;

protected:
    virtual void onEnable(const ModuleContext&) {}
    virtual void onDisable(const ModuleContext&) {}

private:
    std::string name_;
    std::string description_;
    Category category_;
    bool enabled_{false};
};
