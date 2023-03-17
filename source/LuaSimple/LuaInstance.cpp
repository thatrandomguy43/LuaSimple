#include "luainclude/lua.hpp"
#include <string>
#include <iostream>
#include "SetGlobal.hpp"
#include "PushVariable.hpp"
#include "GetGlobal.hpp"
#include <vector>
#include <any>
#include "PopVariable.hpp"
#include "LuaInstance.hpp"
#include "LuaTypeClasses.hpp"

using namespace std;

LuaInstance::LuaInstance(): pointer_to_lua_state(luaL_newstate()),
PushVariable(&pointer_to_lua_state),
PopVariable(&pointer_to_lua_state),
SetGlobal(&pointer_to_lua_state, &PushVariable),
GetGlobal(&pointer_to_lua_state, &PopVariable)
{
    luaL_openlibs(this->pointer_to_lua_state);
    this->instance_list[this->pointer_to_lua_state] = this;
}

LuaInstance::~LuaInstance()
{
    lua_close(this->pointer_to_lua_state);
    this->instance_list.erase(this->pointer_to_lua_state);
}

int LuaInstance::DoFunction(lua_Function function_object, vector<any> arguments) {

    this->PushVariable.Number(function_object.registry_key);
    lua_gettable(this->pointer_to_lua_state, LUA_REGISTRYINDEX);
    for (auto itr = arguments.begin(); itr != arguments.end(); itr++) {
        this->PushVariable.AnyValue(*itr);
    };
    int response_code = lua_pcall(this->pointer_to_lua_state, arguments.size(), LUA_MULTRET, 0);
    this->HandleReturn(response_code);
    return response_code;
}

int LuaInstance::DoString(string code)
{
    int response_code = luaL_dostring(this->pointer_to_lua_state, code.c_str());
    this->HandleReturn(response_code);
    return response_code;
}

int LuaInstance::DoFile(string filename)
{
    int response_code = luaL_dofile(this->pointer_to_lua_state, filename.c_str());
    this->HandleReturn(response_code);
    return response_code;
}

void LuaInstance::HandleReturn(int response) {
    if (response != 0)
    {
        this->lua_return_values.resize(1);
        this->lua_return_values[0] = static_cast<string>(luaL_tolstring(this->pointer_to_lua_state, -1, NULL));
        cerr << any_cast<string>(lua_return_values[0]) << endl;
    }
    else
    {
        int num_returns = lua_gettop(this->pointer_to_lua_state);
        this->lua_return_values.resize(num_returns);
        for (int return_index = num_returns - 1; return_index >= 0; return_index--) {
            this->lua_return_values[return_index] = this->PopVariable.AnyValue();
        };
    };
    return;
}

void LuaInstance::GetArguments(vector<int> types)
{

    int num_arguments = lua_gettop(this->pointer_to_lua_state);
    this->lua_argument_values.resize(num_arguments);
    for (int argument_index = num_arguments - 1; argument_index >= 0; argument_index--)
    {
        // only do a check if there is actualy a value to check against, extra args can be whatever and just go unused anyway
        if (argument_index < types.size())
        {
            luaL_checktype(this->pointer_to_lua_state, argument_index + 1, types[(argument_index)]);
        };

        this->lua_argument_values[argument_index] = this->PopVariable.AnyValue();
        lua_pop(this->pointer_to_lua_state, 1);
    }
    return;
}



void LuaInstance::ReturnResults(vector<any> values)
{
    for (auto itr = values.begin(); itr != values.end(); itr++)
    {
        this->PushVariable.AnyValue(*itr);
    };
}

LuaInstance& LuaInstance::FindInstance(lua_State* pointer_from_lua)
{
    LuaInstance& found_instance = *(LuaInstance::instance_list[pointer_from_lua]);
    return found_instance;
}

lua_Value LuaInstance::PopValue() {
    lua_Value popped;
    switch (lua_type(this->pointer_to_lua_state, -1))
    {
    case LUA_TNIL:
        popped = nullptr;
        break;
    case LUA_TBOOLEAN:
    {
        popped = static_cast<bool>(lua_toboolean(this->pointer_to_lua_state, -1));
    }
    break;

    case LUA_TLIGHTUSERDATA:
    {
        popped = lua_touserdata(this->pointer_to_lua_state, -1);

    }
    break;

    case LUA_TNUMBER:
    {
        if (lua_isinteger(this->pointer_to_lua_state, -1))
        {
            popped = lua_tointeger(this->pointer_to_lua_state, -1);
        }
        else
        {
            popped = lua_tonumber(this->pointer_to_lua_state, -1);
        };
    }
    break;

    case LUA_TSTRING:
    {
        popped = lua_tostring(this->pointer_to_lua_state, -1);
    }
    break;

    case LUA_TTABLE:
    {

    }
    break;
    case LUA_TFUNCTION:
    {
        if (lua_iscfunction(this->pointer_to_lua_state, -1))
        {
            popped = lua_tocfunction(this->pointer_to_lua_state, -1);
        }
        else
        {
            lua_Debug info_getter;
            lua_pushvalue(this->pointer_to_lua_state, -1);
            lua_getinfo(this->pointer_to_lua_state, "u", &info_getter);
            lua_Function func;
            func.argument_count = info_getter.nparams;
            func.takes_extra_args = info_getter.isvararg;
            lua_pushvalue(this->pointer_to_lua_state, -1); //both of these are because getinfo and ref pop the stack, so i need to counteract that
            func.registry_key = luaL_ref(this->pointer_to_lua_state, LUA_REGISTRYINDEX);
            popped = func;
        };
    };
    break;
    case LUA_TUSERDATA:
    {
        lua_Userdata udata;
        if (luaL_getmetafield(this->pointer_to_lua_state, -1, "__name") != LUA_TNIL){
            udata.metatable_name = get<string>(this->PopValue());
        };
        udata.object = static_cast<any*>(lua_touserdata(this->pointer_to_lua_state, -1));
    }
    break;
    };
    lua_pop(this->pointer_to_lua_state, 1);
    return popped;
}