#include <utility>
#include <string>
#include "luainclude/lua.hpp"
#include <unordered_map>
#include <variant>
#include <any>
#include <optional>

#pragma once

typedef std::unordered_map<std::variant<std::string,lua_Integer>,std::any> lua_Table;

class PushVariable{
private:
lua_State** pointer_to_lua_state;

public:
PushVariable(lua_State** pointer_to_lua_state);

void Nil();

void Boolean(bool boolean);

void Number(lua_Number number);

void LightUserdata(void* pointer);

void String(std::string string);

void TableKeyAdder(std::variant<std::string,lua_Integer> key);

void Table(lua_Table table_to_add, bool make_new/*if not set will instead merge the keys and values into existing table on top of stack, presubably overriding any duplicate keys with new ones*/, std::optional<std::string> metatable_name);



//i dont really understand the purpose of c function upvalues, if you need to do stuff with other lua values for some reason just make them
//this now works for at least basic funcs, yay
void Function(lua_CFunction function);


//not doing uservalues quite yet
std::any* Userdata(std::any data, std::optional<std::string> metatable_name /*,int uservalue_count*/);


};