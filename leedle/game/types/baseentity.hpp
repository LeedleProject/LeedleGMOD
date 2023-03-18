#pragma once

#include "../netvars.hpp"

#include "../../math.hpp"

namespace game {

struct Collidable {
    virtual void unk() = 0;
    virtual math::Vector& mins() = 0;
    virtual math::Vector& maxs() = 0;
};

struct Entity {
    NETVAR("DT_BaseEntity", "m_iHealth", health, int);
    NETVAR("DT_BasePlayer", "m_fFlags", flags, int);

    auto* get_collidable() {
        return game::call_virtual_function<3, Collidable*>(this);
    }

    auto is_player() {
        return game::call_virtual_function<130, bool>(this);
    }
};

using Player = Entity;

inline auto get_local_player() {
    static auto function = memory::MemoryScanner("E8 ? ? ? ? 8B 6B 40")
        .scan<Player*(__fastcall*)()>(memory::client).absolute(0x1, 0x5);
    return (*function.get_pointer())();
}

}