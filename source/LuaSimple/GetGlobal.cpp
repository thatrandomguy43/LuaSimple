#include <memory>
#include <iostream>
#include <string>
#include "luainclude/lua.hpp"
#include "LuaInstance.hpp"
#include "GetVariable.hpp"
#include "GetGlobal.hpp"

using namespace std;

GetGlobal::GetGlobal(lua_State** pointer_passed, GetVariable* get_obj) : pointer_to_lua_state(pointer_passed), get_variable(get_obj){

}

int GetGlobal::GetType(string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    int type = lua_type(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return type;
}

bool GetGlobal::Boolean(string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TBOOLEAN){
       return get_variable->Boolean();
    } else {
        cerr << "Global Lua variable " << name << " is not a boolean." << endl;
        return false;
    };
    
}

void* GetGlobal::LightUserdata(string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TLIGHTUSERDATA){
        return get_variable->LightUserdata();
    } else {
        cerr << "Global Lua variable " << name << " is not a light userdata." << endl;
        return nullptr;
    };
   
}

lua_Number GetGlobal::Number(string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TNUMBER){
        return get_variable->Number();
    } else {
        cerr << "Global Lua variable " << name << " is not a number." << endl;
        return 0;
    };
}

string GetGlobal::String(string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    string found_global;
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TSTRING){
        return get_variable->String();
    } else {
        cerr << "Global Lua variable " << name << " is not a string." << endl;
        return "";
    };
}

lua_Table GetGlobal::Table(string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TTABLE){
        return get_variable->Table();
    } else {
        cerr << "Global Lua variable " << name << " is not a table." << endl;
        return {};
    };
}

tuple<int,int,bool> GetGlobal::Function(string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TFUNCTION){
        return get_variable->Function();
    } else {
        cerr << "Global Lua variable " << name << " is not a function." << endl;
        return {};
    };
}

any* GetGlobal::Userdata(string name){
lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TUSERDATA){
        return get_variable->Userdata();
    } else {
        cerr << "Global Lua variable " << name << " is not a userdata." << endl;
        return nullptr;
    };
}


