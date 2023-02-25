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

int StringOfAs(lua_State* L){
    vector<any> arguments = lua.GetArguments({LUA_TNUMBER});
    string help_aaaa;
    int count = any_cast<lua_Number>(arguments[0]);
    for (int itr = 0; itr != count; itr++){
        help_aaaa.append("A");
    };
    lua.MakeLuaVariable.String(help_aaaa);
    return 1;
}

int main(){

lua.DoString("for i=1,10 do print(\"hi\") end");

lua.AddGlobalVariable.Number(80, "some_number");
lua.DoString("print(some_number)");

std::string goofy_words = "heya scott here";
lua.AddGlobalVariable.String(goofy_words, "some_string");
lua.DoString("print(some_string)");

int magic_conch = 0;
lua.AddGlobalVariable.Boolean(magic_conch, "some_bool");
lua.DoString("print(\"The earth is flat? \" .. tostring(some_bool))");
    

lua.DoString("print(\"Your girlfriend is \" .. tostring(girlfriend))");

unordered_map<string,string> compound{
    {"scoundrel","polish"},
    {"zak","british"},
    {"bobot","serbian"},
    {"goose","georgian"}
};

lua.DoString("some_table = {} some_table.a_goofy_field = 20 some_table[2] = \"this is an index\" some_table.anotha_table = {} some_table.anotha_table.balls_status = \"itching\" ");
lua_getglobal(*(lua.pointer_to_lua_state), "some_table");
unordered_map<variant<string,lua_Integer>,any> cpp_table = lua.MakeLuaVariable.ParseTable();
cout << any_cast<lua_Number>(cpp_table.at("a_goofy_field")) << endl;

cout << any_cast<string>(cpp_table.at(2)) << endl;

auto subtable = any_cast<unordered_map<variant<string,lua_Integer>,any>>(cpp_table.at("anotha_table"));
cout << any_cast<string>(subtable.at("balls_status")) << endl; 

class UserdataTestClass{
    private:
    int funny_number = 69;

    public:
    int GetFunnyNumber (lua_State* L){
        lua_settop(L, 0);
        lua.MakeLuaVariable.Number(funny_number);
        return 1;
    }
};

lua.AddGlobalVariable.Function(&StringOfAs, "scream");

lua.DoString("help_me = scream(10) print(help_me)");

UserdataTestClass test_object;
any test_container = test_object;

unordered_map<variant<string, lua_Integer>,any> my_metatable;
int (UserdataTestClass::* metamethod_ptr)(lua_State*) = &UserdataTestClass::GetFunnyNumber;
my_metatable["__call"] = metamethod_ptr;

lua.AddGlobalVariable.Metatable(my_metatable, "test_metatable");
lua.AddGlobalVariable.Userdata(test_container,"my_userdata", "test_metatable");

lua.DoString("somehow_table = my_userdata()");
return 0;
}