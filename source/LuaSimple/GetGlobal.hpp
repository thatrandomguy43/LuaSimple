#include <memory>
#include <string>
#include "luainclude/lua.hpp"
class GetGlobal{
private:
std::shared_ptr<lua_State*> pointer_to_lua_state;
public:

GetGlobal(std::shared_ptr<lua_State*> L){
    this->pointer_to_lua_state = L;
}

};