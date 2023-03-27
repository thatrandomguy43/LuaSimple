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

LuaInstance lua;




int StringOfAs(lua_State* Lua){
    LuaInstance& current_lua = LuaInstance::FindInstance(Lua);
    current_lua.GetArguments({LUA_TNUMBER});
    string help_aaaa;
    int count = get<lua_Integer>(current_lua.lua_argument_values[0]);
    for (int itr = 0; itr != count; itr++){
        help_aaaa.append("A");
    };
    vector<lua_Value> return_vals;
    return_vals.push_back(help_aaaa);
    current_lua.ReturnResults(return_vals);
    return return_vals.size();
}

class TestUserdata
{
private:
int funny_number = 13;
public:

static int GetFunnyNumber (lua_State* Lua){
    LuaInstance& current_lua = LuaInstance::FindInstance(Lua);
    current_lua.GetArguments({LUA_TUSERDATA});
    lua_Userdata userdata_self = get<lua_Userdata>(current_lua.lua_argument_values[0]);
    TestUserdata* self = any_cast<TestUserdata>(userdata_self.object);
    vector<lua_Value> return_vals;
    return_vals.push_back(self->funny_number);
    current_lua.ReturnResults(return_vals);
    return 1;
}

};





int main(){

lua.DoString("for i=1,10 do print(\"hi\") end");

lua.SetGlobal(80, "some_number");
lua.DoString("print(some_number)");

string goofy_words = "heya scott here";
lua.SetGlobal(goofy_words, "some_string");
lua.DoString("print(some_string)");

int magic_conch = 0;
lua.SetGlobal(magic_conch, "some_bool");
lua.DoString("print(\"The earth is flat? \" .. tostring(some_bool))");
    

lua.DoString("print(\"Your girlfriend is \" .. tostring(girlfriend))");
/*
unordered_map<string,string> compound{
    {"scoundrel","polish"},
    {"zak","british"},
    {"bobot","serbian"},
    {"goose","georgian"}
};
*/
lua.DoString("some_table = {} some_table.a_goofy_field = 20 some_table[2] = \"this is an index\" some_table.anotha_table = {} some_table.anotha_table.balls_status = \"itching\" ");

shared_ptr<lua_Table> cpp_table = get<shared_ptr<lua_Table>>(lua.GetGlobal("some_table"));
cout << get<lua_Integer>(cpp_table->table_contents.at("a_goofy_field")) << endl;

cout << get<string>(cpp_table->table_contents.at(2)) << endl;

auto subtable = get<shared_ptr<lua_Table>>(cpp_table->table_contents.at("anotha_table"));
cout << get<string>(subtable->table_contents.at("balls_status")) << endl; 



lua.SetGlobal(&StringOfAs, "scream");

lua.DoString("help_me = scream(10) print(help_me)");


shared_ptr<lua_Table> my_metatable = make_shared<lua_Table>();
lua_CFunction metamethod_ptr = &TestUserdata::GetFunnyNumber;


my_metatable->table_contents["__call"] = metamethod_ptr;
lua.SetMetatable(my_metatable, "test_metatable");

TestUserdata my_object;
lua_Userdata userdata_object;
any any_my_object = make_any<TestUserdata>(my_object);
userdata_object.object = &any_my_object;
userdata_object.metatable_name = "test_metatable";
lua.SetGlobal(userdata_object,"my_userdata");

lua.DoString("print(my_userdata())");

lua.DoString("function GetCharOfStrings(character_index, ...)\n "
"result_string = \"\"\n "
"inputs = {...}\n "
"for k,v in pairs(inputs) do\n "
"result_string = result_string .. v:sub(character_index, character_index)\n "
"end\n "
"return result_string\n "
"end\n ");

lua_Function function_info = get<lua_Function>(lua.GetGlobal("GetCharOfStrings"));

lua.DoFile("C:/Users/Asger/Desktop/programming/LuaSimple/source/funny file.lua");
//lua.DoFile("C:/Users/Bruger/Skrivebord/LuaSimple/source/funny file.lua");
 
cout << get<lua_Number>(lua.lua_return_values[0]) << endl;

lua.DoFunction(function_info, {2 , "my", "balls", "itch"}, "GetCharOfStrings");

cout << get<string>(lua.lua_return_values[0]) << endl;

lua.DoString("function OnlyOneParam(argument) return (-argument) end");

lua_Function one_param_lua_func = get<lua_Function>(lua.GetGlobal("OnlyOneParam"));
lua.DoFunction(one_param_lua_func, {-6, 7}, "OnlyOneParam");
cout << get<lua_Integer>(lua.lua_return_values[0]) << endl;


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

return 0;
}