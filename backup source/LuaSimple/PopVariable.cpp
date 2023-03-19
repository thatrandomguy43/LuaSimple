#include <string>
#include <iostream>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"
#include <variant>
#include "LuaInstance.hpp"
#include "PopVariable.hpp"
#include "LuaTypeClasses.hpp"

using namespace std;

PopVariable::PopVariable(lua_State** pointer_passed): pointer_to_lua_state(pointer_passed)
{
}

bool PopVariable::Boolean()
{
    bool found = lua_toboolean(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found;
}

void* PopVariable::LightUserdata()
{
    void* found = lua_touserdata(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found;
}

lua_Number PopVariable::Number()
{
    lua_Number found = lua_tonumber(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found;
}

string PopVariable::String()
{
    string found = lua_tostring(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found;
}

lua_Table PopVariable::Table()
{
    lua_Table found;
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
        value_to_add = this->AnyValue();
        found.table_contents.insert({ key_to_add, value_to_add });
    };
    if (luaL_getmetafield(*(this->pointer_to_lua_state), -1, "__name")){
        found.metatable_name = this->String();
    }
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found;
}

// so about this... i just learned what the lua registry actually is, and god damn do i feel like a moron
lua_Function PopVariable::LuaFunction()
{
    lua_Debug debug;
    // i have to copy the func first as this getinfo pops it
    lua_pushvalue(*(this->pointer_to_lua_state), -1);
    lua_getinfo(*(this->pointer_to_lua_state), ">uS", &debug);
    lua_Function func;
    // this fuction adds the function to a table (here the registry) with a guarenteed unique key
    func.registry_key = luaL_ref(*(this->pointer_to_lua_state), LUA_REGISTRYINDEX);
    func.argument_count = debug.nparams;
    func.takes_extra_args = debug.isvararg;
    return func;
}

lua_CFunction PopVariable::CFunction(){
    lua_CFunction found = lua_tocfunction(*(this->pointer_to_lua_state),-1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return found;
}

lua_Userdata PopVariable::Userdata()
{
    lua_Userdata found;
    found.object = (any*)lua_touserdata(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
     if (luaL_getmetafield(*(this->pointer_to_lua_state), -1, "__name")){
        found.metatable_name = this->String();
    }
    return found;
}

std::any PopVariable::AnyValue()
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
        if (lua_tocfunction(*(this->pointer_to_lua_state),-1) == NULL){
            lua_Function value = this->LuaFunction();
            return make_any<lua_Function>(value);
        } else {
            lua_CFunction value = this->CFunction();
            return make_any<lua_CFunction>(value);
        };
    }
    break;
    case LUA_TUSERDATA:
    {
        lua_Userdata value = this->Userdata();
        return make_any<lua_Userdata>(value);
    }
    break;

    default:
        return make_any<nullptr_t>(nullptr);
        break;
    }
}