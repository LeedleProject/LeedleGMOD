#include "input.hpp"
#include <winuser.h>

using namespace input;

void Input::initialize() {
    hooks::initialize_hooks(lock_cursor_hook);
}

void Input::shutdown() {
    hooks::remove_hooks(lock_cursor_hook);
}

void LockCursorHook::lock_cursor_hooked(void* data) {
    if (not Input::get().is_cursor_locked()) {
        return game::call_virtual_function<65>(data);
    }
    return Input::get().lock_cursor_hook._hook.original(data);
}