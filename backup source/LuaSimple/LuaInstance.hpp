#include <variant>
#include <string>
#include <unordered_map>
#include <any>
#include <utility>
#include <vector>
#include "luainclude/lua.hpp"
#include "PushVariable.hpp"
#include "PopVariable.hpp"
#include "GetGlobal.hpp"
#include "SetGlobal.hpp"
#include <memory>

#pragma once

typedef std::variant<nullptr_t, bool, void*, lua_Integer, lua_Number, std::string, std::unique_ptr<lua_Table>, lua_CFunction, lua_Function, lua_Userdata> lua_Value;

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
    PopVariable PopVariable;
    GetGlobal GetGlobal;
    SetGlobal SetGlobal;

    int DoFunction(lua_Function, std::vector<std::any>);

    int DoString(std::string);

    int DoFile(std::string);

    void HandleReturn(int);

    void GetArguments(std::vector<int>);

    void PushValue(lua_Value);

    lua_Value PopValue();

    void ReturnResults(std::vector<std::any>);

    static LuaInstance& FindInstance(lua_State*);
};