local userdata_return = cpp_object:Increment()
if (userdata_return % 2) == 0 then
    return userdata_return * 2
else
    return userdata_return
end
