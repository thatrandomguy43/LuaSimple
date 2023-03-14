#include <variant>
#include <string>
#include <unordered_map>
#include <any>
#include <utility>
#include "luainclude/lua.hpp"
#include "PushVariable.hpp"
#include "PopVariable.hpp"
#include "GetGlobal.hpp"
#include "SetGlobal.hpp"

#pragma once

typedef std::unordered_map<std::variant<std::string, lua_Integer>, std::any> lua_Table;

class LuaInstance
{
public:
    lua_State* pointer_to_lua_state;

    std::vector<std::any> lua_return_values;

    std::vector<std::any> lua_argument_values;

    static inline std::unordered_map<lua_State*, LuaInstance*> instance_list = std::unordered_map<lua_State*, LuaInstance*>();

    LuaInstance();
    ~LuaInstance();

    PushVariable PushVariable;
    GetVariable GetVariable;
    GetGlobal GetGlobal;
    SetGlobal SetGlobal;

    int DoFunction(LuaFunction, std::vector<std::any>);

    int DoString(std::string);

    int DoFile(std::string);

    void HandleReturn(int);

    void GetArguments(std::vector<int>);


    void ReturnResults(std::vector<std::any>);

    static LuaInstance& FindInstance(lua_State*);
};