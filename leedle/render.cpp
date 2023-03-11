#include "render.hpp"

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <kiero.h>

#include <functional>
#include <loguru.hpp>
#include <magic_enum.hpp>

// TODO: Reset hook

using namespace render;

static IDirect3DDevice9* DEVICE = nullptr;

struct OverrideRenderState {
	DWORD original_value;
	D3DRENDERSTATETYPE state_type;
	IDirect3DDevice9* direct_device;

	OverrideRenderState(IDirect3DDevice9* direct_device, D3DRENDERSTATETYPE state, DWORD override_value) : state_type(state), direct_device(direct_device) {
		direct_device->GetRenderState(state, &original_value);
		direct_device->SetRenderState(state, override_value);
	}
	~OverrideRenderState() {
		direct_device->SetRenderState(state_type, original_value);
	}
};

void Render::setup_hooks() {
    hooks::initialize_hooks(end_scane_hook, wnd_proc_hook);
    LOG_S(INFO) << "Render initialized";
}

void Render::uninitialize() {
    hooks::remove_hooks(end_scane_hook, wnd_proc_hook);
}

long Render::end_scene_callback(IDirect3DDevice9* device) {
    if (DEVICE == nullptr) {
        DEVICE = device;
        LOG_S(INFO) << "Device: " << std::hex << device;

        ImGui::CreateContext();
        LOG_S(INFO) << "ImGui initialized";
        if (auto* hwnd = FindWindowW(L"Valve001", 0); hwnd) {
            LOG_S(INFO) << "Found window: " << std::hex << hwnd;

            ImGui_ImplWin32_Init(hwnd);
            LOG_S(INFO) << "Win32 backend initialized";

            ImGui_ImplDX9_Init(DEVICE);
            LOG_S(INFO) << "DX9 backend initialized";

            ImGui::GetIO().IniFilename = nullptr;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

            static const ImWchar ranges[] = {
                0x0020,
                0x00FF,  // Basic Latin + Latin Supplement
                0x0400,
                0x044F,  // Cyrillic
                0,
            };

            auto* font = ImGui::GetIO().Fonts->AddFontFromFileTTF(
                "C:/Windows/Fonts/L_10646.ttf",
                18.f,
                0,
                ranges);
            LOG_S(INFO) << "Font created " << std::hex << font;
        }
    }

    OverrideRenderState srgb_override(device, D3DRS_SRGBWRITEENABLE, 0);
	OverrideRenderState colorwriteenable(device, D3DRS_COLORWRITEENABLE, 0xffffffff);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("First");

    ImGui::End();

    ImGui::Begin("Second");

    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    return end_scane_hook._hook.original(DEVICE);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Render::wndproc_callback(
    HWND hwnd,
    UINT msg,
    WPARAM wparam,
    LPARAM lparam) {
    
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    
    return CallWindowProc(WndProcHook::original, hwnd, msg, wparam, lparam);
}

void EndSceneHook::initialize() {
    auto kiero_init_result = kiero::init(kiero::RenderType::D3D9);
    DLOG_S(INFO) << "Initializing EndScene";

    CHECK_S(kiero_init_result == kiero::Status::Success)
        << "Failed to initialize kiero: "
        << magic_enum::enum_name(kiero_init_result);
    DLOG_S(INFO) << "EndScene initialized";
}

void EndSceneHook::hook() {
    DLOG_S(INFO) << "Hooking EndScene";
    _hook.detour = EndSceneHook::end_scene_hooked;
    _hook.hook(kiero::getMethodsTable()[42]);
    DLOG_S(INFO) << "EndScene hooked. Original: " << std::hex
                 << (uintptr_t)_hook.original << " Detour: " << std::hex
                 << (uintptr_t)_hook.detour;
}

void EndSceneHook::unhook() {
    DLOG_S(INFO) << "UNHooking EndScene";
    _hook.unhook();
    DLOG_S(INFO) << "EndScene unhooked";
}

long __stdcall EndSceneHook::end_scene_hooked(IDirect3DDevice9* device) {
    return EndSceneHook::callback(device);
}

void WndProcHook::initialize() {}

void WndProcHook::hook() {
    auto* hwnd = FindWindowW(L"Valve001", 0);
    WndProcHook::original = (WNDPROC)SetWindowLongPtr(
        hwnd,
        GWLP_WNDPROC,
        (LONG_PTR)WndProcHook::wndproc_hooked);
}

void WndProcHook::unhook() {}

long WndProcHook::wndproc_hooked(
    HWND hwnd,
    UINT msg,
    WPARAM wparam,
    LPARAM lparam) {
    //DLOG_S(INFO) << "New windows message";
    return WndProcHook::callback(hwnd, msg, wparam, lparam);
}