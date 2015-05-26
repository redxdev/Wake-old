-- Class Framework

local function Class(base)
  base = base or nil

  if base ~= nil and type(base) ~= "table" then error("base must be a table") end

  local class = {}
  local class_mt = { __index = class }

  local function create(self, ...)
    local inst = {}
    setmetatable(inst, class_mt)
    inst:__construct(...)
    return inst
  end

  function class:class()
    return class
  end

  function class:super()
    return base
  end

  function class:instanceOf(lookingFor)
    local current = class
    while current ~= nil do
      if current == lookingFor then
        return true
      else
        current = current:super()
      end
    end

    return false
  end

  setmetatable(class, { __index = base, __call = create })

  return class
end

return Class
