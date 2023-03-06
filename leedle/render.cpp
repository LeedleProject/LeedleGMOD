#include "render.hpp"

#include <kiero.h>
#include <assert.h>

using namespace render;

void Render::setup_hooks() {
    hooks::initialize_hooks(end_scane_hook);
}

void Render::uninitialize() {
    hooks::remove_hooks(end_scane_hook);
}

long Render::end_scene_callback(IDirect3DDevice9* device) {
    return end_scane_hook.vmt_hook.original(device);;
}

void EndSceneHook::initialize() {
    assert(kiero::init(kiero::RenderType::D3D9));
    vmt_hook.detour = EndSceneHook::end_scene_hooked;
}

void EndSceneHook::hook() {
    vmt_hook.hook(kiero::getMethodsTable(), EndSceneHook::index);
}

void EndSceneHook::unhook() {
    vmt_hook.hook(kiero::getMethodsTable(), EndSceneHook::index);
}

long EndSceneHook::end_scene_hooked(IDirect3DDevice9* device) {
    return EndSceneHook::callback(device);
}