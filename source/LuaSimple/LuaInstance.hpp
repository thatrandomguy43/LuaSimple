#include <variant>
#include <string>
#include <unordered_map>
#include <any>
#include <utility>
#include <vector>
#include "luainclude/lua.hpp"
#include <memory>
#include "LuaTypeClasses.hpp"
#pragma once

typedef std::variant<nullptr_t, bool, void*, lua_Integer, lua_Number, std::string, std::shared_ptr<lua_Table>, lua_CFunction, lua_Function, lua_Userdata> lua_Value;
typedef std::variant<bool, void*, lua_Integer, lua_Number, std::string, std::shared_ptr<lua_Table>, lua_CFunction, lua_Function, lua_Userdata> lua_TableValue;


class LuaInstance
{
public:
    lua_State* pointer_to_lua_state;

    std::vector<lua_Value> lua_return_values;

    std::vector<lua_Value> lua_argument_values;

    static inline std::unordered_map<lua_State*, LuaInstance*> instance_list = std::unordered_map<lua_State*, LuaInstance*>();

    LuaInstance();
    ~LuaInstance();


    int DoFunction(lua_Function, std::vector<lua_Value>);

    int DoString(std::string);

    int DoFile(std::string);

    void HandleReturn(int);

    void GetArguments(std::vector<int>);

    void PushValue(lua_Value);

    lua_Value PopValue();

    void PushTableValue(lua_TableValue);

    lua_TableValue PopTableValue();

    void ReturnResults(std::vector<lua_Value>);

    static LuaInstance& FindInstance(lua_State*);
};