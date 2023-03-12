#pragma once

#include <d3d9.h>
#include <minwindef.h>
#include <winuser.h>

#include <functional>
#include <string>

#include "hooks.hpp"
#include "memory.hpp"
#include "module.hpp"


namespace render {
struct EndSceneHook: public hooks::IHook {
    using function_type = long(__stdcall*)(IDirect3DDevice9*);

    static inline std::function<long(IDirect3DDevice9* device)> callback;

    memory::hook_methods::MinHook<function_type> _hook;

    void initialize() override;
    void hook() override;
    void unhook() override;

    static long __stdcall end_scene_hooked(IDirect3DDevice9* device);
};

struct WndProcHook: public hooks::IHook {
    static inline std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> callback;
    static inline WNDPROC original = nullptr;

    void initialize() override;
    void hook() override;
    void unhook() override;

    static long wndproc_hooked(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

class Render: leedle::IModule {
  private:
    EndSceneHook end_scane_hook;
    WndProcHook wnd_proc_hook;

    long end_scene_callback(IDirect3DDevice9* device);
    LRESULT __stdcall wndproc_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    IDirect3DDevice9* device = nullptr;
    HWND hwnd = nullptr;
  public:
    Render() {
        EndSceneHook::callback =
            std::bind(&Render::end_scene_callback, this, std::placeholders::_1);
        WndProcHook::callback = std::bind(
            &Render::wndproc_callback,
            this,
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3,
            std::placeholders::_4);
    }

    void setup_hooks() override;
    void uninitialize() override;

    auto* get_device() { return device; }
    auto* get_hwnd() { return hwnd; }
};

inline Render RENDER;
}  // namespace render
