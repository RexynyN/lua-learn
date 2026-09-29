-- local parser = require "json"
local path = require "path"

local str = require "string"

local it = str.gmatch("gwagon", "g")


-- we can use system dependet function
print(path.user_home())  -- C:\Documents and Settings\Admin
-- print(path.currentdir()) -- C:\lua\5.1

-- but we can use specific system path notation
local ftp_path = path.new(path.currentdir())
print(ftp_path.join("/root", "some", "dir")) -- /root/some/dir

-- All functions specific to system will fail
assert(not pcall(ftp_path.currentdir))



-- local funk = io.read("l")


local juul = { __version = 123 }


function juul.penjamin(pack) 
    print("I'm off of that " .. pack .. " pack")
end

print(juul.penjamin("Dark Evil"))


-- Lua OOP example
-- In Lua, objects are tables and classes are tables with metatables.
local Animal = {}
Animal.__index = Animal

function Animal:new(name)
    local obj = setmetatable({}, self)
    obj.name = name
    return obj
end

function Animal:speak()
    print(self.name .. " makes a sound.")
end

local Dog = setmetatable({}, Animal)
Dog.__index = Dog

function Dog:new(name, breed)
    local obj = Animal.new(self, name)
    obj.breed = breed
    return obj
end

function Dog:speak()
    print(self.name .. " barks! Woof woof!")
end

local Cat = setmetatable({}, Animal)
Cat.__index = Cat

function Cat:new(name)
    local obj = Animal.new(self, name)
    return obj
end

function Cat:speak()
    print(self.name .. " meows.")
end

local a = Animal:new("Animal")
a:speak()

local d = Dog:new("Rex", "Husky")
d:speak()
print(d.name, d.breed)

local c = Cat:new("Milo")
c:speak()

-- You can also create methods that use self.
print("Class type:", type(d))




print()