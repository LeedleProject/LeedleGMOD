#include <Windows.h>
#include <SafeCall.h>
#include <string>
#include <array>
#include <algorithm>
#include <iostream>

bool __stdcall check_return_address() {
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(reinterpret_cast<void*>(_ReturnAddress()), &mbi, sizeof(mbi));
    std::array<char, MAX_PATH> buffer;
    GetModuleFileName((HMODULE)mbi.AllocationBase, buffer.data(), buffer.size());
    std::string module_name(buffer.data());
    std::transform(module_name.begin(), module_name.end(), module_name.begin(), ::tolower);

    std::cout << "Module name: " << module_name << std::endl;
    return module_name.contains("user32.dll");
}

int main() {
    while (!GetModuleHandleA("user32.dll"))
        LoadLibraryA("user32.dll");
    auto result = SafeCall::Type::Stdcall<int>((uintptr_t)check_return_address, SafeCall::Address::GetGadget("user32.dll"));
    return not result;
}