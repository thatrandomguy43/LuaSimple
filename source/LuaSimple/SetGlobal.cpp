#include "luainclude/lua.hpp"
#include "PushVariable.hpp"
#include "LuaInstance.hpp"
#include "SetGlobal.hpp"
#include <string>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <any>
#include <variant>
#include <memory>
#include <optional>

using namespace std;

SetGlobal::SetGlobal(lua_State** pointer_passed, PushVariable* var_pusher_passed): pointer_to_lua_state(pointer_passed), variable_maker(var_pusher_passed)
{
}

void SetGlobal::Boolean(bool bool_to_set, string name)
{
    variable_maker->Boolean(bool_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return;
}

void SetGlobal::Number(lua_Number number_to_set, string name)
{
    variable_maker->Number(number_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return;
}

void SetGlobal::LightUserdata(void* pointer_to_set, string name)
{
    variable_maker->LightUserdata(pointer_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return;
}
void SetGlobal::String(string string_to_set, string name)
{
    variable_maker->String(string_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return;
}

void SetGlobal::Table(lua_Table table_to_set, string name, optional<string> metatable_name)
{
    variable_maker->Table(table_to_set, true, metatable_name);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return;
}

void SetGlobal::Metatable(lua_Table metatable_to_register, string name)
{
    if (luaL_getmetatable(*(this->pointer_to_lua_state), name.c_str()) == 0)
    {
        lua_pop(*(this->pointer_to_lua_state), 1);
        luaL_newmetatable(*(this->pointer_to_lua_state), name.c_str());
    };
    variable_maker->Table(metatable_to_register, false, {});
    lua_pop(*(this->pointer_to_lua_state), 1);
    return;
}

void SetGlobal::Function(lua_CFunction function_to_set, string name)
{
    variable_maker->Function(function_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return;
}

any* SetGlobal::Userdata(any data, string name, optional<string> metatable_name /*,int uservalue_count*/)
{
    any* userdata_ptr = variable_maker->Userdata(data, metatable_name);

    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return userdata_ptr;
}
