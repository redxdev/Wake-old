-- Class Framework

local function class(base)
  base = base or nil

  if base ~= nil and type(base) ~= "table" then error("base must be a table") end

  local c = {}
  local c_mt = { __index = c }

  local function create(self, ...)
    local inst = {}
    setmetatable(inst, c_mt)
    inst:__construct(...)
    return inst
  end

  function c:__construct()
  end

  function c:class()
    return c
  end

  function c:super()
    return base
  end

  function c:instanceOf(lookingFor)
    local current = c
    while current ~= nil do
      if current == lookingFor then
        return true
      else
        current = current:super()
      end
    end

    return false
  end

  setmetatable(c, { __index = base, __call = create })

  return c
end

return class
