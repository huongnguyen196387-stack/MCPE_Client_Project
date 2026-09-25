#pragma once

#include <android/native_window.h>

class ImGuiLayer {
public:
    bool initialize(ANativeWindow* window);
    void shutdown();

    void newFrame();
    void render();

    bool isInitialized() const { return initialized_; }

private:
    bool initialized_{false};
};
