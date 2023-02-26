#include <memory>
#include <iostream>
#include <string>
#include "luainclude/lua.hpp"
class GetGlobal{
private:
lua_State** pointer_to_lua_state;
public:

GetGlobal(lua_State** L){
    this->pointer_to_lua_state = L;
}

int GetType(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    int type = lua_type(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return type;
}

bool Boolean(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    bool found_global;
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TBOOLEAN){
        found_global = lua_toboolean(*(this->pointer_to_lua_state),-1);
    } else {
        std::cerr << "Global Lua variable " << name << "i s not a boolean." << std::endl;
    };
    return found_global;
}

void* LightUserdata(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    void* found_global;
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TLIGHTUSERDATA){
        found_global = lua_touserdata(*(this->pointer_to_lua_state),-1);
    } else {
        std::cerr << "Global Lua variable " << name << " is not a light userdata." << std::endl;
    };
    return found_global;
}

lua_Number Number(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    lua_Number found_global;
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TNUMBER){
        found_global = lua_tonumber(*(this->pointer_to_lua_state),-1);
    } else {
        std::cerr << "Global Lua variable " << name << " is not a number." << std::endl;
    };
    return found_global;
}

std::string String(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    std::string found_global;
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TSTRING){
        found_global = lua_tostring(*(this->pointer_to_lua_state),-1);
    } else {
        std::cerr << "Global Lua variable " << name << "i s not a string." << std::endl;
    };
    return found_global;
}



};

