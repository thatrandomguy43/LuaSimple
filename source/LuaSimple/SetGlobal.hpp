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

#pragma once

typedef std::unordered_map<std::variant<std::string, lua_Integer>, std::any> lua_Table;
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

    void Table(lua_Table, std::string, std::optional<std::string>);

    void Metatable(lua_Table, std::string);

    void Function(lua_CFunction, std::string);

    std::any* Userdata(std::any, std::string, std::optional<std::string>);
};
