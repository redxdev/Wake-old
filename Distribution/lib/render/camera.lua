local Class = require "class"

local Camera = Class()

function Camera:__construct(pos, point)
  self.position = Vector3.new(pos) or Vector3.new(0, 0, 0)
  self.point = Vector3.new(point) or Vector3.new(1, 0, 0)
  self.up = Vector3.new(up) or Vector3.new(0, 1, 0)
  self.fov = 45
  self.nearPlane = 0.1
  self.farPlane = 100
  self.aspectRatio = window.getWidth() / window.getHeight()
end

function Camera:activate()
  engine.draw:bind(self, self.draw)
end

function Camera:deactivate()
  engine.draw:unbind(self, self.draw)
end

function Camera:draw()
  engine.setViewMatrix(math.lookAt(self.position, self.point, self.up))
  engine.setProjectionMatrix(math.perspective(self.fov, self.aspectRatio, self.nearPlane, self.farPlane))
end

return Camera
