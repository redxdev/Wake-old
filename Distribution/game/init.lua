-- Game initialization
-- Add any code for game initialization here

-- Default initialization code
require "defaults"

local s,e = Shader.load("assets/shaders/basic.vert", "assets/shaders/basic.frag")
if e ~= nil then
  print("Error: " .. e)
else
  print(s)
end
