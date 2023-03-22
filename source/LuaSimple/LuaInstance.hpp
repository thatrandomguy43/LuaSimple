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



class LuaInstance
{

private: 
    void PushValue(lua_Value);

    lua_Value PopValue();

    void HandleReturn(int);

    static inline std::unordered_map<lua_State*, LuaInstance*> instance_list = std::unordered_map<lua_State*, LuaInstance*>();

public:
    lua_State* pointer_to_lua_state;

    std::vector<lua_Value> lua_return_values;

    std::vector<lua_Value> lua_argument_values;

    LuaInstance();
    ~LuaInstance();

    int DoFunction(lua_Function, std::vector<lua_Value>);

    int DoString(std::string);

    int DoFile(std::string);

    void GetArguments(std::vector<int>);

    void ReturnResults(std::vector<lua_Value>);

    void SetGlobal(lua_Value, std::string);

    lua_Value GetGlobal(std::string);

    void SetMetatable(std::shared_ptr<lua_Table>, std::string);

    std::shared_ptr<lua_Table> GetMetatable(std::string);

    static LuaInstance& FindInstance(lua_State*);


};