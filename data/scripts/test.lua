NumberOfTanks = 200
allTheTanks = {}

terrainSizeX = 100
terrainSizeY = 100

Entity = {}

function Entity:setCollisionCallback(callback)
    setEntityCollisionCR(self.index, callback)
end

function Entity:getPosition()
    return getEntityPosition(self.index)
end

function Entity:setPosition(x, y, z)
    setEntityPosition(self.index, x, y, z)
end

function Entity:getVelocity()
    return getEntityVelocity(self.index)
end

function Entity:setVelocity(x, y, z)
    return setEntityVelocity(self.index, x, y, z)
end

function Entity:getMass()
    return getEntityMass(self.index)
end

function Entity:setMass(mass)
    setEntityMass(self.index, mass)
end

function Entity:getRadius()
    return getEntityRadius(self.index)
end

function Entity:setRadius(radius)
    setEntityRadius(self.index, radius)
end

function Entity:new()
    local newEntity = {
        setCollisionCallback = Entity.setCollisionCallback,
        getPosition = Entity.getPosition,
        setPosition = Entity.setPosition,
        getVelocity = Entity.getVelocity,
        setVelocity = Entity.setVelocity,
        getMass = Entity.getMass,
        setMass = Entity.setMass,
        getRadius = Entity.getRadius,
        setRadius = Entity.setRadius
        }
        
    return newEntity
end

Tank = {}

function Tank:update()
    local px, py, pz = self:getPosition()    
    local vx, vy, vz = self:getVelocity()
    
    resetEntityActorMatrix(self.index, self.actors.body)
    --setEntityPosition(self.index, px, py, pz)
    setEntityActorRotation(self.index, self.actors.body, 0, px * 10, 0)
    
    if px < 0.5 and vx < 0 then self:setVelocity(-vx, vy, vz) end
    if px > terrainSizeX - 1.5 and vx > 0 then self:setVelocity(-vx, vy, vz) end
    if py < 0.5 and vy < 0 then self:setVelocity(vx, -vy, vz) end
    if py > terrainSizeY - 1.5 and vy > 0 then self:setVelocity(vx, -vy, vz) end
end

function Tank:onCollision(entity)
    local px, py, pz = self:getPosition()    
    local vx, vy, vz = self:getVelocity()    
    local px2, py2, pz2 = entity:getPosition()    
    local vx2, vy2, vz2 = entity:getVelocity()
    
    if px < px2 and vx > 0 then vx = -vx end
    if px > px2 and vx < 0 then vx = -vx end
    
    if py > py2 and vy < 0 then vy = -vy end
    if py < py2 and vy > 0 then vy = -vy end
    
    self:setVelocity(vx, vy, vz)
end

function Tank:new()
    local newTank = Entity:new()
    newTank.update = Tank.update
    newTank.onCollision = Tank.onCollision
		
	newTank.index = addEntity(newTank)
    
    setEntityDefaultRotation(newTank.index, 90, 0, 0)
    newTank:setRadius(0.5)
    
    newTank.actors = {}
    newTank.actors.body = addActor(newTank.index,
        "data/models/bradley_body.c3m")
    newTank.actors.head = addActor(newTank.index,
        "data/models/bradley_head.c3m", newTank.actors.body)
    newTank.actors.turret = addActor(newTank.index,
        "data/models/bradley_turret.c3m")
    
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
            allTheTanks[i]:setVelocity(randomDirection(), randomDirection(), 0)
        end
    end
end

function onMoveForward(intensity)
    print("command callback -- " .. intensity)
end

function allTheThings()
	setTerrainSize(terrainSizeX, terrainSizeY)
	createCommand("Move Forward", onMoveForward)
    for i = 1, NumberOfTanks do
        local t = Tank:new()
        t:setPosition(randomLocation(), randomLocation(), 0)
        t:setVelocity(randomDirection(), randomDirection(), 0)
        allTheTanks[i] = t
        
        local x, y, z = allTheTanks[i]:getVelocity()
    end
    
    setUpdateCallback(update)
end

allTheThings()

