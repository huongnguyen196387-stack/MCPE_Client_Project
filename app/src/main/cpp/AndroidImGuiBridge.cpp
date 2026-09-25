#include <jni.h>
#include <GLES3/gl3.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"

static bool g_initialized = false;
static int g_width = 1;
static int g_height = 1;

extern "C"
JNIEXPORT void JNICALL
Java_com_mcpe_client_MainActivity_00024NativeBridge_nativeInit(
        JNIEnv*, jclass) {

    if (g_initialized)
        return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;

    ImGui_ImplOpenGL3_Init("#version 300 es");

    g_initialized = true;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mcpe_client_MainActivity_00024NativeBridge_nativeResize(
        JNIEnv*, jclass, jint width, jint height) {

    g_width = width > 0 ? width : 1;
    g_height = height > 0 ? height : 1;

    glViewport(0, 0, g_width, g_height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mcpe_client_MainActivity_00024NativeBridge_nativeTouch(
        JNIEnv*, jclass, jfloat x, jfloat y, jboolean down) {

    if (!g_initialized)
        return;

    ImGuiIO& io = ImGui::GetIO();

    io.AddMousePosEvent(x, y);
    io.AddMouseButtonEvent(0, down == JNI_TRUE);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mcpe_client_MainActivity_00024NativeBridge_nativeMove(
        JNIEnv*, jclass, jfloat x, jfloat y) {

    if (!g_initialized)
        return;

    ImGui::GetIO().AddMousePosEvent(x, y);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mcpe_client_MainActivity_00024NativeBridge_nativeRender(
        JNIEnv*, jclass) {

    if (!g_initialized)
        return;

    glViewport(0, 0, g_width, g_height);
    glClearColor(0.035f, 0.04f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool enabled = true;
    static bool fps = true;
    static bool keystrokes = false;
    static bool armor = false;
    static float fov = 90.0f;

    ImGui::SetNextWindowPos(
        ImVec2(g_width * 0.08f, g_height * 0.08f),
        ImGuiCond_Once);

    ImGui::SetNextWindowSize(
        ImVec2(g_width * 0.84f, g_height * 0.78f),
        ImGuiCond_Once);

    ImGui::Begin(
        "MCPE Client",
        nullptr,
        ImGuiWindowFlags_NoCollapse);

    ImGui::Text("MCPE Client");
    ImGui::Separator();

    ImGui::Text("Status: Android / OpenGL ES 3");
    ImGui::Spacing();

    if (ImGui::BeginTabBar("ClientTabs")) {

        if (ImGui::BeginTabItem("Performance")) {
            ImGui::Checkbox("Client Enabled", &enabled);
            ImGui::Checkbox("FPS Counter", &fps);
            ImGui::Checkbox("Render Culling", &enabled);
            ImGui::Checkbox("Chunk Optimizer", &enabled);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("PvP")) {
            ImGui::Checkbox("Keystrokes HUD", &keystrokes);
            ImGui::Checkbox("Armor Status", &armor);
            ImGui::SliderFloat("FOV", &fov, 60.0f, 120.0f);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Info")) {
            ImGui::Text("MCPE Client Project");
            ImGui::Text("Native C++ / Dear ImGui");
            ImGui::Text("OpenGL ES 3 renderer");
            ImGui::Spacing();
            ImGui::Text("Integration skeleton");
            ImGui::Text("No Bedrock offsets or injection code.");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Spacing();
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
