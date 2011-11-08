NumberOfTanks = 25
allTheTanks = {}

Tank = {}

function Tank:getPosition()
    return getEntityPosition(self.index)
end

function Tank:setPosition(x, y, z)
    setEntityPosition(self.index, x, y, z)
end

function Tank:getVelocity()
    return getEntityVelocity(self.index)
end

function Tank:setVelocity(x, y, z)
    return setEntityVelocity(self.index, x, y, z)
end

function Tank:update()
    local px = 0
    local py = 0
    local pz = 0
    
    px, py, pz = self:getPosition()
    
    local vx = 0
    local vy = 0
    local vz = 0
    
    vx, vy, vz = self:getVelocity()
    
    --print(px .. " " .. py .. " " .. pz .. " " .. vx .. " " .. vy .. " " .. vz)
    
    if px < 0.5 and vx < 0 then self:setVelocity(-vx, vy, vz) end
    if px > 18.5 and vx > 0 then self:setVelocity(-vx, vy, vz) end
    if py < 0.5 and vy < 0 then self:setVelocity(vx, -vy, vz) end
    if py > 18.5 and vy > 0 then self:setVelocity(vx, -vy, vz) end    
end

function Tank:getMass()
    return getEntityMass(self.index)
end

function Tank:setMass(mass)
    setEntityMass(self.index, mass)
end

function Tank:getRadius()
    return getEntityRadius(self.index)
end

function Tank:setRadius(radius)
    setEntityRadius(self.index, radius)
end

function Tank:new()
    local newTank = {
        index = addEntity(),
        getPosition = Tank.getPosition,
        setPosition = Tank.setPosition,
        getVelocity = Tank.getVelocity,
        setVelocity = Tank.setVelocity,
        getMass = Tank.getMass,
        setMass = Tank.setMass,
        getRadius = Tank.getRadius,
        setRadius = Tank.setRadius,
        update = Tank.update
        }
    
    setEntityDefaultRotation(newTank.index, 90, 0, 0)
    addActor(newTank.index, "bradley.c3m")
    
    return newTank
end

function randomLocation()
    return math.random() * 20
end

function randomDirection()
    return (math.random() - 0.5) * 0.05
end

function update()
    for i = 1, NumberOfTanks do
        allTheTanks[i]:update()
        
        local chance = math.random()
        if chance > 0.992  then
            allTheTanks[i]:setVelocity(randomDirection(),
                randomDirection(), 0)
        end
    end
end

function sayHello()
    print("Hello!")
end

function sayThings(inTable)
    print(inTable.message)
end

function allTheThings()
    for i = 1, NumberOfTanks do
        allTheTanks[i] = Tank:new()
        allTheTanks[i]:setPosition(randomLocation(),
            randomLocation(), 0)
        allTheTanks[i]:setVelocity(randomDirection(),
            randomDirection(), 0)
        
        local x = 0
        local y = 0
        local z = 0
        
        x, y, z = allTheTanks[i]:getVelocity()
        
        --print("tank velocity : " .. x .. " " .. y .. " " .. z)
    end
    
    local a = { cash = 100 }
    print("table reference: ")
    print(a)
    sendTableToC(a)
    
    print("function reference: ")
    print(sayHello)
    sendFunctionToC(sayHello)
    print("testing it on a table:")
    sendFunctionToC(a)
    
    local t = { message = "HUZZAH!" }
    sendBoth(sayThings, t, 1, 3, nil, "bai")
end

allTheThings()

