#include "luainclude/lua.hpp"
#include "PushVariable.hpp"
#include <string>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <any>
#include <variant>
#include <memory>
#include <optional>
#pragma once
class SetGlobal
{
private:
lua_State** pointer_to_lua_state;
PushVariable* variable_maker;



public:
SetGlobal(lua_State** pointer_to_lua_state, PushVariable* variable_maker){
    this->pointer_to_lua_state = pointer_to_lua_state;
    this->variable_maker = variable_maker;
}


void Boolean(bool bool_to_set, std::string name){
    variable_maker->Boolean(bool_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return;
}

void Number(double number_to_set, std::string name){
    variable_maker->Number(number_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
        return;
}

void LightUserdata(void* pointer_to_set, std::string name){
    variable_maker->LightUserdata(pointer_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return;
}
void String(std::string string_to_set, std::string name){
    variable_maker->String(string_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return;
}

void Table(lua_Table table_to_set, std::string name, std::optional<std::string> metatable_name){
    variable_maker->Table(table_to_set, true, metatable_name);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
}

void Metatable(lua_Table metatable_to_register, std::string name){
    if (luaL_getmetatable(*(this->pointer_to_lua_state),name.c_str()) == 0){
        lua_pop(*(this->pointer_to_lua_state),1);
        luaL_newmetatable(*(this->pointer_to_lua_state), name.c_str());
    };
    variable_maker->Table(metatable_to_register, false,{});
    return;
}

void Function(lua_CFunction function_to_set, std::string name){
    variable_maker->Function(function_to_set);
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
}

std::any* Userdata(std::any data, std::string name, std::optional<std::string> metatable_name /*,int uservalue_count*/){
    std::any* userdata_ptr = variable_maker->Userdata(data, metatable_name);
    
    lua_setglobal(*(this->pointer_to_lua_state), name.c_str());
    return userdata_ptr;
}

};



