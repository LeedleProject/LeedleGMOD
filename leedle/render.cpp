#include "render.hpp"

#include <imgui.h>
#include <kiero.h>
#include <winuser.h>

#include <functional>
#include <loguru.hpp>
#include <magic_enum.hpp>

#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

using namespace render;

static IDirect3DDevice9* DEVICE = nullptr;

void Render::setup_hooks() {
    hooks::initialize_hooks(end_scane_hook);
}

void Render::uninitialize() {
    hooks::remove_hooks(end_scane_hook);
}

long Render::end_scene_callback(IDirect3DDevice9* device) {
    if (not DEVICE) {
        DEVICE = device;
        LOG_S(INFO) << "Device: " << std::hex << device << std::endl;

        ImGui::CreateContext();
        if (auto hwnd = FindWindowW(L"Valve001", 0); hwnd) {
            ImGui_ImplWin32_Init(hwnd);
            ImGui_ImplDX9_Init(device);
            ImGui::GetIO().IniFilename = nullptr;

            static const ImWchar ranges[] = {
				0x0020, 0x00FF, // Basic Latin + Latin Supplement
				0x0400, 0x044F, // Cyrillic
				0,
			};
            ImGui::GetIO().Fonts->AddFontFromFileTTF("C:/Windows/Fonts/L_10646.ttf", 18.f, 0, ranges);
        }
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, world!");

    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return end_scane_hook.vmt_hook.original(device);
}

long Render::wndproc_callback(
    HWND hwnd,
    UINT msg,
    WPARAM wparam,
    LPARAM lparam) {
    return CallWindowProc(WndProcHook::original, hwnd, msg, wparam, lparam);
}

void EndSceneHook::initialize() {
    auto kiero_init_result = kiero::init(kiero::RenderType::D3D9);

    CHECK_S(kiero_init_result == kiero::Status::Success)
        << "Failed to initialize kiero: "
        << magic_enum::enum_name(kiero_init_result);
    vmt_hook.detour = EndSceneHook::end_scene_hooked;
}

void EndSceneHook::hook() {
    vmt_hook.hook(kiero::getMethodsTable());
}

void EndSceneHook::unhook() {
    vmt_hook.hook(kiero::getMethodsTable());
}

long EndSceneHook::end_scene_hooked(IDirect3DDevice9* device) {
    return EndSceneHook::callback(device);
}

void WndProcHook::initialize() {}

void WndProcHook::hook() {}

void WndProcHook::unhook() {}