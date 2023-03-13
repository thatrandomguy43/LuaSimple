#include <memory>
#include <iostream>
#include <string>
#include "luainclude/lua.hpp"
#include "LuaInstance.hpp"
#include "GetVariable.hpp"
#include "GetGlobal.hpp"

using namespace std;

GetGlobal::GetGlobal(lua_State** pointer_passed, GetVariable* get_obj): pointer_to_lua_state(pointer_passed),
get_variable(get_obj)
{
}

int GetGlobal::GetType(string name)
{
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    int type = lua_type(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return type;
}

bool GetGlobal::Boolean(string name)
{
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TBOOLEAN)
    {
        bool found_global = get_variable->Boolean();
        lua_pop(*(this->pointer_to_lua_state), 1);
        return found_global;
    }
    else
    {
        cerr << "Global Lua variable " << name << " is not a boolean." << endl;
        lua_pop(*(this->pointer_to_lua_state), 1);
        return false;
    };
}

void* GetGlobal::LightUserdata(string name)
{
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TLIGHTUSERDATA)
    {
        void* found_global = get_variable->LightUserdata();
        lua_pop(*(this->pointer_to_lua_state), 1);
        return found_global;
    }
    else
    {
        cerr << "Global Lua variable " << name << " is not a light userdata." << endl;
        lua_pop(*(this->pointer_to_lua_state), 1);
        return nullptr;
    };
}

lua_Number GetGlobal::Number(string name)
{
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TNUMBER)
    {
        lua_Number found_global = get_variable->Number();
        lua_pop(*(this->pointer_to_lua_state), 1);
        return found_global;
    }
    else
    {
        cerr << "Global Lua variable " << name << " is not a number." << endl;
        lua_pop(*(this->pointer_to_lua_state), 1);
        return 0;
    };
}

string GetGlobal::String(string name)
{
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    string found_global;
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TSTRING)
    {
        string found_global = get_variable->String();
        lua_pop(*(this->pointer_to_lua_state), 1);
        return found_global;
    }
    else
    {
        cerr << "Global Lua variable " << name << " is not a string." << endl;
        lua_pop(*(this->pointer_to_lua_state), 1);
        return "";
    };
}

lua_Table GetGlobal::Table(string name)
{
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TTABLE)
    {
        lua_Table found_global = get_variable->Table();
        lua_pop(*(this->pointer_to_lua_state), 1);
        return found_global;
    }
    else
    {
        cerr << "Global Lua variable " << name << " is not a table." << endl;
        lua_pop(*(this->pointer_to_lua_state), 1);
        return {};
    };
}

LuaFunction GetGlobal::Function(string name)
{
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TFUNCTION)
    {
        LuaFunction found_global = get_variable->Function();
        return found_global;
    }
    else
    {
        cerr << "Global Lua variable " << name << " is not a function." << endl;
        lua_pop(*(this->pointer_to_lua_state), 1);
        return {};
    };
}

any* GetGlobal::Userdata(string name)
{
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TUSERDATA)
    {
        any* found_global = get_variable->Userdata();
        lua_pop(*(this->pointer_to_lua_state), 1);
        return found_global;
    }
    else
    {
        cerr << "Global Lua variable " << name << " is not a userdata." << endl;
        lua_pop(*(this->pointer_to_lua_state), 1);
        return nullptr;
    };
}
