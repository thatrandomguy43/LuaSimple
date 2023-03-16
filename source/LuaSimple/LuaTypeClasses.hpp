#include <string>
#include <variant>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"

#pragma once





class lua_Table
{
public:
    std::unordered_map<std::variant<std::string, lua_Integer>, std::any> table_contents;
    std::optional<std::string> metatable_name;
};

class lua_Function
{
public:
    int registry_key;
    int argument_count;
    bool takes_extra_args;
};

class lua_Userdata 
{
public:
std::any* object;
std::optional<std::string> metatable_name;
};



class notlua_Table
{
public:
    std::unordered_map<std::variant<nullptr_t, bool, void*, lua_Integer, lua_Number, std::string, notlua_Table*, lua_Function, lua_Userdata>, std::variant<nullptr_t, bool, void*, lua_Integer, lua_Number, std::string, notlua_Table*, lua_Function, lua_Userdata>> table_contents;
    std::optional<std::string> metatable_name;
};

