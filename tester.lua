-- local cc = package.loadlib("./test.so", "luaopen_test")()
local cc = require 'test'
x = cc.c_add(32, 37)
print(x)


local matrix = {{1.1, 2.2}, {3.3, 4.4}}
cc.c_parse_array_double(matrix)

-- local matrix1 = {{1.1}, {3.3, 4.4}}
-- cc.c_parse_array_double(matrix1)

local minha_matriz = {
    {"Ação", "Atenção"},
    {"Coração", "Açúcar"}
}

cc.c_parse_array_string(minha_matriz)

-- cc.parse_2d_double();

local breno = { 
    breno = 'skibidi',
    { 1, 2, 3, 4},
    momo = 'fiote',
    numbers = { 4, 3, 2, 1}
}


print(breno.numbers[3])