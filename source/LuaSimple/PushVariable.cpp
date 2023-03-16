#include <utility>
#include <string>
#include "luainclude/lua.hpp"
#include <unordered_map>
#include <variant>
#include <any>
#include <optional>
#include <iostream>
#include <cstring>
#include "LuaInstance.hpp"
#include "PushVariable.hpp"
#include "LuaTypeClasses.hpp"


using namespace std;

PushVariable::PushVariable(lua_State** pointer_passed): pointer_to_lua_state(pointer_passed)
{
}

void PushVariable::Nil()
{
    luaL_checkstack(*(this->pointer_to_lua_state), 1, "Error: cannot push additional variable to Lua. The Lua stack size limit has been reached, or the program is out of memory.");
    lua_pushnil(*(this->pointer_to_lua_state));
    return;
}

void PushVariable::Boolean(bool boolean)
{
    luaL_checkstack(*(this->pointer_to_lua_state), 1, "Error: cannot push additional variable to Lua. The Lua stack size limit has been reached, or the program is out of memory.");
    lua_pushboolean(*(this->pointer_to_lua_state), boolean);
    return;
}

void PushVariable::Number(lua_Number number)
{
    luaL_checkstack(*(this->pointer_to_lua_state), 1, "Error: cannot push additional variable to Lua. The Lua stack size limit has been reached, or the program is out of memory.");
    lua_pushnumber(*(this->pointer_to_lua_state), number);
    return;
}

void PushVariable::LightUserdata(void* pointer)
{
    luaL_checkstack(*(this->pointer_to_lua_state), 1, "Error: cannot push additional variable to Lua. The Lua stack size limit has been reached, or the program is out of memory.");
    lua_pushlightuserdata(*(this->pointer_to_lua_state), pointer);
    return;
}

void PushVariable::String(string string)
{
    luaL_checkstack(*(this->pointer_to_lua_state), 1, "Error: cannot push additional variable to Lua. The Lua stack size limit has been reached, or the program is out of memory.");
    lua_pushlstring(*(this->pointer_to_lua_state), string.c_str(), string.size());
    return;
}

void PushVariable::TableKeyAdder(variant<string, lua_Integer> key)
{
    // number index or string index?
    if (holds_alternative<lua_Integer>(key))
    {
        lua_seti(*(this->pointer_to_lua_state), -2, get<lua_Integer>(key));
    }
    else if (holds_alternative<string>(key))
    {
        lua_setfield(*(this->pointer_to_lua_state), -2, (get<string>(key)).c_str());
    }
    else
    { // the variant is sometimes empty so in that case remove the value manually to clean up
        lua_pop(*(this->pointer_to_lua_state), 1);
    };
    return;
}

void PushVariable::Table(lua_Table table_to_add, bool make_new /*if not set will instead merge the keys and values into existing table on top of stack, presubably overriding any duplicate keys with new ones*/)
{
    if (make_new)
    {
        luaL_checkstack(*(this->pointer_to_lua_state), 1, "Error: cannot push additional variable to Lua. The Lua stack size limit has been reached, or the program is out of memory.");
        lua_newtable(*(this->pointer_to_lua_state));
    };
    for (auto itr = table_to_add.table_contents.begin(); itr != table_to_add.table_contents.end(); itr++)
    {
        this->AnyValue(itr->second);
        this->TableKeyAdder(itr->first);
    };
    if (table_to_add.metatable_name.has_value())
    {
        luaL_setmetatable(*(this->pointer_to_lua_state), table_to_add.metatable_name.value().c_str());
    };
    return;
}

// i dont really understand the purpose of c function upvalues, if you need to do stuff with other lua values for some reason just make them
// this now works for at least basic funcs, yay
void PushVariable::CFunction(lua_CFunction function)
{
    luaL_checkstack(*(this->pointer_to_lua_state), 1, "Error: cannot push additional variable to Lua. The Lua stack size limit has been reached, or the program is out of memory.");
    lua_pushcfunction(*(this->pointer_to_lua_state), function);
    return;
}

void PushVariable::LuaFunction(lua_Function function){
    this->Number(function.registry_key);
    lua_gettable(*(this->pointer_to_lua_state), LUA_REGISTRYINDEX);
    return;
}


lua_Userdata PushVariable::Userdata(lua_Userdata data)
{
    luaL_checkstack(*(this->pointer_to_lua_state), 1, "Error: cannot push additional variable to Lua. The Lua stack size limit has been reached, or the program is out of memory.");
    void* userdata_ptr = lua_newuserdatauv(*(this->pointer_to_lua_state), sizeof data, 0);
    if (data.metatable_name.has_value())
    {
        luaL_setmetatable(*(this->pointer_to_lua_state), data.metatable_name.value().c_str());
    }
    memmove(userdata_ptr, &data, sizeof data);
    lua_Userdata return_obj;
    return_obj.object = (any*)userdata_ptr;
    return_obj.metatable_name = data.metatable_name;
    return return_obj;
}

void PushVariable::AnyValue(any value) {
    if (value.has_value())
    {
        if (value.type() == typeid(bool))
        {
            this->Boolean(any_cast<bool>(value));
        }
        else if (value.type() == typeid(lua_Number))
        {
            this->Number(any_cast<lua_Number>(value));
        }
        else if (value.type() == typeid(void*))
        {
            this->LightUserdata(any_cast<void*>(value));
        }
        else if (value.type() == typeid(string))
        {
            this->String(any_cast<string>(value));
        }
        else if (value.type() == typeid(lua_Table))
        {
            this->Table(any_cast<lua_Table>(value), false);
        }
        else if (value.type() == typeid(lua_Function))
        {
            this->LuaFunction(any_cast<lua_Function>(value));
        }
        else if (value.type() == typeid(lua_CFunction))
        {
            this->CFunction(any_cast<lua_CFunction>(value));
        }
        else
        {
            this->Nil();
        };
        // full userdata would not work as you need to get a pointer to its location in lua back
    };
}