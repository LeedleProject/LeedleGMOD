#pragma once

#include "../hooks.hpp"
#include "../memory.hpp"
#include <concepts>
#include <loguru.hpp>

#include <vector>
#include <sstream>

namespace game {

template <class ReturnType = void, class ...Args>
struct WithCallbacks {

    std::vector<std::function<ReturnType(Args...)>> callbacks;
    auto add_callback(const std::invocable<ReturnType, Args...> auto& callback) {
        callbacks.emplace_back(std::move(callback));

        #ifdef _DEBUG
        std::stringstream ss;
        ((ss << typeid(Args).name() << ", "), ...);

        DLOG_S(INFO) << "Added callback: " << std::hex << std::hash<decltype(callback)>{}(callback)
            << " " << typeid(ReturnType).name() << "(" << ss << ")";
        #endif
    }

};

}