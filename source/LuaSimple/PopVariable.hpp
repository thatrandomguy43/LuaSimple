#include <string>
#include <iostream>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"
#include <variant>
#include "LuaFunction.hpp"
#include <unordered_map>

#pragma once

typedef std::unordered_map<std::variant<std::string, lua_Integer>, std::any> lua_Table;

class PopVariable
{
private:
    lua_State** pointer_to_lua_state;

public:
    PopVariable(lua_State**);

    bool Boolean();

    void* LightUserdata();

    lua_Number Number();

    std::string String();

    lua_Table Table();

    lua_Function LuaFunction();

    lua_CFunction CFunction();

    std::any* Userdata();

    std::any AnyValue();
};
