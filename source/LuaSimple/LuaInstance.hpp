#include <variant>
#include <string>
#include <unordered_map>
#include <any>
#include "luainclude/lua.hpp"
#include "PushVariable.hpp"
#include "GetVariable.hpp"
#include "GetGlobal.hpp"
#include "SetGlobal.hpp"
typedef std::unordered_map<std::variant<std::string,lua_Integer>,std::any> lua_Table;

class LuaInstance{
public:
lua_State* pointer_to_lua_state;

LuaInstance();
~LuaInstance();

PushVariable PushVariable;
GetVariable GetVariable;
GetGlobal GetGlobal;
SetGlobal SetGlobal;

int DoString(std::string code);
std::vector<std::any> GetArguments(std::vector<int> types);
void ReturnResults(std::vector<std::any> values);
};



