require("os")

tt= os.date('*t')
for k, v in pairs(tt) do
    print(k, v)
end

--[[ for i=1, #t do
    print(t[i])
end ]]