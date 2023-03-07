#include "luainclude/lua.hpp"
#include <string>
#include <iostream>
#include "SetGlobal.hpp"
#include "PushVariable.hpp"
#include "GetGlobal.hpp"
#include <vector>
#include <any>
#include "GetVariable.hpp"
#pragma once

typedef std::unordered_map<std::variant<std::string,lua_Integer>,std::any> lua_Table;

class LuaInstance{

    public:
    lua_State* pointer_to_lua_state;

    LuaInstance(){
        this->pointer_to_lua_state = luaL_newstate();
        luaL_openlibs(this->pointer_to_lua_state);
    };

    ~LuaInstance(){
        lua_close(this->pointer_to_lua_state);
    };

    PushVariable PushVariable{&pointer_to_lua_state};
    GetVariable GetVariable{&pointer_to_lua_state};
    GetGlobal GetGlobal{&pointer_to_lua_state, &GetVariable};
    SetGlobal SetGlobal{&pointer_to_lua_state, &PushVariable};


int DoString(std::string code){
    int response_code = luaL_dostring(this->pointer_to_lua_state,code.c_str());
    if (response_code != 0){
        std::string error_message = luaL_tolstring(this->pointer_to_lua_state, -1, NULL);
        std::cerr << error_message;
    };
    return response_code;
};

std::vector<std::any> GetArguments(std::vector<int> types){
    std::vector<std::any> argument_values;
    int num_arguments = lua_gettop(this->pointer_to_lua_state);
    argument_values.resize(num_arguments);

    for (int argument = 0; argument != num_arguments; argument++){
    //only do a check if there is actualy a value to check against, don't want any buffer overflows here
        if (argument <= types.size()){
            luaL_checktype(this->pointer_to_lua_state, -1, types[argument]);
        };

        switch (lua_type(this->pointer_to_lua_state,-1))
        {
//changing from push back to index access fixed the any.type() value loss it seems
        case LUA_TNIL:
            argument_values[argument] = std::make_any<nullptr_t>(nullptr);
        break;
        case LUA_TBOOLEAN:
        {
            bool value = lua_toboolean(this->pointer_to_lua_state, -1);
            argument_values[argument] = (std::make_any<bool>(value));
        }
        break;

        case LUA_TLIGHTUSERDATA:
        {
            void* value = lua_touserdata(this->pointer_to_lua_state, -1);
            argument_values[argument] =(std::make_any<void*>(value));
        }
        break;
            
        case LUA_TNUMBER:
        {
            lua_Number value = lua_tonumber(this->pointer_to_lua_state, -1);
            argument_values[argument] =(std::make_any<lua_Number>(value));
        }
        break;
            
        case LUA_TSTRING:
        {
            std::string value = lua_tostring(this->pointer_to_lua_state, -1);
            argument_values[argument] =(std::make_any<std::string>(value));
        }
        break;
         
        case LUA_TTABLE:
        {
            lua_Table value = GetVariable.Table();
            argument_values[argument] =(std::make_any<lua_Table>(value));
        }
        break;
        case LUA_TFUNCTION:
        {
            std::tuple<int,int,bool> value = GetVariable.Function();
            argument_values[argument] =(std::make_any<std::tuple<int,int,bool>>(value));
        }

        case LUA_TUSERDATA:
        {
            std::any* value = (std::any*)(lua_touserdata(this->pointer_to_lua_state, -1));
            argument_values[argument] =(std::make_any<std::any*>(value));
        }
        break;

        default:
            argument_values[argument] = (std::make_any<nullptr_t>(nullptr));
        break;
        }

        }
        return argument_values;
    }
void ReturnResults(std::vector<std::any> values){

}
};