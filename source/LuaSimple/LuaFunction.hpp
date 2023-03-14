// will be used to store or pass any lua function, stored with an integer key in the registry theough luaL_ref()

#pragma once
class lua_Function
{
public:
    int registry_key;
    int argument_count;
    bool takes_extra_args;
};