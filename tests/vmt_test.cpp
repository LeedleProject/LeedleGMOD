#include <iostream>
#include <memory.hpp>
#include <memory>
#include <Windows.h>

struct Foo {
    int a = 12;

    virtual int foo() {
        return 1;
    }
};

static int __fastcall foo_detour(Foo* foo, void*) {
    std::cout << "foo_detour called foo->a: " << foo->a << std::endl;
    return foo->a != 12;
}

int main() {
    auto foo_local = std::make_shared<Foo>();

    memory::VMTHook hook { .detour = foo_detour };
    hook.hook((uintptr_t**)foo_local.get(), 0);

    auto result = foo_local->foo();
    return result;
}