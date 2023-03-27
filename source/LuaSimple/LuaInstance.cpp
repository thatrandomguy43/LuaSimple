#include "luainclude/lua.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <any>
#include "LuaInstance.hpp"
#include "LuaTypeClasses.hpp"
#include <cstring>

using namespace std;


LuaInstance::LuaInstance(): pointer_to_lua_state(luaL_newstate())
{
    luaL_openlibs(this->pointer_to_lua_state);
    this->instance_list[this->pointer_to_lua_state] = this;
}

LuaInstance::~LuaInstance()
{
    lua_close(this->pointer_to_lua_state);
    this->instance_list.erase(this->pointer_to_lua_state);
}

int LuaInstance::FunctionDumper(lua_State* lua_ptr, const void* data_buffer, size_t length, void* target_void){
    const unsigned char* data = static_cast<const unsigned char*>(data_buffer);
    lua_Function* target = static_cast<lua_Function*>(target_void);
    for (size_t idx = 0; idx < length; idx++){
        target->bytecode.push_back(data[idx]);
    }
    return 0;
}

void LuaInstance::PushValue(lua_Value to_push){
    if (holds_alternative<nullptr_t>(to_push))
    {
        lua_pushnil(this->pointer_to_lua_state);
    } 
    else if (holds_alternative<bool>(to_push))
    {
        lua_pushboolean(this->pointer_to_lua_state, get<bool>(to_push));
    }
    else if (holds_alternative<void*>(to_push))
    {
        lua_pushlightuserdata(this->pointer_to_lua_state, get<void*>(to_push));
    }
    else if (holds_alternative<lua_Integer>(to_push))
    {
        lua_pushinteger(this->pointer_to_lua_state, get<lua_Integer>(to_push));
    }
    else if (holds_alternative<lua_Number>(to_push))
    {
        lua_pushnumber(this->pointer_to_lua_state, get<lua_Number>(to_push));
    }
    else if (holds_alternative<string>(to_push))
    {
        lua_pushstring(this->pointer_to_lua_state, get<string>(to_push).c_str());
    }
    else if (holds_alternative<shared_ptr<lua_Table>>(to_push))
    {
        lua_newtable(this->pointer_to_lua_state);
        for (auto itr = get<shared_ptr<lua_Table>>(to_push)->table_contents.begin(); itr != get<shared_ptr<lua_Table>>(to_push)->table_contents.end(); itr++){
            this->PushValue(itr->first);
            this->PushValue(itr->second);
            lua_settable(this->pointer_to_lua_state, -3);
        }
        if (get<shared_ptr<lua_Table>>(to_push)->metatable_name.has_value()){
            luaL_setmetatable(this->pointer_to_lua_state, get<shared_ptr<lua_Table>>(to_push)->metatable_name.value().c_str());
        }
    }
    else if (holds_alternative<lua_CFunction>(to_push))
    {
        lua_pushcfunction(this->pointer_to_lua_state, get<lua_CFunction>(to_push));
    }
    else if (holds_alternative<lua_Function>(to_push))
    {
        luaL_loadbufferx(this->pointer_to_lua_state, reinterpret_cast<char*>(get<lua_Function>(to_push).bytecode.data()), get<lua_Function>(to_push).bytecode.size(), "unnamed_lua_function", "b");
    }
    else if (holds_alternative<lua_Userdata>(to_push))
    {
        any* userdata_buffer = static_cast<any*>(lua_newuserdatauv(this->pointer_to_lua_state, sizeof *(get<lua_Userdata>(to_push).object),0));
        //memmove bad, but this is what i found to work
        memmove(userdata_buffer, get<lua_Userdata>(to_push).object, sizeof *(get<lua_Userdata>(to_push).object));
        if (get<lua_Userdata>(to_push).metatable_name.has_value())
        {
            luaL_setmetatable(this->pointer_to_lua_state, get<lua_Userdata>(to_push).metatable_name.value().c_str());
        }
    }
    return;
}

lua_Value LuaInstance::PopValue() {
    lua_Value popped;
    switch (lua_type(this->pointer_to_lua_state, -1))
    {
    case LUA_TNIL:
        popped = nullptr;
        break;
    case LUA_TBOOLEAN:
    {
        popped = static_cast<bool>(lua_toboolean(this->pointer_to_lua_state, -1));
    }
    break;

    case LUA_TLIGHTUSERDATA:
    {
        popped = lua_touserdata(this->pointer_to_lua_state, -1);

    }
    break;

    case LUA_TNUMBER:
    {
        if (lua_isinteger(this->pointer_to_lua_state, -1))
        {
            popped = lua_tointeger(this->pointer_to_lua_state, -1);
        }
        else
        {
            popped = lua_tonumber(this->pointer_to_lua_state, -1);
        };
    }
    break;

    case LUA_TSTRING:
    {
        popped = lua_tostring(this->pointer_to_lua_state, -1);
    }
    break;

    case LUA_TTABLE:
    {
        popped = make_shared<lua_Table>();
        this->PushValue(nullptr);
        lua_Value key;
        lua_Value value;
        auto popped_ptr = get<shared_ptr<lua_Table>>(popped);
        while (lua_next(this->pointer_to_lua_state, -2) != 0)
        {
            value = this->PopValue();
            lua_pushvalue(this->pointer_to_lua_state, -1);
            key = this->PopValue();
            popped_ptr->table_contents[key] = value;
        }
        if (luaL_getmetafield(this->pointer_to_lua_state, -1, "__name") != LUA_TNIL){
            get<shared_ptr<lua_Table>>(popped)->metatable_name = get<string>(this->PopValue());
        }
        
        
    }
    break;
    case LUA_TFUNCTION:
    {
        if (lua_iscfunction(this->pointer_to_lua_state, -1))
        {
            popped = lua_tocfunction(this->pointer_to_lua_state, -1);
        }
        else
        {
            lua_Function temp_container;
            lua_dump(this->pointer_to_lua_state, &(this->FunctionDumper), &temp_container, false);
            popped = temp_container;
        }
    };
    break;
    case LUA_TUSERDATA:
    {
        lua_Userdata udata;
        if (luaL_getmetafield(this->pointer_to_lua_state, -1, "__name") != LUA_TNIL){
            udata.metatable_name = get<string>(this->PopValue());
        };
        udata.object = static_cast<any*>(lua_touserdata(this->pointer_to_lua_state, -1));
        popped = udata; 
    }
    break;
    }
    lua_pop(this->pointer_to_lua_state, 1);
    return popped;
}

void LuaInstance::HandleReturn(int response) {
    if (response != 0)
    {
        this->lua_return_values.resize(1);
        this->lua_return_values[0] = static_cast<string>(luaL_tolstring(this->pointer_to_lua_state, -1, NULL));
        cerr << get<string>(lua_return_values[0]) << endl;
    }
    else
    {
        int num_returns = lua_gettop(this->pointer_to_lua_state);
        this->lua_return_values.resize(num_returns);
        for (int return_index = num_returns - 1; return_index >= 0; return_index--) {
            this->lua_return_values[return_index] = this->PopValue();
        }
    }
    return;
}

int LuaInstance::DoFunction(lua_Function function_object, vector<lua_Value> arguments, const optional<string>& debug_name) {
    lua_getglobal(this->pointer_to_lua_state, "debug");
    lua_getfield(this->pointer_to_lua_state, 1, "traceback");
    lua_remove(this->pointer_to_lua_state, 1);
    if (debug_name.has_value()){
        luaL_loadbufferx(this->pointer_to_lua_state, reinterpret_cast<char*>(function_object.bytecode.data()), function_object.bytecode.size(), debug_name.value().c_str(), "b");
    } else {
        luaL_loadbufferx(this->pointer_to_lua_state, reinterpret_cast<char*>(function_object.bytecode.data()), function_object.bytecode.size(), "unnamed lua function", "b");
    }
    for (auto itr = arguments.begin(); itr != arguments.end(); itr++) {
        this->PushValue(*itr);
    }
    int response_code = lua_pcall(this->pointer_to_lua_state, arguments.size(), LUA_MULTRET, 1);
    lua_remove(this->pointer_to_lua_state, 1);
    this->HandleReturn(response_code);
    return response_code;
}

int LuaInstance::DoString(const string& code)
{
    int response_code = luaL_dostring(this->pointer_to_lua_state, code.c_str());
    this->HandleReturn(response_code);
    return response_code;
}

int LuaInstance::DoFile(const string& filename)
{
    int response_code = luaL_dofile(this->pointer_to_lua_state, filename.c_str());
    this->HandleReturn(response_code);
    return response_code;
}

void LuaInstance::GetArguments(vector<int> types)
{

    int num_arguments = lua_gettop(this->pointer_to_lua_state);
    this->lua_argument_values.resize(num_arguments);
    for (int argument_index = num_arguments - 1; argument_index >= 0; argument_index--)
    {
        // only do a check if there is actualy a value to check against, extra args can be whatever and just go unused anyway
        if (argument_index < types.size())
        {
            luaL_checktype(this->pointer_to_lua_state, argument_index + 1, types[(argument_index)]);
        }

        this->lua_argument_values[argument_index] = this->PopValue();
    }
    return;
}

void LuaInstance::ReturnResults(vector<lua_Value> values)
{
    for (auto itr = values.begin(); itr != values.end(); itr++)
    {
        this->PushValue(*itr);
    }
}

void LuaInstance::SetGlobal(lua_Value value, const string& name){
    this->PushValue(value);
    lua_setglobal(this->pointer_to_lua_state, name.c_str());
    return;
}

lua_Value LuaInstance::GetGlobal(const string& name){
    lua_getglobal(this->pointer_to_lua_state, name.c_str());
    return this->PopValue();
}

void LuaInstance::SetMetatable(shared_ptr<lua_Table> table_to_set, const std::string& name){
    
    luaL_newmetatable(this->pointer_to_lua_state, name.c_str());
    for (auto itr = table_to_set->table_contents.begin(); itr != table_to_set->table_contents.end(); itr++){
            this->PushValue(itr->first);
            this->PushValue(itr->second);
            lua_settable(this->pointer_to_lua_state, -3);
        }
    lua_pop(this->pointer_to_lua_state, 1);
    return;
}

shared_ptr<lua_Table> LuaInstance::GetMetatable(const std::string& name){
    luaL_getmetatable(this->pointer_to_lua_state, name.c_str());
    return get<shared_ptr<lua_Table>>(this->PopValue());
}

LuaInstance& LuaInstance::FindInstance(lua_State* pointer_from_lua)
{
    LuaInstance& found_instance = *(LuaInstance::instance_list[pointer_from_lua]);
    return found_instance;
}
