-- Game initialization
-- Add any code for game initialization here

-- Uncomment requires to see demos
require "inputdemo"

local function gameWindowClosed()
  engine.stop()
end

window.closed:bind(gameWindowClosed)

local a = Vector4.new(1, 2, 3, 4)
local b = Vector4.new{3, 2, 1, 6}
local c = Vector4.new(a)
local d = Vector4.new()

print(a)
print(b)
print(c)
print(d)

print(a == b)
print(a == c)

for _,v in ipairs(a:table()) do
  print(v)
end

for i = 1,#a do
  print(a:get(i))
end

a:set(4, 10)
print(a:get(4))

a:setAll(5, 6, 7, 9)
print(a)

print(a:dot(b))
print(a:distance(b))
print(a:length())
print(a:apply(function(v) return v + 20 end))
print(a:normalize())
print(a:reflect(b))
print(a:refract(b, 0.5))

print(a + 1)
print(1 + a)
print(a + b)

print(a - 1)
print(1 - a)
print(a + b)

print(a * 2)
print(2 * a)
print(a * b)

print(a / 2)
print(2 / a)
print(a / b)
