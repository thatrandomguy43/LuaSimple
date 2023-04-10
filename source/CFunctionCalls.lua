local c_call_returns = {}
print("line 2")
c_call_returns.sequence_number = DigitSequenceNumber(10)
print("line 4")
c_call_returns.alphabetaicallity = IsAlphabetical("A beehive")
print("line 6")
function KVTypeMatch(key, value)
    return type(key) == type(value)
end
print("line 10")
local multityped_table = {[true] = false,
["score"] = 42,
["team"] = "defenders",
[20] = "June 20th"}
print("line 15")
c_call_returns.type_data = CallLuaOnContents(multityped_table, KVTypeMatch)
print("line 17", c_call_returns.sequence_number, c_call_returns.alphabetaicallity, c_call_returns.type_data)
return c_call_returns.sequence_number, c_call_returns.alphabetaicallity, c_call_returns.type_data