#include "LuaSimple/LuaInstance.hpp"
#include <string>
#include <utility>
#include <unordered_map>
#include <any>
#include <variant>
#include <iostream>
#include <vector>
#include "LuaSimple/LuaTypeClasses.hpp"
using namespace std;

LuaInstance LUA_INST;




int StringOfAs(lua_State* lua_ptr){
    LuaInstance& current_lua = LuaInstance::GetLuaData(lua_ptr, {LUA_TNUMBER});
    string help_aaaa;
    lua_Integer count = get<lua_Integer>(current_lua.lua_argument_values[0]);
    for (long long int itr = 0; itr != count; itr++){
        help_aaaa.append("A");
    };

    current_lua.ReturnResults({help_aaaa});
    return 1;
}

class TestUserdata
{
private:
int funny_number = 13;
public:

static int GetFunnyNumber (lua_State* lua_ptr){
    LuaInstance& current_lua = LuaInstance::GetLuaData(lua_ptr, {LUA_TUSERDATA});
    lua_Userdata userdata_self = get<lua_Userdata>(current_lua.lua_argument_values[0]);
    TestUserdata* self = any_cast<TestUserdata>(userdata_self.object);
    current_lua.ReturnResults({self->funny_number});
    return 1;
}

};

int DigitSequenceNumber(lua_State* lua_ptr){
    LuaInstance& lua_object = LuaInstance::GetLuaData(lua_ptr, {LUA_TNUMBER});
    lua_Integer length;
    if (holds_alternative<lua_Integer>(lua_object.lua_argument_values[0])) {
        length = get<lua_Integer>(lua_object.lua_argument_values[0]);
    } else {
        length = static_cast<lua_Integer>(floor(get<lua_Number>(lua_object.lua_argument_values[0])));
    }
    if (length > 18){
        length = 18;
    } else if (length < 1) {
        length = 1;
    }
    lua_Integer result = 0;
    for (int itr = 1; not (itr > length); itr++){
        result = result + pow(itr, length-itr);
    }
    lua_object.ReturnResults({result});
    return 1;
}



int main(){

LUA_INST.DoString("print(\"Hey everyone, Lua here! My favorite number is\", 8*8)");


shared_ptr<lua_Table> test_table = make_shared<lua_Table>();

test_table->table_contents = {{"name","John"},{"age", 42},{"employed",true}};
test_table->metatable_name = nullopt;

LUA_INST.SetGlobal(test_table, "person_record");
LUA_INST.DoString("return person_record.age");

lua_Integer person_age = get<lua_Integer>(LUA_INST.lua_return_values[0]); 
if (person_age != 42){
    throw "Test 1 Failed!";
}


LUA_INST.DoString("funny_quote = \"Airplane food, am i right?\"");

string comedy_gold = get<string>(LUA_INST.GetGlobal("funny_quote"));
if (comedy_gold != string{"Airplane food, am i right?"}){
    throw "Test 2 failed!";
}


LUA_INST.DoString("ToKVpair = function(key, value)\n"
"local result_table = {[key] = value}\n"
"return result_table\n"
"end\n");
lua_Function key_value_function = get<lua_Function>(LUA_INST.GetGlobal("ToKVpair"));

lua_Value table_pair = LUA_INST.DoFunction(key_value_function, {false, "Nope!"}, nullopt);

lua_Value table_pair_value = get<shared_ptr<lua_Table>>(table_pair)->table_contents.at(false);
if (get<string>(table_pair_value) != string{"Nope!"}){
    throw "Test 3 failed!";
}
return 0;
}
/*

lua.DoString(
"function TracebackTest3()\n"
"local z = 3\n"
"error(\"skill issue\")\n"
"end\n"
"function TracebackTest2()\n"
"local y = 2\n"
"TracebackTest3()\n"
"end\n"
"function TracebackTest1()\n"
"local x = 1\n"
"TracebackTest2()\n"
"end\n"
);
auto traceback_tester = lua.GetGlobal("TracebackTest1");
lua.DoFunction(get<lua_Function>(traceback_tester), {}, "TracebackTester");
*/