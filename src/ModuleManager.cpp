#include "client/ModuleManager.h"

#include "client/Modules/Performance/ChunkUpdateOptimizer.h"
#include "client/Modules/Performance/RenderCullingModule.h"
#include "client/Modules/PvP/ArmorStatus.h"
#include "client/Modules/PvP/CPSCounter.h"
#include "client/Modules/PvP/CustomFOV.h"
#include "client/Modules/PvP/KeystrokesHUD.h"

#include <imgui.h>

#include <array>
#include <string>

namespace {
const std::array<Category, 4> kCategories = {
    Category::PVP,
    Category::PERFORMANCE,
    Category::VISUALS,
    Category::SETTINGS
};
}

ModuleManager::ModuleManager() {
    registerBuiltIns();
}

void ModuleManager::registerBuiltIns() {
    modules_.push_back(std::make_unique<CPSCounter>());
    modules_.push_back(std::make_unique<KeystrokesHUD>());
    modules_.push_back(std::make_unique<ArmorStatus>());
    modules_.push_back(std::make_unique<CustomFOV>());

    modules_.push_back(std::make_unique<RenderCullingModule>());
    modules_.push_back(std::make_unique<ChunkUpdateOptimizer>());
}

Module* ModuleManager::findByName(const std::string& name) const {
    for (const auto& module : modules_) {
        if (module->getName() == name) {
            return module.get();
        }
    }
    return nullptr;
}

bool ModuleManager::toggle(const std::string& name) {
    if (auto* module = findByName(name)) {
        module->toggle();
        return true;
    }
    return false;
}

void ModuleManager::onTick(const ModuleContext& ctx) {
    for (const auto& module : modules_) {
        module->onTick(ctx);
    }
}

void ModuleManager::onRenderUI(const ModuleContext& ctx) {
    for (const auto& module : modules_) {
        if (module->getCategory() == selectedCategory_) {
            module->onRenderUI(ctx);
        }
    }
}

void ModuleManager::onRender3D(const ModuleContext& ctx) {
    for (const auto& module : modules_) {
        if (module->isEnabled()) {
            module->onRender3D(ctx);
        }
    }
}

void ModuleManager::renderModuleRow(Module& module, const ModuleContext& ctx) {
    ImGui::PushID(module.getName().c_str());

    bool enabled = module.isEnabled();
    if (ImGui::Checkbox("##enabled", &enabled)) {
        module.setEnabled(enabled, ctx);
    }

    ImGui::SameLine();
    ImGui::TextUnformatted(module.getName().c_str());

    ImGui::SameLine();
    ImGui::TextDisabled(" - %s", module.getDescription().c_str());

    ImGui::Separator();

    ImGui::Indent(8.0f);
    module.onRenderUI(ctx);
    ImGui::Unindent(8.0f);

    ImGui::Spacing();
    ImGui::PopID();
}

void ModuleManager::renderMenu(const ModuleContext& ctx) {
    ImGui::SetNextWindowSize(ImVec2(900.0f * uiScale_, 560.0f * uiScale_), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("MCPE Client")) {
        ImGui::End();
        return;
    }

    ImGui::BeginChild("Sidebar", ImVec2(190.0f * uiScale_, 0.0f), true);
    ImGui::Text("MCPE CLIENT");
    ImGui::Separator();

    for (const auto category : kCategories) {
        const bool selected = category == selectedCategory_;
        if (ImGui::Selectable(categoryToString(category), selected, 0,
                              ImVec2(-1.0f, 42.0f * uiScale_))) {
            selectedCategory_ = category;
        }
    }

    ImGui::Separator();
    ImGui::TextDisabled("Native Android NDK");
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Content", ImVec2(0.0f, 0.0f), false);
    ImGui::Text("%s", categoryToString(selectedCategory_));
    ImGui::Separator();

    bool any = false;
    for (const auto& module : modules_) {
        if (module->getCategory() != selectedCategory_) {
            continue;
        }
        any = true;
        renderModuleRow(*module, ctx);
    }

    if (!any) {
        if (selectedCategory_ == Category::SETTINGS) {
            ImGui::Text("UI Scale");
            ImGui::SliderFloat("##ui_scale", &uiScale_, 0.75f, 1.5f, "%.2fx");
            ImGui::Spacing();
            ImGui::TextWrapped(
                "Settings is reserved for client-wide configuration. "
                "Add persistence, themes, keybinds, and bridge diagnostics here."
            );
        } else if (selectedCategory_ == Category::VISUALS) {
            ImGui::TextDisabled("No Visuals modules are registered yet.");
        }
    }

    ImGui::EndChild();
    ImGui::End();
}
