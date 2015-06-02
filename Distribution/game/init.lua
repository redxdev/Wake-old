-- Game initialization
-- Add any code for game initialization here

-- Default initialization code
require "defaults"

local s,e = Shader.load("assets/shaders/basic.vert", "assets/shaders/basic.frag")
if e ~= nil then
  print("Error: " .. e)
else
  print(s)
  local u = s:getUniform("inColor")
  if u ~= nil then
    print(u)
    u:setMatrix2({1, 2, 3, 4})
    print(Matrix2x2.new{1, 2, 3, 4})
  else
    print("Error: bad uniform")
  end
end
