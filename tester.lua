-- local cc = package.loadlib("./test.so", "luaopen_test")()
local cc = require 'test'
x = cc.c_add(32, 37)
print(x)