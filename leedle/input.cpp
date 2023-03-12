#include "input.hpp"
#include <winuser.h>

using namespace input;

void Input::setup_hooks() {
    hooks::initialize_hooks(lock_cursor_hook);
}

void Input::uninitialize() {
    hooks::remove_hooks(lock_cursor_hook);
}

void LockCursorHook::lock_cursor_hooked(void* data) {
    if (not INPUT.is_cursor_locked()) {
        return game::call_virtual_function<65>(data);
    }
    return INPUT.lock_cursor_hook._hook.original(data);
}