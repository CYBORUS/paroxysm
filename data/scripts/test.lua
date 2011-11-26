NumberOfTanks = 200
allTheTanks = {}

terrainSizeX = 100
terrainSizeY = 100

Tank = { mt = {} }
Tank.mt.__index = Tank

function Tank:setCollisionCallback(callback)
    setEntityCollisionCR(self.index, callback)
end

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
    resetEntityActorMatrix(self.index, self.actors.body)
    --setEntityPosition(self.index, px, py, pz)
    setEntityActorRotation(self.index, self.actors.body, 0, px * 10, 0)
    
    if px < 0.5 and vx < 0 then self:setVelocity(-vx, vy, vz) end
    if px > terrainSizeX - 1.5 and vx > 0 then self:setVelocity(-vx, vy, vz) end
    if py < 0.5 and vy < 0 then self:setVelocity(vx, -vy, vz) end
    if py > terrainSizeY - 1.5 and vy > 0 then self:setVelocity(vx, -vy, vz) end
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

function Tank:onCollision(entity)
    local px = 0
    local py = 0
    local pz = 0
    
    px, py, pz = self:getPosition()
    
    local vx = 0
    local vy = 0
    local vz = 0
    
    vx, vy, vz = self:getVelocity()
    
    local px2 = 0
    local py2 = 0
    local pz2 = 0
    
    px2, py2, pz2 = entity:getPosition()
    
    local vx2 = 0
    local vy2 = 0
    local vz2 = 0
    
    vx2, vy2, vz2 = entity:getVelocity()
    
    if px < px2 and vx > 0 then vx = -vx end
    if px > px2 and vx < 0 then vx = -vx end
    
    if py > py2 and vy < 0 then vy = -vy end
    if py < py2 and vy > 0 then vy = -vy end
    
    self:setVelocity(vx, vy, vz)
end

function Tank:new()
    local newTank = {}
    setmetatable(newTank, Tank.mt)
		
	newTank.index = addEntity(newTank)
    
    setEntityDefaultRotation(newTank.index, 90, 0, 0)
    newTank:setRadius(0.5)
    
    newTank.actors = {}
    newTank.actors.body = addActor(newTank.index, "data/models/bradley_body.c3m")
    newTank.actors.head = addActor(newTank.index, "data/models/bradley_head.c3m", newTank.actors.body)
    newTank.actors.turret = addActor(newTank.index, "data/models/bradley_turret.c3m")
    
    local collisionCallback = function(entity) newTank:onCollision(entity) end
    newTank:setCollisionCallback(collisionCallback)
    
    return newTank
end

function randomLocation()
    return math.random() * terrainSizeX
end

function randomDirection()
    return (math.random() - 0.5) * 0.05
end

function update()
    for i = 1, NumberOfTanks do
        allTheTanks[i]:update()
        
        local chance = math.random()
        if false then
            allTheTanks[i]:setVelocity(randomDirection(),
                randomDirection(), 0)
        end
    end
end

function allTheThings()
	setTerrainSize(terrainSizeX, terrainSizeY)
	createCommand("testing", sayHello)
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
    end
end

allTheThings()

