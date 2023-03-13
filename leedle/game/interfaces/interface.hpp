#pragma once

#include <vadefs.h>
#include <vcruntime.h>
#include <concepts>
#include <string_view>
#include "../../memory.hpp"

/*
    Interface name must me in uppercase
*/
#define DEFINE_INTERFACE(_class, name) inline game::Interface<_class> name = _class::capture_interface();

namespace game {
    template <size_t index, class ReturnType = void, class ...Args>
    constexpr auto call_virtual_function(auto* _self, Args&& ...args) {
        using function_type = ReturnType(__fastcall*)(void*, Args...);
        return (*(function_type**)_self)[index](_self, std::forward<Args>(args)...);
    }

    struct InterfaceReg {
        void*(*create_function)();
        const char* name;
        InterfaceReg* next;
    };

    template <class T>
    struct Interface {
        T* _self;

        Interface(T* _self) : _self(_self) {}

        T* operator->() { return _self; }
        T& operator*() { return *_self; }

        uintptr_t* get_virtual_table() { return *(uintptr_t**)_self; }

        template <size_t index, class ReturnType = void, class ...Args>
        static constexpr auto call_virtual(auto* _self, Args&& ...args) {
            return call_virtual_function<index, ReturnType>(_self, std::forward<Args>(args)...);
        }

        static Interface<T> create_interface(memory::MemoryModule& module, std::string_view name) {
            using CreateInterfaceType = T*(*)(const char*, int);
            return { module.get_symbol<CreateInterfaceType>("CreateInterface").template invoke_r<T*>(name.data(), 0) };
        }
    };
}