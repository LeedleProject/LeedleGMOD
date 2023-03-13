#pragma once

#include <stdint.h>
#include "interface.hpp"

#include "../types/luainterface.hpp"

namespace game {

struct LuaShared : public Interface<LuaShared> {
	virtual void destr1() = 0;
	virtual void init(void* (*)(char const*, int*), bool, void*, void*) = 0;
	virtual void shutdown(void) = 0;
	virtual void dump_stats(void) = 0;
	virtual ILuaInterface* create_lua_interface(uint8_t, bool) = 0;
	virtual void close_lua_interface(void*) = 0;
	virtual ILuaInterface* get_lua_interface(unsigned char) = 0;
	virtual void load_file(std::string const&, std::string const&, bool, bool) = 0;
	virtual void get_cache(std::string const&) = 0;
	virtual void mount_lua(char const*) = 0;
	virtual void mount_lua_add(char const*, char const*) = 0;
	virtual void un_mount_lua(char const*) = 0;
	virtual void set_file_contents(char const*, char const*) = 0;
	virtual void set_lua_find_hook(void*) = 0;
	virtual void find_scripts(std::string const&, std::string const&, std::vector<int>&) = 0;
	virtual void get_stack_traces(void) = 0;
	virtual void invalidate_cache(std::string const&) = 0;
	virtual void empty_cache(void) = 0;

    static auto capture_interface() {
        return create_interface(memory::lua_shared, "LUASHARED003");
    }

};

DEFINE_INTERFACE(LuaShared, LUASHARED);
}