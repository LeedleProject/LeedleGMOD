#include "gui.hpp"
#include "render.hpp"

#include <imgui.h>
#include <string_view>
#include <tuple>
#include <vector>

#include "game/interfaces/interface.hpp"

#include <loguru.hpp>

using namespace gui;
using namespace ImGui;
using namespace render;

constexpr std::string_view InterfaceReg_s_pattern = "48 8B 1D ? ? ? ? 48 8B FA 48 8B F1";

void GUI::render() {
    if (is_open()) {
        Begin("Leedle");

        End();
    }

    static std::vector<std::tuple<std::string, void*>> interfaces;
    if (interfaces.empty()) {
        auto first_reg = memory::MemoryScanner(InterfaceReg_s_pattern).
            scan<game::InterfaceReg*>(memory::client.begin(), memory::client.end()).absolute(0x3, 0x7);
        if (first_reg.get_pointer()) {
            for (auto* reg = *first_reg.get_pointer(); reg; reg = reg->next) {
                interfaces.emplace_back(std::make_tuple(reg->name, reg->create_function));
                LOG_S(INFO) << "Registered interface: " << std::get<0>(interfaces.back()) 
                    << " (" << std::hex << std::get<1>(interfaces.back()) << ")";
            }
        }
    }

    #ifdef _DEBUG

    Begin("Ну типо дебаг");

    Text("Build date: %s", __DATE__);
    Text("device: %p", RENDER.get_device());
    Text("hwnd: %p", RENDER.get_hwnd());

    End();

    Begin("Интерфейсы");
    if (BeginTable("interfacestable", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersOuterH)) {
        for (auto& [name, address] : interfaces) {
            TableNextRow();

            TableNextColumn();
            Text("%s", name.c_str());

            TableNextColumn();
            Text("%p", address);
        }
        EndTable();
    }
    End();

    #endif
}