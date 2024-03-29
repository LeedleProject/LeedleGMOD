#include "leedle.hpp"

#include <Windows.h>
#include <consoleapi.h>
#include <corecrt_math.h>
#include <corecrt_startup.h>

#include <array>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <format>
#include <loguru.hpp>
#include <thread>

#include "gui.hpp"
#include "hooks.hpp"
#include "math.hpp"
#include "memory.hpp"
#include "render.hpp"
#include "input.hpp"
#include "hooks/hooks.hpp"
#include "features/features.hpp"

#include <MinHook.h>
#include <debugapi.h>
#include <handleapi.h>
#include <stdio.h>
#include <winuser.h>

auto leedle_terminate_handler() {
    LOG_S(INFO) << "Terminating..." << std::endl;

    try {
        std::rethrow_exception(std::current_exception());
    } catch (const std::exception& e) {
        LOG_S(ERROR) << e.what() << std::endl;
        auto tmp = MessageBoxA(nullptr, e.what(), "Runtime error", MB_OK);
    } catch (const std::runtime_error& err) {
        LOG_S(ERROR) << err.what() << std::endl;
        auto tmp = MessageBoxA(nullptr, err.what(), "Runtime error", MB_OK);
    } catch (...) {
        std::array<char, MAX_PATH> buffer;
        if (strerror_s(buffer.data(), buffer.size(), errno) != 0) {
            LOG_S(ERROR) << "Unknown error: " << errno << " : " << buffer.data()
                         << std::endl;
        }
    }

    loguru::flush();
    std::terminate();
}

auto allocate_console() {
    #if _DEBUG
    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
    #endif
}

void leedle::logger::initialize_logging() {
    allocate_console();

    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    loguru::init(argc, (char**)argv);
    loguru::g_preamble_file = false;
    loguru::g_preamble_thread = false;

    loguru::set_fatal_handler([](const auto& message) {
        throw std::runtime_error(std::format(
            "{}:[{}] {}",
            message.filename,
            message.prefix,
            message.message));
    });

    loguru::add_callback (
        "gamelogs",
        [](auto user_data, const loguru::Message& message) {
            if (message.verbosity >= loguru::Verbosity_INFO) {
                leedle::logger::game_message("[LEEDLE] {}", message.message);
            } else {
                leedle::logger::game_warning("[LEEDLE] {}", message.message);
            }
        },
        0,
        loguru::Verbosity_MAX
    );

    auto leedle_home = leedle::fs::get_leedle_root();
    if (not std::filesystem::exists(leedle_home)) {
        std::filesystem::create_directories(leedle_home);
    }

    auto log_path = leedle_home.append("leedle.log");
    loguru::add_file(
        log_path.string().c_str(),
        loguru::FileMode::Append,
        loguru::Verbosity_MAX);
}

auto __stdcall entry_point(HMODULE mod) {
    std::set_terminate(leedle_terminate_handler);

    leedle::logger::initialize_logging();

    leedle::Leedle::get().unload_function = [mod]() {
        game::shutdown_default_hooks();
        leedle::shutdown_modules(features::Features::get(), gui::GUI::get(), render::Render::get(), input::Input::get(), leedle::Leedle::get());

        FreeLibraryAndExitThread(mod, 0);
        // TODO: Unload the module.
    };

    CHECK_S(MH_Initialize() == MH_OK) << "Cannot initialize core staff 0x1";

    leedle::initialize_modules(leedle::Leedle::get(), input::Input::get(), render::Render::get(), gui::GUI::get(), features::Features::get());
    game::initialize_default_hooks();

    DLOG_S(INFO) << "Angle sizeof: " << sizeof(math::Angle);
}

bool __stdcall DllMain(
    HMODULE mod,
    DWORD ul_reason_for_call,
    LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        std::thread(entry_point, mod).detach();
    }
    return TRUE;
}

void my_init_function() __attribute__((constructor));

void my_init_function() {
    while (memory::MemoryModule::get_module_by_name("serverbrowser") == INVALID_HANDLE_VALUE) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}