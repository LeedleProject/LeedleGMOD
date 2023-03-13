#pragma once

#include "../netvars.hpp"

#include "../../math.hpp"

struct Collidable {
    virtual void unk() = 0;
    virtual math::Vector& mins() = 0;
    virtual math::Vector& maxs() = 0;
};

class Player;

struct Entity {
    NETVAR("DT_BaseEntity", "m_iHealth", health, int);

    auto* get_collidable() {
        return game::call_virtual_function<3, Collidable*>(this);
    }

    auto is_player() {
        return game::call_virtual_function<130, bool>(this);
    }
};