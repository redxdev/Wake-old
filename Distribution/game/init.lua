-- Game initialization
-- Add any code for game initialization here

-- Default initialization code
require "defaults"

local Camera = require "render/camera"

local cam = Camera({0, 3, 0}, {10, 0, 0})
cam:activate()
