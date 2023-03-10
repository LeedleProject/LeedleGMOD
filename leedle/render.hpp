#pragma once

#include "leedle.hpp"
#include "hooks.hpp"
#include "memory.hpp"

#include <functional>
#include <d3d9.h>
#include <string>

namespace render {
    struct EndSceneHook : public hooks::IHook {
        using function_type = long(__cdecl*)(IDirect3DDevice9*);

        static inline std::function<long(IDirect3DDevice9* device)> callback;

        memory::hook_methods::VMTHook<function_type, 42> vmt_hook;
        
        void initialize() override;
        void hook() override;
        void unhook() override;

        static long end_scene_hooked(IDirect3DDevice9* device);
    };

    class Render : leedle::IModule {
    private:
        EndSceneHook end_scane_hook;

        long end_scene_callback(IDirect3DDevice9* device);
    public:
        Render() {
            EndSceneHook::callback = std::bind(&Render::end_scene_callback, this, std::placeholders::_1);
        }   

        void setup_hooks() override;
        void uninitialize() override;
    };
}

