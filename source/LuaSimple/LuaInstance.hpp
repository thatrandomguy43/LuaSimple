#include "luainclude/lua.hpp"
#include <string>
#include <iostream>
#include "SetGlobal.hpp"
#include "PushVariable.hpp"
#include <vector>
#include <any>

#pragma once
class LuaInstance{

    public:
    std::shared_ptr<lua_State*> pointer_to_lua_state = std::make_shared<lua_State*>();

    LuaInstance(){
        *(this->pointer_to_lua_state) = luaL_newstate();
        luaL_openlibs(*(this->pointer_to_lua_state));
    };

    ~LuaInstance(){
        lua_close(*(this->pointer_to_lua_state));
    };

    PushVariable PushVariable{this->pointer_to_lua_state};

    SetGlobal SetGlobal{this->pointer_to_lua_state, &PushVariable};


int DoString(std::string code){
    int response_code = luaL_dostring(*(this->pointer_to_lua_state),code.c_str());
    if (response_code != 0){
        std::string error_message = luaL_tolstring(*(this->pointer_to_lua_state), -1, NULL);
        std::cerr << error_message;
    };
    return response_code;
};

    std::vector<std::any> GetArguments(std::vector<int> types){
        std::vector<std::any> argument_values;
        int num_arguments = lua_gettop(*(this->pointer_to_lua_state));
        argument_values.resize(num_arguments);

        for (int argument = 0; argument != num_arguments; argument++){
        //only do a check if there is actualy a value to check against, don't want any buffer overflows here
            if (argument <= types.size()){
                luaL_checktype(*(this->pointer_to_lua_state), -1, types[argument]);
            };

            switch (lua_type(*(this->pointer_to_lua_state),-1))
            {
//changing from push back to index access fixed the any.type() value loss it seems
            case LUA_TNIL:
                argument_values.push_back(std::make_any<nullptr_t>(nullptr));
            break;
            case LUA_TBOOLEAN:
            {
                bool value = lua_toboolean(*(this->pointer_to_lua_state), -1);
                argument_values[argument] = (std::make_any<bool>(value));
            }
            break;

            case LUA_TLIGHTUSERDATA:
            {
                void* value = lua_touserdata(*(this->pointer_to_lua_state), -1);
                argument_values[argument] =(std::make_any<void*>(value));
            }
            break;
            
            case LUA_TNUMBER:
            {
                lua_Number value = lua_tonumber(*(this->pointer_to_lua_state), -1);
                argument_values[argument] =(std::make_any<lua_Number>(value));
            }
            break;
            
            case LUA_TSTRING:
            {
                std::string value = lua_tostring(*(this->pointer_to_lua_state), -1);
                
                argument_values[argument] =(std::make_any<std::string>(value));
            }
            break;
         
            case LUA_TTABLE:
            {
                std::unordered_map<std::variant<std::string,lua_Integer>,std::any> value = PushVariable.ParseTable();
                argument_values[argument] =(std::make_any<std::unordered_map<std::variant<std::string,lua_Integer>,std::any>>(value));
            }
            break;
           /* case LUA_TFUNCTION:
            {
                std::string value = 
            }*/

            case LUA_TUSERDATA:
            {
                std::any* value = (std::any*)(lua_touserdata(*(this->pointer_to_lua_state), -1));
                argument_values[argument] =(std::make_any<std::any*>(value));
            }
            break;

            default:

            break;
            }
            //gotta do it this way so it is always working with the top value, and to not leave stuff left on the stack
            if (types[argument] != LUA_TFUNCTION){ //in this case the pop already got done by lua_insert
                lua_pop(*(this->pointer_to_lua_state),1);
            }
        }
        return argument_values;
    }
};