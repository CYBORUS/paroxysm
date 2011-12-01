NumberOfTanks = 50
TankSpeed = 0.1
allTheTanks = {}

terrainSizeX = 16
terrainSizeY = 16

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
	
--Something is wrong with this function, needs looking at
function Entity:addVelocity(x1, y1, z1)
	local x2, y2, z2 = self:getVelocity()
	local x = x1 + x2
	local y = y1 + y2
	local z = z1 + z2
	self:setVelocity(x, y, z)
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

function Entity:delete()
    removeEntity(self.index)
end

function Entity:new()
    local newEntity = {
        setCollisionCallback = Entity.setCollisionCallback,
        getPosition = Entity.getPosition,
        setPosition = Entity.setPosition,
        getVelocity = Entity.getVelocity,
        setVelocity = Entity.setVelocity,
		addVelocity = Entity.addVelocity,
        getMass = Entity.getMass,
        setMass = Entity.setMass,
        getRadius = Entity.getRadius,
        setRadius = Entity.setRadius,
        delete = Entity.delete
        }
    
    newEntity.index = addEntity(newEntity)
        
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
    
	if self.isPlayerTank then 
		--self:setVelocity(0, 0, 0)
        entity:delete()
	else
		self:setVelocity(vx, vy, vz)
	end
end

function Tank:new()
    local newTank = Entity:new()
    newTank.update = Tank.update
    newTank.onCollision = Tank.onCollision
    
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
        local t = allTheTanks[i]
        if t and t.index then t:update() end
        
        local chance = math.random()
        if false then
            t:setVelocity(randomDirection(), randomDirection(), 0)
        end
    end
end

function onMoveForward(intensity)
    playerTank.velocity.y = 0.05 * intensity
    playerTank:updateVelocity()
end

function onMoveBackward(intensity)
    playerTank.velocity.y = -0.05 * intensity
    playerTank:updateVelocity()
end

function onMoveLeft(intensity)
	playerTank.velocity.x = -0.05 * intensity
    playerTank:updateVelocity()
end

function onMoveRight(intensity)
    playerTank.velocity.x = 0.05 * intensity
    playerTank:updateVelocity()
end

function onSpace(intensity)
    if intensity < 1 then return end
    
    if playerTank.isBeingFollowed then
        playerTank.isBeingFollowed = false
        cameraUnfollow(true)
    else
        playerTank.isBeingFollowed = true
        cameraFollow(playerTank.index)
    end
end

function allTheThings()
	setTerrainSize(terrainSizeX, terrainSizeY)
	
	createCommand("Move Forward", onMoveForward, 119)
	createCommand("Move Backward", onMoveBackward, 115)
	createCommand("Move Left", onMoveLeft, 97)
	createCommand("Move Right", onMoveRight, 100)
	createCommand("Space", onSpace, 32)
	
    for i = 1, NumberOfTanks do
        local t = Tank:new()
        t:setPosition(randomLocation(), randomLocation(), 0)
        t:setVelocity(randomDirection(), randomDirection(), 0)
        allTheTanks[i] = t
    end
    
	playerTank = Tank:new()
	playerTank:setPosition(5, 5, 0)
    playerTank:setVelocity(0, 0, 0)
	playerTank.isPlayerTank = true
	playerTank.isBeingFollowed = true
	playerTank.velocity = { x = 0, y = 0, z = 0 }
	
	function playerTank:updateVelocity()
	    self:setVelocity(self.velocity.x, self.velocity.y, self.velocity.z)
	end
	
	cameraFollow(playerTank.index)
    
    setUpdateCallback(update)
end

allTheThings()

