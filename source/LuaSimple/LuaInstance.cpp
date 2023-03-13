#include "luainclude/lua.hpp"
#include <string>
#include <iostream>
#include "SetGlobal.hpp"
#include "PushVariable.hpp"
#include "GetGlobal.hpp"
#include <vector>
#include <any>
#include "GetVariable.hpp"
#include "LuaInstance.hpp"
#include "LuaFunction.hpp"

using namespace std;

LuaInstance::LuaInstance(): pointer_to_lua_state(luaL_newstate()),
PushVariable(&pointer_to_lua_state),
GetVariable(&pointer_to_lua_state),
SetGlobal(&pointer_to_lua_state, &PushVariable),
GetGlobal(&pointer_to_lua_state, &GetVariable)
{
    luaL_openlibs(this->pointer_to_lua_state);
    this->instance_list[this->pointer_to_lua_state] = this;
}

LuaInstance::~LuaInstance()
{
    lua_close(this->pointer_to_lua_state);
    this->instance_list.erase(this->pointer_to_lua_state);
}

int LuaInstance::DoFunction(LuaFunction function_object, vector<any> arguments){
    this->PushVariable.Number(function_object.registry_key);
    lua_gettable(this->pointer_to_lua_state, LUA_REGISTRYINDEX);
    for (auto itr = arguments.begin(); itr != arguments.end(); itr++){
        this->PushVariable.AnyValue(*itr);
    };
    int response_code = lua_pcall(this->pointer_to_lua_state, function_object.argument_count, LUA_MULTRET, 0);
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

void LuaInstance::HandleReturn(int response){
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
        for (int return_index = num_returns-1; return_index >= 0; return_index--){
            this->lua_return_values[return_index] = this->GetVariable.AnyValue();
        };
    };
    return;
}

void LuaInstance::GetArguments(vector<int> types)
{

    int num_arguments = lua_gettop(this->pointer_to_lua_state);
    this->lua_argument_values.resize(num_arguments);
    for (int argument_index = num_arguments-1; argument_index >= 0; argument_index--)
    {
        // only do a check if there is actualy a value to check against, extra args can be whatever and just go unused anyway
        if (argument_index < types.size())
        {
            luaL_checktype(this->pointer_to_lua_state, argument_index+1, types[(argument_index)]);
        };

        this->lua_argument_values[argument_index] = this->GetVariable.AnyValue();
        lua_pop(this->pointer_to_lua_state, 1);
    }
    return;
}



void LuaInstance::ReturnResults(vector<any> values)
{
    for (auto itr = values.begin(); itr != values.end(); itr++)
    {
        // if there's nothing in the any do nothing
        if (itr->has_value())
        {
            // boolean
            if (itr->type() == typeid(bool))
            {
                this->PushVariable.Boolean(any_cast<bool>(*itr));
            }
            // number
            else if (itr->type() == typeid(lua_Number))
            {
                this->PushVariable.Number(any_cast<lua_Number>(*itr));
            }
            // light userdata
            else if (itr->type() == typeid(void*))
            {
                this->PushVariable.LightUserdata(any_cast<void*>(*itr));
            }
            // string
            else if (itr->type() == typeid(string))
            {
                this->PushVariable.String(any_cast<string>(*itr));
            }
            else if (itr->type() == typeid(lua_CFunction))
            {
                this->PushVariable.Function(any_cast<lua_CFunction>(*itr));
            }
            // nil
            else
            {
                lua_pushnil(this->pointer_to_lua_state);
            };
            // full userdata would not work as you need to get a pointer to its location in lua back
        };
    };
}

LuaInstance& LuaInstance::FindInstance(lua_State* pointer_from_lua)
{
    LuaInstance& found_instance = *(LuaInstance::instance_list[pointer_from_lua]);
    return found_instance;
}
