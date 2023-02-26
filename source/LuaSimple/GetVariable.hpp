#include <string>
#include <iostream>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"
#include <variant>
#pragma once
class GetVariable
{
private:
lua_State** pointer_to_lua_state;
public:
GetVariable(lua_State** L){
    this->pointer_to_lua_state = L;
}


std::unordered_map<std::variant<std::string,lua_Integer>,std::any> Table(){
    std::unordered_map<std::variant<std::string,lua_Integer>,std::any> return_table;
    std::variant<std::string,lua_Integer> key_to_add;
    std::any value_to_add;
    lua_pushnil(*(this->pointer_to_lua_state));
    while (lua_next(*(this->pointer_to_lua_state),-2) != 0){
        if (lua_type(*(this->pointer_to_lua_state),-2) == LUA_TNUMBER){
            key_to_add = lua_tointeger(*(this->pointer_to_lua_state), -2);
        } else if (lua_type(*(this->pointer_to_lua_state),-2) == LUA_TSTRING) {
            key_to_add = lua_tostring(*(this->pointer_to_lua_state), -2);
        };
        //we do a little copy pasting
        switch (lua_type(*(this->pointer_to_lua_state),-1))
            {

            case LUA_TNIL:
                value_to_add = nullptr;
            break;
            case LUA_TBOOLEAN:
            {
                value_to_add = lua_toboolean(*(this->pointer_to_lua_state), -1);
            }
            break;

            case LUA_TLIGHTUSERDATA:
            {
                value_to_add = lua_touserdata(*(this->pointer_to_lua_state), -1);
            }
            break;
            
            case LUA_TNUMBER:
            {
                value_to_add = lua_tonumber(*(this->pointer_to_lua_state), -1);
            }
            break;
            
            case LUA_TSTRING:
            {
                value_to_add = (std::string)lua_tostring(*(this->pointer_to_lua_state), -1);
            }
            break;
         
            case LUA_TTABLE:
            {//when the function is recursive! susjerma.jpg
                value_to_add = Table();
            }
            break;

            case LUA_TUSERDATA:
            {
                value_to_add = (std::any*)(lua_touserdata(*(this->pointer_to_lua_state), -1));
            }
            break;

            default:

            break;
            }
        return_table.insert({key_to_add,value_to_add});
        lua_pop(*(this->pointer_to_lua_state), 1);
    };
    return return_table;
}

};


