#include <memory>
#include <iostream>
#include <string>
#include "luainclude/lua.hpp"
#include "GetVariable.hpp"

typedef std::unordered_map<std::variant<std::string,lua_Integer>,std::any> lua_Table;
class GetGlobal{
private:
lua_State** pointer_to_lua_state;
GetVariable* get_variable;
public:

GetGlobal(lua_State** L, GetVariable* get_obj);

int GetType(std::string name);

bool Boolean(std::string name);

void* LightUserdata(std::string name);

lua_Number Number(std::string name);

std::string String(std::string name);

lua_Table Table(std::string name);

std::tuple<int,int,bool> Function(std::string name);

std::any* Userdata(std::string name);
};

