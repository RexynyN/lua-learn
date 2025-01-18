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
assert(not pcall( ftp_path.currentdir ) )



-- local funk = io.read("l")


local juul = { __version = 123 }


function juul.penjamin(pack) 
    print("I'm off of that " .. pack .. " pack")
end



print(juul.penjamin("Dark Evil"))