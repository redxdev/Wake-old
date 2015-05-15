-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

engine.tick:bind(function() engine.stop() end)

local a = Matrix4x4.new(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)
local b = Matrix4x4.new{8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7}
local c = Matrix4x4.new(a)
local d = Matrix4x4.new()

print(a)
print(b)
print(c)
print(d)

print(a==b)
print(a==c)

for _,v in ipairs(a:table()) do
  print(v)
end

print(a)
print(a:get(4))
print(a:get(4,4))
a:set(4,4,100)
print(a:get(4,4))
a:setAll(10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160)
print(a)
print(#a)
print(a:rows())
print(a:columns())
