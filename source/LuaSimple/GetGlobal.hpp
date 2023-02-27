#include <memory>
#include <iostream>
#include <string>
#include "luainclude/lua.hpp"
#include "GetVariable.hpp"
class GetGlobal{
private:
lua_State** pointer_to_lua_state;
GetVariable* get_variable;
public:

GetGlobal(lua_State** L, GetVariable* get_obj){
    this->pointer_to_lua_state = L;
    this->get_variable = get_obj;
}

int GetType(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    int type = lua_type(*(this->pointer_to_lua_state), -1);
    lua_pop(*(this->pointer_to_lua_state), 1);
    return type;
}

bool Boolean(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TBOOLEAN){
       return get_variable->Boolean();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a boolean." << std::endl;
        return false;
    };
    
}

void* LightUserdata(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TLIGHTUSERDATA){
        return get_variable->LightUserdata();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a light userdata." << std::endl;
        return nullptr;
    };
   
}

lua_Number Number(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TNUMBER){
        return get_variable->Number();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a number." << std::endl;
        return 0;
    };
}

std::string String(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    std::string found_global;
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TSTRING){
        return get_variable->String();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a string." << std::endl;
        return "";
    };
}

std::unordered_map<std::variant<std::string,lua_Integer>,std::any> Table(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TTABLE){
        return get_variable->Table();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a table." << std::endl;
        return {};
    };
}

std::tuple<std::string,int,bool> Function(std::string name){
    lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TFUNCTION){
        return get_variable->Function();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a function." << std::endl;
        return {};
    };
}

std::any* Userdata(std::string name){
lua_getglobal(*(this->pointer_to_lua_state), name.c_str());
    if (lua_type(*(this->pointer_to_lua_state), -1) == LUA_TUSERDATA){
        return get_variable->Userdata();
    } else {
        std::cerr << "Global Lua variable " << name << " is not a userdata." << std::endl;
        return nullptr;
    };
}
};

