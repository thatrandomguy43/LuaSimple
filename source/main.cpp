#include "LuaSimple/LuaInstance.hpp"
#include <string>
#include <utility>
#include <unordered_map>
#include <any>
#include <variant>
#include <iostream>
#include <vector>
#include "LuaSimple/LuaFunction.hpp"
using namespace std;

LuaInstance lua;

int StringOfAs(lua_State* Lua){
    LuaInstance& current_lua = LuaInstance::FindInstance(Lua);
    current_lua.GetArguments({LUA_TNUMBER});
    string help_aaaa;
    int count = any_cast<lua_Number>(current_lua.lua_argument_values[0]);
    for (int itr = 0; itr != count; itr++){
        help_aaaa.append("A");
    };
    vector<any> return_vals;
    return_vals.push_back(make_any<string>(help_aaaa));
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
    any* packed_self = static_cast<any*>(lua_touserdata(lua.pointer_to_lua_state, 1));
    TestUserdata* self = any_cast<TestUserdata>(packed_self);
    vector<any> return_vals;
    return_vals.push_back(make_any<lua_Number>(self->funny_number));
    current_lua.ReturnResults(return_vals);
    return 1;
}

};





int main(){

lua.DoString("for i=1,10 do print(\"hi\") end");

lua.SetGlobal.Number(80, "some_number");
lua.DoString("print(some_number)");

string goofy_words = "heya scott here";
lua.SetGlobal.String(goofy_words, "some_string");
lua.DoString("print(some_string)");

int magic_conch = 0;
lua.SetGlobal.Boolean(magic_conch, "some_bool");
lua.DoString("print(\"The earth is flat? \" .. tostring(some_bool))");
    

lua.DoString("print(\"Your girlfriend is \" .. tostring(girlfriend))");

unordered_map<string,string> compound{
    {"scoundrel","polish"},
    {"zak","british"},
    {"bobot","serbian"},
    {"goose","georgian"}
};

lua.DoString("some_table = {} some_table.a_goofy_field = 20 some_table[2] = \"this is an index\" some_table.anotha_table = {} some_table.anotha_table.balls_status = \"itching\" ");

unordered_map<variant<string,lua_Integer>,any> cpp_table = lua.GetGlobal.Table("some_table");
cout << any_cast<lua_Number>(cpp_table.at("a_goofy_field")) << endl;

cout << any_cast<string>(cpp_table.at(2)) << endl;

auto subtable = any_cast<unordered_map<variant<string,lua_Integer>,any>>(cpp_table.at("anotha_table"));
cout << any_cast<string>(subtable.at("balls_status")) << endl; 

any ref_test = make_any<double>(69.420);
double* retrieved_value_ref_test = any_cast<double>(&ref_test);
*retrieved_value_ref_test = 54.321;
cout << any_cast<double>(ref_test) << endl;

lua.SetGlobal.Function(&StringOfAs, "scream");

lua.DoString("help_me = scream(10) print(help_me)");


unordered_map<variant<string, lua_Integer>,any> my_metatable;
int (*metamethod_ptr)(lua_State*) = &TestUserdata::GetFunnyNumber;
TestUserdata my_object;

my_metatable["__call"] = make_any<lua_CFunction>(metamethod_ptr);

lua.SetGlobal.Metatable(my_metatable, "test_metatable");
lua.SetGlobal.Userdata(my_object,"my_userdata", "test_metatable");

lua.DoString("print(my_userdata(my_userdata))");

LuaFunction function_info = lua.GetGlobal.Function("print");

cout << boolalpha << "Stored in registry at: " << function_info.registry_key << " Argument count: " << function_info.argument_count << " Takes extra parameters: " << function_info.takes_extra_args << endl;

//lua.DoFile("C:/Users/Asger/Desktop/programming/LuaSimple/source/funny file.lua");
lua.DoFile("C:/Users/Bruger/Skrivebord/LuaSimple/source/funny file.lua");

cout << any_cast<lua_Number>(lua.lua_return_values[0]) << endl;

lua.DoFunction(function_info, {make_any<string>("my "), make_any<string>("balls "), make_any<string>("itch")});
return 0;
}