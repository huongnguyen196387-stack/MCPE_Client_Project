#pragma once

#include "client/Module.h"

#include <memory>
#include <string>
#include <vector>

class ModuleManager {
public:
    ModuleManager();

    template <typename T>
    T* get() const {
        for (const auto& module : modules_) {
            if (auto* casted = dynamic_cast<T*>(module.get())) {
                return casted;
            }
        }
        return nullptr;
    }

    Module* findByName(const std::string& name) const;
    bool toggle(const std::string& name);

    void onTick(const ModuleContext& ctx);
    void onRenderUI(const ModuleContext& ctx);
    void onRender3D(const ModuleContext& ctx);

    void renderMenu(const ModuleContext& ctx);

private:
    void registerBuiltIns();
    void renderModuleRow(Module& module, const ModuleContext& ctx);

    std::vector<std::unique_ptr<Module>> modules_;
    Category selectedCategory_{Category::PVP};
    float uiScale_{1.0f};
};
