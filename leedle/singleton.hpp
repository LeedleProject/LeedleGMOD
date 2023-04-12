#pragma once

#include <memory>

template<class T>
class Singleton {
protected:
    Singleton() = default;
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T* get_pointer() {
        static T instance;
        return &instance;
    }

    static T& get() {
        return *get_pointer();
    }
};