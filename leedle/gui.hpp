#pragma once

#include "module.hpp"

namespace gui {

enum class MenuState {
    Closed,
    Open,
};

class GUI: public leedle::IModule {
    MenuState menu_state = MenuState::Closed;

public:
    GUI() {}

    void setup_hooks() override {}
    void uninitialize() override {}

    constexpr auto is_open() { return menu_state == MenuState::Open; }
    constexpr auto get_menu_state() { return menu_state; }
    constexpr auto set_menu_state(MenuState state) { menu_state = state; }
    constexpr auto open_menu() { set_menu_state(MenuState::Open); }
    constexpr auto close_menu() { set_menu_state(MenuState::Closed); }

    void render();
};

inline GUI GUI;
}  // namespace gui