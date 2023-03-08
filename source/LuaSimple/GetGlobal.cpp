#include <memory>
#include <iostream>
#include <string>
#include "luainclude/lua.hpp"
#include "LuaInstance.hpp"
#include "GetVariable.hpp"
#include "GetGlobal.hpp"

GetGlobal::GetGlobal(lua_State** L, GetVariable* get_obj){
    this->pointer_to_lua_state = L;
    this->get_variable = get_obj;
}

int GetGlobal::GetType(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    int type = lua_type(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return type;
}

bool GetGlobal::Boolean(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TBOOLEAN){
       return get_variable->Boolean();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a boolean." << std::endl;
        return false;
    };
    
}

void* GetGlobal::LightUserdata(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TLIGHTUSERDATA){
        return get_variable->LightUserdata();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a light userdata." << std::endl;
        return nullptr;
    };
   
}

lua_Number GetGlobal::Number(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TNUMBER){
        return get_variable->Number();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a number." << std::endl;
        return 0;
    };
}

std::string GetGlobal::String(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    std::string found_global;
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TSTRING){
        return get_variable->String();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a string." << std::endl;
        return "";
    };
}

lua_Table GetGlobal::Table(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TTABLE){
        return get_variable->Table();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a table." << std::endl;
        return {};
    };
}

std::tuple<int,int,bool> GetGlobal::Function(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TFUNCTION){
        return get_variable->Function();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a function." << std::endl;
        return {};
    };
}

std::any* GetGlobal::Userdata(std::string name){
lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TUSERDATA){
        return get_variable->Userdata();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a userdata." << std::endl;
        return nullptr;
    };
}


