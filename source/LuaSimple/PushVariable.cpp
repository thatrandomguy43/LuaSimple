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

using namespace std;

PushVariable::PushVariable(lua_State **pointer_passed) : pointer_to_lua_state(pointer_passed)
{
}

void PushVariable::Nil()
{
    lua_pushnil(*(this->pointer_to_lua_state));
    return;
}

void PushVariable::Boolean(bool boolean)
{
    lua_pushboolean(*(this->pointer_to_lua_state), boolean);
    return;
}

void PushVariable::Number(lua_Number number)
{
    lua_pushnumber(*(this->pointer_to_lua_state), number);
    return;
}

void PushVariable::LightUserdata(void *pointer)
{
    lua_pushlightuserdata(*(this->pointer_to_lua_state), pointer);
    return;
}

void PushVariable::String(string string)
{
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

void PushVariable::Table(lua_Table table_to_add, bool make_new /*if not set will instead merge the keys and values into existing table on top of stack, presubably overriding any duplicate keys with new ones*/, optional<string> metatable_name)
{
    if (make_new)
    {
        lua_newtable(*(this->pointer_to_lua_state));
    };
    for (auto itr = table_to_add.begin(); itr != table_to_add.end(); itr++)
    {
        // if there's nothing in the any do nothing
        if (itr->second.has_value())
        {
            // boolean
            if (itr->second.type() == typeid(bool))
            {
                Boolean(any_cast<bool>(itr->second));
                TableKeyAdder(itr->first);
            }
            // number
            else if (itr->second.type() == typeid(lua_Number))
            {
                Number(any_cast<lua_Number>(itr->second));
                TableKeyAdder(itr->first);
            }
            // light userdata
            else if (itr->second.type() == typeid(void *))
            {
                LightUserdata(any_cast<void *>(itr->second));
                TableKeyAdder(itr->first);
            }
            // string
            else if (itr->second.type() == typeid(string))
            {
                String(any_cast<string>(itr->second));
                TableKeyAdder(itr->first);
            }
            else if (itr->second.type() == typeid(lua_CFunction))
            {
                Function(any_cast<lua_CFunction>(itr->second));
                TableKeyAdder(itr->first);
            }
            // nil
            else
            {
                lua_pushnil(*(this->pointer_to_lua_state));
                TableKeyAdder(itr->first);
            };
            // full userdata would not work as you need to get a pointer to its location in lua back
        };
    };
    if (metatable_name.has_value())
    {
        luaL_setmetatable(*(this->pointer_to_lua_state), metatable_name.value().c_str());
    }
    return;
}

// i dont really understand the purpose of c function upvalues, if you need to do stuff with other lua values for some reason just make them
// this now works for at least basic funcs, yay
void PushVariable::Function(lua_CFunction function)
{
    lua_pushcfunction(*(this->pointer_to_lua_state), function);
}

// not doing uservalues quite yet
any *PushVariable::Userdata(any data, optional<string> metatable_name /*,int uservalue_count*/)
{
    void *userdata_ptr = lua_newuserdatauv(*(this->pointer_to_lua_state), sizeof data, 0);
    if (metatable_name.has_value())
    {
        luaL_setmetatable(*(this->pointer_to_lua_state), metatable_name.value().c_str());
    }
    memmove(userdata_ptr, &data, sizeof data);
    return (any *)userdata_ptr;
}
