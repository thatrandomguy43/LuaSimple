#include <memory>
#include <iostream>
#include <string>
#include "luainclude/lua.hpp"
#include "PopVariable.hpp"
#include "LuaFunction.hpp"
#include <unordered_map>
#include <any>
#include <variant>

#pragma once

typedef std::unordered_map<std::variant<std::string, lua_Integer>, std::any> lua_Table;
class GetGlobal
{
private:
    lua_State** pointer_to_lua_state;
    PopVariable* get_variable;

public:
    GetGlobal(lua_State**, PopVariable*);

    int GetType(std::string);

    bool Boolean(std::string);

    void* LightUserdata(std::string);

    lua_Number Number(std::string);

    std::string String(std::string);

    lua_Table Table(std::string);

    lua_Function LuaFunction(std::string);

    lua_CFunction CFunction(std::string);

    std::any* Userdata(std::string);

    std::any AnyValue(std::string);
};
