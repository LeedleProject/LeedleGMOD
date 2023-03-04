#include <iostream>
#include <memory.hpp>
#include <memory>
#include <Windows.h>

constexpr auto magic = 12;

struct Foo {
    int a = magic;

    virtual int foo() {
        return 1;
    }
};

static int __fastcall foo_detour(Foo* foo, void* _ecx) {
    std::cout << "foo_detour called foo->a: " << foo->a << std::endl;
    return (int)(foo->a != magic);
}

int main() {
    auto foo_local = std::make_shared<Foo>();

    memory::hook_methods::VMTHook<int(__fastcall*)(Foo*, void*)> hook { .detour = foo_detour };
    hook.hook((uintptr_t**)foo_local.get(), 0);

    auto result = foo_local->foo();
    return result;
}