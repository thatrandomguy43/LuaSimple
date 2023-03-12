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

int LuaInstance::DoString(string code)
{
    int response_code = luaL_dostring(this->pointer_to_lua_state, code.c_str());
    if (response_code != 0)
    {
        string error_message = luaL_tolstring(this->pointer_to_lua_state, -1, NULL);
        cerr << error_message;
    };
    return response_code;
}

int LuaInstance::DoFile(string filename)
{
    int response_code = luaL_dofile(this->pointer_to_lua_state, filename.c_str());
    if (response_code != 0)
    {
        string error_message = luaL_tolstring(this->pointer_to_lua_state, -1, NULL);
        cerr << error_message;
    }
    else
    {
    };
    return response_code;
}

vector<any> LuaInstance::GetArguments(vector<int> types)
{
    vector<any> argument_values;
    int num_arguments = lua_gettop(this->pointer_to_lua_state);
    argument_values.resize(num_arguments);
    for (int argument = num_arguments; argument != 0; argument--)
    {
        // only do a check if there is actualy a value to check against, extra args can be whatever and just go unused anyway
        if (argument <= types.size())
        {
            luaL_checktype(this->pointer_to_lua_state, argument, types[(argument - 1)]);
        };

        lua_pop(this->pointer_to_lua_state, 1);
    }
    return argument_values;
}

std::any LuaInstance::GetValue()
{
    switch (lua_type(this->pointer_to_lua_state, -1))
    {
    case LUA_TNIL:
        return make_any<nullptr_t>(nullptr);
        break;
    case LUA_TBOOLEAN:
    {
        bool value = this->GetVariable.Boolean();
        return make_any<bool>(value);
    }
    break;

    case LUA_TLIGHTUSERDATA:
    {
        void* value = this->GetVariable.LightUserdata();
        return make_any<void*>(value);
    }
    break;

    case LUA_TNUMBER:
    {
        lua_Number value = this->GetVariable.Number();
        return make_any<lua_Number>(value);
    }
    break;

    case LUA_TSTRING:
    {
        string value = this->GetVariable.String();
        return make_any<string>(value);
    }
    break;

    case LUA_TTABLE:
    {
        lua_Table value = this->GetVariable.Table();
        return make_any<lua_Table>(value);
    }
    break;
    case LUA_TFUNCTION:
    {
        LuaFunction value = this->GetVariable.Function();
        return make_any<LuaFunction>(value);
    }
    break;
    case LUA_TUSERDATA:
    {
        any* value = this->GetVariable.Userdata();
        return make_any<any*>(value);
    }
    break;

    default:
        return make_any<nullptr_t>(nullptr);
        break;
    }
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
