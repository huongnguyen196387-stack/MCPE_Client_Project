cmake_minimum_required(VERSION 3.22)
project(MCPEClient LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT ANDROID)
    message(WARNING "This project is intended to be built with the Android NDK toolchain.")
endif()

include(FetchContent)

# Dear ImGui is fetched at configure time to keep the project ZIP small.
# Pinned to a known 1.92.6 release; update intentionally when upgrading.
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.6
    GIT_SHALLOW TRUE
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

set(IMGUI_DIR ${imgui_SOURCE_DIR})

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_android.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set(CLIENT_SOURCES
    src/main.cpp
    src/Module.cpp
    src/ModuleManager.cpp
    src/core/Frustum.cpp
    src/ui/ImGuiLayer.cpp
    src/Modules/Performance/RenderCullingModule.cpp
    src/Modules/Performance/ChunkUpdateOptimizer.cpp
    src/Modules/PvP/CPSCounter.cpp
    src/Modules/PvP/KeystrokesHUD.cpp
    src/Modules/PvP/ArmorStatus.cpp
    src/Modules/PvP/CustomFOV.cpp
    src/integration/RenderHookAdapter.cpp
)

add_library(mcpe_client SHARED
    ${CLIENT_SOURCES}
    ${IMGUI_SOURCES}
)

target_include_directories(mcpe_client PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(mcpe_client PRIVATE
    IMGUI_IMPL_OPENGL_ES3
    MCPE_CLIENT_BUILD
)

target_compile_options(mcpe_client PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
)

target_link_libraries(mcpe_client PRIVATE
    android
    log
    GLESv3
    EGL
    dl
)

set_target_properties(mcpe_client PROPERTIES
    OUTPUT_NAME "mcpe_client"
)
