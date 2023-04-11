local c_call_returns = {}
c_call_returns.sequence_number = DigitSequenceNumber(10)
c_call_returns.alphabetaicallity = IsAlphabetical("A beehive")
function KVTypeMatch(key, value)
    return type(key) == type(value)
end
local multityped_table = {[true] = false,
["score"] = 42,
["team"] = "defenders",
[20] = "June 20th"}
c_call_returns.type_data = CallLuaOnContents(multityped_table, KVTypeMatch)
return c_call_returns.sequence_number, c_call_returns.alphabetaicallity, c_call_returns.type_data