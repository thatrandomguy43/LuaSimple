#include "LuaSimple/LuaInstance.hpp"
#include <string>
#include <utility>
#include <unordered_map>
#include <any>
#include <variant>
#include <iostream>
#include <vector>
using namespace std;

LuaInstance lua;

int StringOfAs(lua_State* Lua){
    vector<any> arguments = lua.GetArguments({LUA_TNUMBER});
    string help_aaaa;
    int count = any_cast<lua_Number>(arguments[0]);
    for (int itr = 0; itr != count; itr++){
        help_aaaa.append("A");
    };
    lua.PushVariable.String(help_aaaa);
    return 1;
}

class TestUserdata
{
private:
int funny_number = 13;
public:

static int GetFunnyNumber (lua_State* L){
    any* self = static_cast<any*>(lua_touserdata(lua.pointer_to_lua_state, 1));
    //any_cast<TestUserdata>(*self);
    lua.PushVariable.Number(69);
    return 1;
}

};





int main(){

int* asan_fail_generator = new int;
delete asan_fail_generator;
// *asan_fail_generator = 4;


lua.DoString("for i=1,10 do print(\"hi\") end");

lua.SetGlobal.Number(80, "some_number");
lua.DoString("print(some_number)");

std::string goofy_words = "heya scott here";
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

lua_getglobal(lua.pointer_to_lua_state, "print");
tuple<int,int,bool> function_info = lua.GetVariable.Function();

cout << boolalpha << "Stack position: " << get<0>(function_info) << " Argument count: " << get<1>(function_info) << " Takes extra parameters: " << get<bool>(function_info) << endl;
return 0;
}