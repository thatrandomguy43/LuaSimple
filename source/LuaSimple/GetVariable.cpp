#include <string>
#include <iostream>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"
#include <variant>
#include "LuaInstance.hpp"
#include "GetVariable.hpp"

using namespace std;

GetVariable::GetVariable(lua_State** pointer_passed): pointer_to_lua_state(pointer_passed)
{
}

bool GetVariable::Boolean()
{
    bool found_global = lua_toboolean(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found_global;
}

void* GetVariable::LightUserdata()
{
    void* found_global = lua_touserdata(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found_global;
}

lua_Number GetVariable::Number()
{
    lua_Number found_global = lua_tonumber(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found_global;
}

string GetVariable::String()
{
    string found_global = lua_tostring(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found_global;
}

lua_Table GetVariable::Table()
{
    lua_Table return_table;
    variant<string, lua_Integer> key_to_add;
    any value_to_add;
    lua_pushnil(*(this->pointer_to_lua_state));
    while (lua_next(*(this->pointer_to_lua_state), -2) != 0)
    {
        if (lua_type(*(this->pointer_to_lua_state), -2) == LUA_TNUMBER)
        {
            key_to_add = lua_tointeger(*(this->pointer_to_lua_state), -2);
        }
        else if (lua_type(*(this->pointer_to_lua_state), -2) == LUA_TSTRING)
        {
            key_to_add = lua_tostring(*(this->pointer_to_lua_state), -2);
        };
        // we do a little copy pasting
        switch (lua_type(*(this->pointer_to_lua_state), -1))
        {

        case LUA_TNIL:
            value_to_add = nullptr;
        break;
        case LUA_TBOOLEAN:
        {
            value_to_add = this->Boolean();
        }
        break;

        case LUA_TLIGHTUSERDATA:
        {
            value_to_add = this->LightUserdata();
        }
        break;

        case LUA_TNUMBER:
        {
            value_to_add = this->Number();
        }
        break;

        case LUA_TSTRING:
        {
            value_to_add = this->String();
        }
        break;

        case LUA_TTABLE:
        { // when the function is recursive! susjerma.jpg
            value_to_add = this->Table();
        }
        break;

        case LUA_TFUNCTION:
        {
            value_to_add = this->Function();
        }
        break;

        case LUA_TUSERDATA:
        {
            value_to_add = this->Userdata();
        }
        break;

        default:
            value_to_add = nullptr;
            break;
        }
        return_table.insert({ key_to_add, value_to_add });
    };
    lua_pop(*(this->pointer_to_lua_state), 1);
    return return_table;
}

// so about this... i just learned what the lua registry actually is, and god damn do i feel like a moron
LuaFunction GetVariable::Function()
{
    lua_Debug debug;
    // i have to copy the func first as this getinfo pops it
    lua_pushvalue(*(this->pointer_to_lua_state), -1);
    lua_getinfo(*(this->pointer_to_lua_state), ">uS", &debug);
    LuaFunction func;
    // this fuction adds the function to a table (here the registry) with a guarenteed unique key
    func.registry_key = luaL_ref(*(this->pointer_to_lua_state), LUA_REGISTRYINDEX);
    func.argument_count = debug.nparams;
    func.takes_extra_args = debug.isvararg;
    return func;
}

any* GetVariable::Userdata()
{
    any* found_global = (any*)lua_touserdata(*(this->pointer_to_lua_state), -1);
    return found_global;
}

std::any GetVariable::AnyValue()
{
    switch (lua_type(*(this->pointer_to_lua_state), -1))
    {
    case LUA_TNIL:
        return make_any<nullptr_t>(nullptr);
        break;
    case LUA_TBOOLEAN:
    {
        bool value = this->Boolean();
        return make_any<bool>(value);
    }
    break;

    case LUA_TLIGHTUSERDATA:
    {
        void* value = this->LightUserdata();
        return make_any<void*>(value);
    }
    break;

    case LUA_TNUMBER:
    {
        lua_Number value = this->Number();
        return make_any<lua_Number>(value);
    }
    break;

    case LUA_TSTRING:
    {
        string value = this->String();
        return make_any<string>(value);
    }
    break;

    case LUA_TTABLE:
    {
        lua_Table value = this->Table();
        return make_any<lua_Table>(value);
    }
    break;
    case LUA_TFUNCTION:
    {
        LuaFunction value = this->Function();
        return make_any<LuaFunction>(value);
    }
    break;
    case LUA_TUSERDATA:
    {
        any* value = this->Userdata();
        return make_any<any*>(value);
    }
    break;

    default:
        return make_any<nullptr_t>(nullptr);
        break;
    }
}