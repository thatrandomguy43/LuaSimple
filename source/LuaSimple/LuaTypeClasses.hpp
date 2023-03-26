#include <string>
#include <variant>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"
#include <memory>
#include <optional>
#include <vector>
#include <cstddef>
#pragma once





class lua_Function
{
public:
    std::vector<unsigned char> bytecode;

    bool operator==(const lua_Function& other) const{
       return this->bytecode == other.bytecode;
    }
};

template <> struct std::hash<std::vector<std::byte>> {
    size_t operator()(const std::vector<unsigned char>& vect){
        size_t result_hash = 0;
        for (auto itr : vect){
            result_hash = result_hash ^ std::hash<unsigned char>()(itr);
        };
        return result_hash;
    }
};


template <> 
struct std::hash<lua_Function> {
    size_t operator()(const lua_Function& func){
        return std::hash<std::vector<std::byte>>()(func.bytecode);
    }
};

class lua_Userdata 
{
public:
std::any* object;
std::optional<std::string> metatable_name;

bool operator==(const lua_Userdata& other) const {
    return this->object == other.object;
}

};

template <>
struct std::hash<lua_Userdata>
{
size_t operator()(const lua_Userdata& udata){
    return std::hash<std::any*>()(udata.object);
}
};

class lua_Table
{
public:
    std::unordered_map<std::variant<nullptr_t, bool, void*, lua_Integer, lua_Number, std::string, std::shared_ptr<lua_Table>, lua_CFunction, lua_Function, lua_Userdata>, std::variant<nullptr_t, bool, void*, lua_Integer, lua_Number, std::string, std::shared_ptr<lua_Table>, lua_CFunction, lua_Function, lua_Userdata>> table_contents;
    std::optional<std::string> metatable_name;

   
};
