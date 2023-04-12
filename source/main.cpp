#include "LuaSimple/LuaInstance.hpp"
#include <string>
#include <utility>
#include <unordered_map>
#include <any>
#include <variant>
#include <iostream>
#include <vector>
#include "LuaSimple/LuaTypeClasses.hpp"
#include <cmath>
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
lua_Integer some_number;
public:
TestUserdata(lua_Integer number) : some_number(number) {
}

static int Increment (lua_State* lua_ptr){
    LuaInstance& current_lua = LuaInstance::GetLuaData(lua_ptr, {LUA_TUSERDATA});
    lua_Userdata userdata_self = get<lua_Userdata>(current_lua.lua_argument_values[0]);
    TestUserdata& self = any_cast<TestUserdata&>(*(userdata_self.object));
    self.some_number++;
    current_lua.ReturnResults({self.some_number});
    return 1;
}

};

/*int*/ int DigitSequenceNumber(lua_State* lua_ptr){
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
    string result{};
    for (int idx = 1; not (idx > length); idx++){
        result.append(to_string(idx % 10));
    }
    lua_object.ReturnResults({stoll(result,nullptr,10)});
    return 1;
}


/*bool*/ int IsAlphabetical(lua_State* lua_ptr){
    const string ALPHABET{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

    auto& current_lua = LuaInstance::GetLuaData(lua_ptr, {LUA_TSTRING});
    string& checked_string = get<string>(current_lua.lua_argument_values[0]);
    bool follows_rules = true;
    for (int idx = 0; idx != checked_string.size(); idx++){
        if (idx != 0){
            if (ALPHABET.find(toupper(checked_string.at(idx)),0) < ALPHABET.find(toupper(checked_string.at(idx-1)),0)) {
                follows_rules = false;
            }
        }
    };
    return current_lua.ReturnResults({follows_rules});
} 

/*lua_Table*/ int CallLuaOnContents(lua_State* lua_ptr){
    auto& current_lua = LuaInstance::GetLuaData(lua_ptr, {LUA_TTABLE, LUA_TFUNCTION});
    shared_ptr<lua_Table> table_to_call_on = get<shared_ptr<lua_Table>>(current_lua.lua_argument_values[0]);
    shared_ptr<lua_Table> results_table = make_shared<lua_Table>();
    if (holds_alternative<lua_Function>(current_lua.lua_argument_values[1])){
        lua_Function input_function = get<lua_Function>(current_lua.lua_argument_values[1]);
        for (auto kv_pair : table_to_call_on->table_contents){
            current_lua.DoFunction(input_function, {kv_pair.first, kv_pair.second}, nullopt);
            results_table->table_contents.insert({kv_pair.first, current_lua.lua_return_values[0]});
            current_lua.lua_return_values.clear();
        }
    }
    return current_lua.ReturnResults({results_table});
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
LUA_INST.DoFunction(key_value_function, {false, "Nope!"}, nullopt);

lua_Value table_pair = LUA_INST.lua_return_values[0];

lua_Value table_pair_value = get<shared_ptr<lua_Table>>(table_pair)->table_contents.at(false);
if (get<string>(table_pair_value) != string{"Nope!"}){
    throw "Test 3 failed!";
}


shared_ptr<lua_Table> add_method_metatable = make_shared<lua_Table>();

LUA_INST.DoString("table_add = function(operand1, operand2)\n"
"return operand1[1] + operand2[1]\n"
"end\n");
add_method_metatable->table_contents.insert({"__add", LUA_INST.GetGlobal("table_add")});

LUA_INST.SetMetatable(add_method_metatable, "numeric_table");

shared_ptr<lua_Table> number_table1 = make_shared<lua_Table>();
number_table1->table_contents.insert({1, 10.5});
number_table1->metatable_name = "numeric_table";

shared_ptr<lua_Table> number_table2 = make_shared<lua_Table>();
number_table2->table_contents.insert({1, 15.25});
number_table2->metatable_name = "numeric_table";

LUA_INST.SetGlobal(number_table1, "num_table1");
LUA_INST.SetGlobal(number_table2, "num_table2");
LUA_INST.DoString("return num_table1 + num_table2");

if (get<lua_Number>(LUA_INST.lua_return_values[0]) != 25.75){
    throw "Test 4 failed!";
}

LUA_INST.SetGlobal(&DigitSequenceNumber, "DigitSequenceNumber");
LUA_INST.SetGlobal(&IsAlphabetical, "IsAlphabetical");
LUA_INST.SetGlobal(&CallLuaOnContents, "CallLuaOnContents");
//LUA_INST.DoFile("..\\..\\source\\CFunctionCalls.lua");
LUA_INST.DoFile("C:\\Users\\Bruger\\Skrivebord\\LuaSimple\\source\\CFunctionCalls.lua");
//LUA_INST.DoFile("C:\\Users\\Asger\\Desktop\\programming\\LuaSimple\\source\\CFunctionCalls.lua");

shared_ptr<lua_Table> expected_table = make_shared<lua_Table>();
expected_table->table_contents[true] = true;
expected_table->table_contents["score"] = false;
expected_table->table_contents["team"] = true;
expected_table->table_contents[20LL] = false;

if ((get<lua_Integer>(LUA_INST.lua_return_values[0]) != 1234567890) or 
(get<bool>(LUA_INST.lua_return_values[1]) != false) or 
(get<shared_ptr<lua_Table>>(LUA_INST.lua_return_values[2])->table_contents != expected_table->table_contents)){
    throw "Test 5 failed!";
}


LUA_INST.DoString(
"function TracebackTest3()\n"
"error(\"Attempted to fix a bug value\")\n"
"end\n"
"function TracebackTest2()\n"
"TracebackTest3()\n"
"end\n"
"function TracebackTest1()\n"
"TracebackTest2()\n"
"end\n"
);
auto traceback_tester = LUA_INST.GetGlobal("TracebackTest1");
LUA_INST.DoFunction(get<lua_Function>(traceback_tester), {}, "TracebackTester");


shared_ptr<lua_Table> member_functions_metatable = make_shared<lua_Table>();
shared_ptr<lua_Table> index_metamethod = make_shared<lua_Table>();
index_metamethod->table_contents.insert({string{"Increment"}, &TestUserdata::Increment});
member_functions_metatable->table_contents.insert({"__index", index_metamethod});

LUA_INST.SetMetatable(member_functions_metatable, "TestUserdata");

lua_Userdata test_udata_object;
TestUserdata test_cpp_object{7};
any wrapped_cpp_object = make_any<TestUserdata>(test_cpp_object);
test_udata_object.metatable_name = "TestUserdata";
test_udata_object.object = &wrapped_cpp_object;

LUA_INST.SetGlobal(test_udata_object, "cpp_object");
//LUA_INST.DoFile("..\\..\\source\\UsingUserdata.lua");
LUA_INST.DoFile("C:\\Users\\Bruger\\Skrivebord\\LuaSimple\\source\\UsingUserdata.lua");
//LUA_INST.DoFile("C:\\Users\\Asger\\Desktop\\programming\\LuaSimple\\source\\UsingUserdata.lua");
if (get<lua_Integer>(LUA_INST.lua_return_values[0]) != 16){
    throw "Test 6 failed!";
}


LuaInstance LUA_INST_2;
lua_Value should_be_empty = LUA_INST_2.GetGlobal("funny_quote");
LUA_INST.SetGlobal("OnefivenineThreeTwo", "NumberWords");
LUA_INST_2.SetGlobal("twoEighttwothree", "NumberWords");

//LUA_INST.DoFile("..\\..\\source\\TwoInstances.lua");
LUA_INST.DoFile("C:\\Users\\Bruger\\Skrivebord\\LuaSimple\\source\\TwoInstances.lua");
//LUA_INST.DoFile("C:\\Users\\Asger\\Desktop\\programming\\LuaSimple\\source\\TwoInstances.lua");
//LUA_INST_2.DoFile("..\\..\\source\\TwoInstances.lua");
LUA_INST_2.DoFile("C:\\Users\\Bruger\\Skrivebord\\LuaSimple\\source\\TwoInstances.lua");
//LUA_INST_2.DoFile("C:\\Users\\Asger\\Desktop\\programming\\LuaSimple\\source\\TwoInstances.lua");

if ((get<lua_Integer>(LUA_INST.lua_return_values[0]) != 15932) or 
(get<lua_Integer>(LUA_INST_2.lua_return_values[0]) != 2823) or 
(get<nullptr_t>(should_be_empty) != nullptr)){
    throw "Test 7 failed!";
}
return 0;
}


