#include <utility>
#include <string>
#include "luainclude/lua.hpp"
#include <unordered_map>
#include <variant>
#include <any>
#include <optional>
#include <iostream>
#include <cstring>
#pragma once
class PushVariable{
private:
lua_State** pointer_to_lua_state;

public:
PushVariable(lua_State** pointer_to_lua_state){
    this->pointer_to_lua_state = pointer_to_lua_state;
}

void Nil(){
    lua_pushnil(*(this->pointer_to_lua_state));
    return;
}

void Boolean(bool boolean){
    lua_pushboolean(*(this->pointer_to_lua_state), boolean);
    return;
}

void Number(lua_Number number){
    lua_pushnumber(*(this->pointer_to_lua_state), number);
    return;
}

void LightUserdata(void* pointer){
    lua_pushlightuserdata(*(this->pointer_to_lua_state), pointer);
    return;
}

void String(std::string string){
    lua_pushlstring(*(this->pointer_to_lua_state), string.c_str(), string.size());
    return;
}

void TableKeyAdder(std::variant<std::string,lua_Integer> key){
    //number index or string index?
    if (std::holds_alternative<lua_Integer>(key)){
        lua_seti(*(this->pointer_to_lua_state), -2, std::get<lua_Integer>(key));  
    } else if (std::holds_alternative<std::string>(key)) {
        lua_setfield(*(this->pointer_to_lua_state), -2, (std::get<std::string>(key)).c_str());
    } else { //the variant is sometimes empty so in that case remove the value manually to clean up
        lua_pop(*(this->pointer_to_lua_state),1);
    };
return;
}

void Table(std::unordered_map<std::variant<std::string,lua_Integer>,std::any> table_to_add, bool make_new/*if not set will instead merge the keys and values into existing table on top of stack, presubably overriding any duplicate keys with new ones*/, std::optional<std::string> metatable_name){
    if (make_new){
    lua_newtable(*(this->pointer_to_lua_state));
    };
    for (auto itr = table_to_add.begin(); itr != table_to_add.end(); itr++){
        //if there's nothing in the any do nothing
        if (itr->second.has_value()){
            //boolean
            if (itr->second.type() == typeid(bool)){
                Boolean(std::any_cast<bool>(itr->second));
                TableKeyAdder(itr->first);
            }
            //number
            else if (itr->second.type() == typeid(lua_Number)){
                Number(std::any_cast<lua_Number>(itr->second));
                TableKeyAdder(itr->first);
            }
            //light userdata
            else if (itr->second.type() == typeid(void*)){
                LightUserdata(std::any_cast<void*>(itr->second));
                TableKeyAdder(itr->first);
            }
            //string
            else if (itr->second.type() == typeid(std::string)){
                String(std::any_cast<std::string>(itr->second));
                TableKeyAdder(itr->first);
            }
            else if (itr->second.type() == typeid(lua_CFunction)){
                Function(std::any_cast<lua_CFunction>(itr->second));
                TableKeyAdder(itr->first);
            }
            //nil
             else {
                lua_pushnil(*(this->pointer_to_lua_state));
                TableKeyAdder(itr->first);
            };
            //full userdata would not work as you need to get a pointer to its location in lua back
        };
    };
    if (metatable_name.has_value()){
        luaL_setmetatable(*(this->pointer_to_lua_state), metatable_name.value().c_str());
    }
    return;
}




//i dont really understand the purpose of c function upvalues, if you need to do stuff with other lua values for some reason just make them
//this now works for at least basic funcs, yay
void Function(lua_CFunction function){
    lua_pushcfunction(*(this->pointer_to_lua_state), function);
}


//not doing uservalues quite yet
std::any* Userdata(std::any data, std::optional<std::string> metatable_name /*,int uservalue_count*/){
    void* userdata_ptr = lua_newuserdatauv(*(this->pointer_to_lua_state), sizeof data, 0);
    if (metatable_name.has_value()){
        luaL_setmetatable(*(this->pointer_to_lua_state), metatable_name.value().c_str());
    }
    std::memmove(userdata_ptr, &data, sizeof data);
    return (std::any*)userdata_ptr;
}

};