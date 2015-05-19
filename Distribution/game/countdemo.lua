local i = 0
local nextTime = 5
local function tick()
    nextTime = nextTime - engine.getDeltaTime()
    if nextTime <= 0 then
        nextTime = 5 + nextTime
        i = i + 1
        print(i)
    end
end

engine.tick:bind(tick)
