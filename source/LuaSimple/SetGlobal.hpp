#include "luainclude/lua.hpp"
#include "PushVariable.hpp"
#include <string>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <any>
#include <variant>
#include <memory>
#include <optional>

typedef std::unordered_map<std::variant<std::string,lua_Integer>,std::any> lua_Table;
class SetGlobal
{
private:
lua_State** pointer_to_lua_state;
PushVariable* variable_maker;



public:
SetGlobal(lua_State** pointer_to_lua_state, PushVariable* variable_maker);


void Boolean(bool bool_to_set, std::string name);

void Number(double number_to_set, std::string name);
void LightUserdata(void* pointer_to_set, std::string name);
void String(std::string string_to_set, std::string name);

void Table(lua_Table table_to_set, std::string name, std::optional<std::string> metatable_name);

void Metatable(lua_Table metatable_to_register, std::string name);

void Function(lua_CFunction function_to_set, std::string name);

std::any* Userdata(std::any data, std::string name, std::optional<std::string> metatable_name /*,int uservalue_count*/);


};



