#pragma once

#include "../types/recvproprs.hpp"

namespace game {

using CreateClientClassFn = void* (*)(int ent_number, int serial_number);
using CreateEventFn = void* (*)();

struct ClientClass {
    CreateClientClassFn create_fn;
    CreateEventFn create_event_fn;
    char* network_name;
    RecvTable* recv_table;
    ClientClass* next;
    int class_id;
};

}  // namespace game