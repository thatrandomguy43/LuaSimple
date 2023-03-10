#include <variant>
#include <string>
#include <unordered_map>
#include <any>
#include <utility>
#include "luainclude/lua.hpp"
#include "PushVariable.hpp"
#include "GetVariable.hpp"
#include "GetGlobal.hpp"
#include "SetGlobal.hpp"

#pragma once

typedef std::unordered_map<std::variant<std::string,lua_Integer>,std::any> lua_Table;

class LuaInstance{
public:
lua_State* pointer_to_lua_state;

static inline std::vector<std::pair<lua_State*, LuaInstance*>> instance_list = std::vector<std::pair<lua_State*, LuaInstance*>>();

LuaInstance();
~LuaInstance();

PushVariable PushVariable;
GetVariable GetVariable;
GetGlobal GetGlobal;
SetGlobal SetGlobal;

int DoString(std::string);

std::vector<std::any> GetArguments(std::vector<int>);

void ReturnResults(std::vector<std::any>);

static LuaInstance& FindInstance(lua_State*);
};



