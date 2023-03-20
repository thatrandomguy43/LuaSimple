#include <string>
#include <variant>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"
#include <memory>
#include <optional>
#pragma once






class lua_Function
{
public:
    lua_Integer registry_key;
    lua_Integer argument_count;
    bool takes_extra_args;

    bool operator==(const lua_Function& other) const{
        if (this->registry_key == other.registry_key){
            return true;
        } else {
            return false;
        };
    }
};


template <> struct std::hash<lua_Function>
{
size_t operator()(const lua_Function& func){
    return std::hash<lua_Integer>()(func.registry_key << func.argument_count ^ static_cast<lua_Integer>(func.takes_extra_args));
}
};


class lua_Userdata 
{
public:
std::any* object;
std::optional<std::string> metatable_name;

bool operator==(const lua_Userdata& other) const {
    if (this->object == other.object){
        return true;
    } else {
        return false;
    }
}

};


template <> struct std::hash<lua_Userdata>
{
size_t operator()(const lua_Userdata& udata){
    return std::hash<any*>()(udata.object);
}
};

class lua_Table
{
public:
    std::unordered_map<std::variant<bool, void*, lua_Integer, lua_Number, std::string, std::shared_ptr<lua_Table>, lua_CFunction, lua_Function, lua_Userdata>, std::variant<bool, void*, lua_Integer, lua_Number, std::string, std::shared_ptr<lua_Table>, lua_CFunction, lua_Function, lua_Userdata>> table_contents;
    std::optional<std::string> metatable_name;

   
};
