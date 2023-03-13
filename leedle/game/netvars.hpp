#pragma once

#include <stdint.h>
#include <vcruntime.h>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

#include "fnv.hpp"
#include "interfaces/vclient.hpp"

#include "loguru.hpp"

#define NETVAR(table, prop, funcname, type)  \
    type& funcname() { \
        static auto offset = 0; \
        if (offset == 0) { \
            offset = game::NETVARS.get_netvar(table, prop); \
        } \
        return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
    }

#define NETVAR_PTR(table, prop, funcname, type)  \
    type* funcname() { \
        static auto offset = 0; \
        if (offset == 0) { \
            offset = game::NETVARS.get_netvar(table, prop) \
        } \
        return reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
    }

namespace game {

class Netvars {
    using hashed = uint32_t;
    using NetvarKeyValueMap = std::unordered_map<hashed, ptrdiff_t>;
    using NetvarTableMap = std::unordered_map<hashed, NetvarKeyValueMap>;

    NetvarTableMap netvars;

    void collect_props(RecvTable* table, ptrdiff_t child_offset = 0) {
        for (auto& prop : std::span<RecvProp>(table->props, table->props + table->props_count)) {
            if (prop.data_table && prop.elements_count > 0) {
                if (std::string_view(prop.prop_name).starts_with("0"))
                    continue;
                collect_props(prop.data_table, child_offset + prop.offset);
            }

            std::string_view name = prop.prop_name;
            if (not name.starts_with("m"))
                continue;
            
            auto name_hash = Fnv::hash(prop.prop_name);
            auto table_name_hash = Fnv::hash(table->table_name);
            auto offset = child_offset + prop.offset;

            netvars[table_name_hash][name_hash] = offset;

            LOG_S(INFO) << "Registred netvar " << table->table_name << "::" << prop.prop_name;
        }
    }

    auto initialize() {
        if (netvars.empty()) {
            for (auto client_class = CLIENT->get_all_classes(); client_class; client_class = client_class->next) {
                auto table = client_class->recv_table;
                if (table == nullptr)
                    continue;

                auto table_name = std::string_view(table->table_name);
                auto table_name_hash = Fnv::hash(table_name.data());

                collect_props(table);
            }
        }
    }

public:
    auto get_netvar(std::string_view classname, std::string_view name) {
        auto class_hash = Fnv::hash(classname.data());
        auto name_hash = Fnv::hash(name.data());

        initialize();

        return netvars.count(class_hash) > 0 && netvars[class_hash].count(name_hash) > 0 ? netvars[class_hash][name_hash]
            : ptrdiff_t(0);
    }

    auto get_netvars() {
        return std::ref(netvars);
    }
};

inline Netvars NETVARS;

}