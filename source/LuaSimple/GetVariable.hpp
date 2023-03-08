#include <string>
#include <iostream>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"
#include <variant>


typedef std::unordered_map<std::variant<std::string,lua_Integer>,std::any> lua_Table;

class GetVariable
{
private:
lua_State** pointer_to_lua_state;
public:

GetVariable(lua_State** L);

bool Boolean();

void* LightUserdata();


lua_Number Number();

std::string String();

lua_Table Table();

std::tuple<int,int,bool> Function();

std::any* Userdata();

};


