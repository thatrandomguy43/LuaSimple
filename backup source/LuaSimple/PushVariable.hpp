#include <utility>
#include <string>
#include "luainclude/lua.hpp"
#include <unordered_map>
#include <variant>
#include <any>
#include <optional>
#include "LuaTypeClasses.hpp"

#pragma once



class PushVariable
{
private:
    lua_State** pointer_to_lua_state;

public:
    PushVariable(lua_State**);

    void Nil();

    void Boolean(bool);

    void Number(lua_Number);

    void LightUserdata(void*);

    void String(std::string);

    void TableKeyAdder(std::variant<std::string, lua_Integer>);

    void Table(lua_Table, bool /*if not set will instead merge the keys and values into existing table on top of stack, presubably overriding any duplicate keys with new ones*/);

    // i dont really understand the purpose of c function upvalues, if you need to do stuff with other lua values for some reason just make them
    // this now works for at least basic funcs, yay
    void CFunction(lua_CFunction);

    void LuaFunction(lua_Function);

    // not doing uservalues quite yet
    void Userdata(lua_Userdata);

    void AnyValue(std::any);
};