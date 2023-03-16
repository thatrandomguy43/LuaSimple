#include "luainclude/lua.hpp"
#include "PushVariable.hpp"
#include <string>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <any>
#include <variant>
#include <memory>
#include <optional>
#include "LuaTypeClasses.hpp"
#pragma once


class SetGlobal
{
private:
    lua_State** pointer_to_lua_state;
    PushVariable* variable_maker;

public:
    SetGlobal(lua_State**, PushVariable*);

    void Boolean(bool, std::string);

    void Number(double, std::string);

    void LightUserdata(void*, std::string);

    void String(std::string, std::string);

    void Table(lua_Table, std::string);

    void Metatable(lua_Table, std::string);

    void LuaFunction(lua_Function, std::string);

    void CFunction(lua_CFunction, std::string);

    lua_Userdata Userdata(lua_Userdata, std::string);
};
