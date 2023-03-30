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
    static int FunctionDumper(lua_State*, const void*, size_t, void*);

    void PushValue(lua_Value);

    lua_Value PopValue();

    void HandleReturn(int);

    static inline std::unordered_map<lua_State*, LuaInstance*> instance_list = std::unordered_map<lua_State*, LuaInstance*>();

public:
    lua_State* lua_ptr;

    std::vector<lua_Value> lua_return_values;

    std::vector<lua_Value> lua_argument_values;

    LuaInstance();
    ~LuaInstance();

    int DoFunction(lua_Function, std::vector<lua_Value>, const std::optional<std::string>&);

    int DoString(const std::string&);

    int DoFile(const std::string&);

    int ReturnResults(const std::vector<lua_Value>&);

    void SetGlobal(lua_Value, const std::string&);

    lua_Value GetGlobal(const std::string&);

    void SetMetatable(std::shared_ptr<lua_Table>, const std::string&);

    std::shared_ptr<lua_Table> GetMetatable(const std::string&);

    static LuaInstance& GetLuaData(lua_State*, const std::vector<int>&);


};