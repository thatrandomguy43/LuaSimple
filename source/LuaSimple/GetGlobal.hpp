#include <memory>
#include <iostream>
#include <string>
#include "luainclude/lua.hpp"
#include "GetVariable.hpp"
#include "LuaFunction.hpp"

#pragma once

typedef std::unordered_map<std::variant<std::string, lua_Integer>, std::any> lua_Table;
class GetGlobal
{
private:
    lua_State** pointer_to_lua_state;
    GetVariable* get_variable;

public:
    GetGlobal(lua_State**, GetVariable*);

    int GetType(std::string);

    bool Boolean(std::string);

    void* LightUserdata(std::string);

    lua_Number Number(std::string);

    std::string String(std::string);

    lua_Table Table(std::string);

    LuaFunction Function(std::string);

    std::any* Userdata(std::string);
};
