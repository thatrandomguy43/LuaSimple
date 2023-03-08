#include "luainclude/lua.hpp"
#include <string>
#include <iostream>
#include "SetGlobal.hpp"
#include "PushVariable.hpp"
#include "GetGlobal.hpp"
#include <vector>
#include <any>
#include "GetVariable.hpp"
#include "LuaInstance.hpp"






    LuaInstance::LuaInstance(){
        this->pointer_to_lua_state = luaL_newstate();
        luaL_openlibs(this->pointer_to_lua_state);
    };

    LuaInstance::~LuaInstance(){
        lua_close(this->pointer_to_lua_state);
    };

LuaInstance::PushVariable = PushVariable(&pointer_to_lua_state);
LuaInstance::GetVariable = GetVariable(&pointer_to_lua_state);
LuaInstance::GetGlobal = GetGlobal(&pointer_to_lua_state, &GetVariable);
SetGlobal SetGlobal = SetGlobal(&pointer_to_lua_state, &PushVariable);


int LuaInstance::DoString(std::string code){
    int response_code = luaL_dostring(this->pointer_to_lua_state,code.c_str());
    if (response_code != 0){
        std::string error_message = luaL_tolstring(this->pointer_to_lua_state, -1, NULL);
        std::cerr << error_message;
    };
    return response_code;
};

std::vector<std::any> LuaInstance::GetArguments(std::vector<int> types){
    std::vector<std::any> argument_values;
    int num_arguments = lua_gettop(this->pointer_to_lua_state);
    argument_values.resize(num_arguments);
    int function_count = 0;
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
            function_count++;
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
        //removes everything that is not a function, as it has no reason to fill the stack anymore and would block functions from being called
        lua_settop(this->pointer_to_lua_state, function_count);
        return argument_values;
    }
void LuaInstance::ReturnResults(std::vector<std::any> values){

}
