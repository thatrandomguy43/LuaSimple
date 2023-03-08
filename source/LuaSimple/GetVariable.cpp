#include <string>
#include <iostream>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"
#include <variant>
#include "LuaInstance.hpp"
#include "GetVariable.hpp"

GetVariable::GetVariable(lua_State** L){
    this->pointer_to_lua_state = L;
}

bool GetVariable::Boolean(){
    bool found_global = lua_toboolean(*(this->pointer_to_lua_state),-1);
    return found_global;
}

void* GetVariable::LightUserdata(){
    void* found_global = lua_touserdata(*(this->pointer_to_lua_state),-1);
    return found_global;
}


lua_Number GetVariable::Number(){
    lua_Number found_global = lua_tonumber(*(this->pointer_to_lua_state),-1);
    return found_global;
}

std::string GetVariable::String(){
    std::string found_global = lua_tostring(*(this->pointer_to_lua_state),-1);
    return found_global;
}

lua_Table GetVariable::Table(){
    lua_Table return_table;
    std::variant<std::string,lua_Integer> key_to_add;
    std::any value_to_add;
    lua_pushnil(*(this->pointer_to_lua_state));
    while (lua_next(*(this->pointer_to_lua_state),-2) != 0){
        if (lua_type(*(this->pointer_to_lua_state),-2) == LUA_TNUMBER){
            key_to_add = lua_tointeger(*(this->pointer_to_lua_state), -2);
        } else if (lua_type(*(this->pointer_to_lua_state),-2) == LUA_TSTRING) {
            key_to_add = lua_tostring(*(this->pointer_to_lua_state), -2);
        } else {
            
        };
        //we do a little copy pasting
        switch (lua_type(*(this->pointer_to_lua_state),-1))
            {

            case LUA_TNIL:
                value_to_add = nullptr;
            break;
            case LUA_TBOOLEAN:
            {
                value_to_add = Boolean();
            }
            break;

            case LUA_TLIGHTUSERDATA:
            {
                value_to_add = LightUserdata();
            }
            break;
            
            case LUA_TNUMBER:
            {
                value_to_add = Number();
            }
            break;
            
            case LUA_TSTRING:
            {
                value_to_add = String();
            }
            break;
         
            case LUA_TTABLE:
            {//when the function is recursive! susjerma.jpg
                value_to_add = Table();
            }
            break;

            case LUA_TFUNCTION:
            {
                value_to_add = Function();
                //slaps the function below the table
                
            }
            break;

            case LUA_TUSERDATA:
            {
                value_to_add = (std::any*)(lua_touserdata(*(this->pointer_to_lua_state), -1));
            }
            break;

            default:
                value_to_add = nullptr;
            break;
            }
        return_table.insert({key_to_add,value_to_add});
        lua_pop(*(this->pointer_to_lua_state), 1);
    };
    return return_table;
}

std::tuple<int,int,bool> GetVariable::Function(){
    lua_Debug debug;
    lua_getinfo(*(this->pointer_to_lua_state), ">uS", &debug);
    int functions_in_stack = 0;
    while (lua_isfunction(*(this->pointer_to_lua_state), functions_in_stack))
    {
        functions_in_stack++;
    }
    lua_insert(*(this->pointer_to_lua_state), functions_in_stack+1);
    return std::make_tuple<int,int,bool>(functions_in_stack+1, debug.nparams, debug.isvararg);
}

std::any* GetVariable::Userdata(){
    std::any* found_global = (std::any*)lua_touserdata(*(this->pointer_to_lua_state),-1);
    return found_global;
}