NumberOfTanks = 25
allTheTanks = {}
Tank = {}

function Tank:setPosition(x, y, z)
    self.position.x = x
    self.position.y = y
    self.position.z = z
    setEntityPosition(self.index, x, y, z)
end

function Tank:update()
    self.position.x = self.position.x + self.direction.x
    self.position.y = self.position.y + self.direction.y
    self.position.z = self.position.z + self.direction.z
    setEntityPosition(self.index, self.position.x,
        self.position.y, self.position.z)
    
    if self.position.x < 0.5 and self.direction.x < 0 then
        self.direction.x = -self.direction.x
    end
    
    if self.position.x > 18.5 and self.direction.x > 0 then
        self.direction.x = -self.direction.x
    end
    
    if self.position.y < 0.5 and self.direction.y < 0 then
        self.direction.y = -self.direction.y
    end
    
    if self.position.y > 18.5 and self.direction.y > 0 then
        self.direction.y = -self.direction.y
    end    
end

function Tank:setDirection(x, y, z)
    self.direction.x = x
    self.direction.y = y
    self.direction.z = z
end

function Tank:getMass()
    return getEntityMass(self.index)
end

function Tank:setMass(mass)
    setEntityMass(self.index, mass)
end

function Tank:new()
    local newTank = {
        index = addEntity(),
        position = { x = 0, y = 0, z = 0 },
        direction = { x = 0, y = 0, z = 0 },
        setPosition = Tank.setPosition,
        setDirection = Tank.setDirection,
        getMass = Tank.getMass,
        setMass = Tank.setMass,
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
        local chance = math.random()
        if chance > 0.992  then
            allTheTanks[i]:setDirection(randomDirection(),
                randomDirection(), 0)
        end
        allTheTanks[i]:update()
    end
end

function allTheThings()
    for i = 1, NumberOfTanks do
        allTheTanks[i] = Tank:new()
        allTheTanks[i]:setPosition(randomLocation(),
            randomLocation(), 0)
        allTheTanks[i]:setDirection(randomDirection(),
            randomDirection(), 0)
        allTheTanks[i]:setMass(math.random() + 0.1)
        print("tank mass = " .. allTheTanks[i]:getMass())
    end
end

allTheThings()
