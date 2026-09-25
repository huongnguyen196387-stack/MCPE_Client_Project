cmake_minimum_required(VERSION 3.22)
project(MCPEClient LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../..")

include(FetchContent)

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
    ${ROOT_DIR}/src/main.cpp
    ${ROOT_DIR}/src/Module.cpp
    ${ROOT_DIR}/src/ModuleManager.cpp
    ${ROOT_DIR}/src/core/Frustum.cpp
    ${ROOT_DIR}/src/ui/ImGuiLayer.cpp
    ${ROOT_DIR}/src/Modules/Performance/RenderCullingModule.cpp
    ${ROOT_DIR}/src/Modules/Performance/ChunkUpdateOptimizer.cpp
    ${ROOT_DIR}/src/Modules/PvP/CPSCounter.cpp
    ${ROOT_DIR}/src/Modules/PvP/KeystrokesHUD.cpp
    ${ROOT_DIR}/src/Modules/PvP/ArmorStatus.cpp
    ${ROOT_DIR}/src/Modules/PvP/CustomFOV.cpp
    ${ROOT_DIR}/src/integration/RenderHookAdapter.cpp
)

add_library(mcpe_client SHARED
    ${CLIENT_SOURCES}
    ${IMGUI_SOURCES}
)

target_include_directories(mcpe_client PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${ROOT_DIR}/include
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

list(APPEND SOURCES "${CMAKE_CURRENT_LIST_DIR}/AndroidImGuiBridge.cpp")
