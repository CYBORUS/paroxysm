NumberOfTanks = 50
TankSpeed = 0.1
allTheTanks = {}

terrainSizeX = 16
terrainSizeY = 16

Entity = {}

function Entity:SetCollisionCallback(callback)
    EngineSetEntityCollisionCallback(self.index, callback)
end

function Entity:GetPosition()
    return EngineGetEntityPosition(self.index)
end

function Entity:SetPosition(x, y, z)
    EngineSetEntityPosition(self.index, x, y, z)
end

function Entity:GetVelocity()
    return EngineGetEntityVelocity(self.index)
end

function Entity:SetVelocity(x, y, z)
    return EngineSetEntityVelocity(self.index, x, y, z)
end
	
--Something is wrong with this function, needs looking at
function Entity:AddVelocity(x1, y1, z1)
	local x2, y2, z2 = self:GetVelocity()
	local x = x1 + x2
	local y = y1 + y2
	local z = z1 + z2
	self:SetVelocity(x, y, z)
end

function Entity:GetMass()
    return EngineGetEntityMass(self.index)
end

function Entity:SetMass(mass)
    EngineSetEntityMass(self.index, mass)
end

function Entity:GetRadius()
    return EngineGetEntityRadius(self.index)
end

function Entity:SetRadius(radius)
    EngineSetEntityRadius(self.index, radius)
end

function Entity:Delete()
    EngineRemoveEntity(self.index)
end

function Entity:New()
    local newEntity = {
        SetCollisionCallback = Entity.SetCollisionCallback,
        GetPosition = Entity.GetPosition,
        SetPosition = Entity.SetPosition,
        GetVelocity = Entity.GetVelocity,
        SetVelocity = Entity.SetVelocity,
        AddVelocity = Entity.AddVelocity,
        GetMass = Entity.GetMass,
        SetMass = Entity.SetMass,
        GetRadius = Entity.GetRadius,
        SetRadius = Entity.SetRadius,
        Delete = Entity.Delete
        }
    
    newEntity.index = EngineAddEntity(newEntity)
        
    return newEntity
end

Tank = {}

function Tank:Update()
    local px, py, pz = self:GetPosition()    
    local vx, vy, vz = self:GetVelocity()
    
    EngineResetEntityActorMatrix(self.index, self.actors.body)
    --EngineSetEntityPosition(self.index, px, py, pz)
    EngineSetEntityActorRotation(self.index, self.actors.body, 0, px * 10, 0)
    
    if px < 0.5 and vx < 0 then self:SetVelocity(-vx, vy, vz) end
    if px > terrainSizeX - 1.5 and vx > 0 then self:SetVelocity(-vx, vy, vz) end
    if py < 0.5 and vy < 0 then self:SetVelocity(vx, -vy, vz) end
    if py > terrainSizeY - 1.5 and vy > 0 then self:SetVelocity(vx, -vy, vz) end
end

function Tank:OnCollision(entity)
    local px, py, pz = self:GetPosition()    
    local vx, vy, vz = self:GetVelocity()    
    local px2, py2, pz2 = entity:GetPosition()    
    local vx2, vy2, vz2 = entity:GetVelocity()
    
    if px < px2 and vx > 0 then vx = -vx end
    if px > px2 and vx < 0 then vx = -vx end
    
    if py > py2 and vy < 0 then vy = -vy end
    if py < py2 and vy > 0 then vy = -vy end
    
	if self.isPlayerTank then 
		--self:setVelocity(0, 0, 0)
        entity:Delete()
	else
		self:SetVelocity(vx, vy, vz)
	end
end

function Tank:New()
    local newTank = Entity:New()
    newTank.Update = Tank.Update
    newTank.OnCollision = Tank.OnCollision
    
    EngineSetEntityDefaultRotation(newTank.index, 90, 0, 0)
    newTank:SetRadius(0.5)
    
    newTank.actors = {}
    newTank.actors.body = EngineAddActor(newTank.index,
        "data/models/bradley_body.c3m")
    newTank.actors.head = EngineAddActor(newTank.index,
        "data/models/bradley_head.c3m")
    newTank.actors.turret = EngineAddActor(newTank.index,
        "data/models/bradley_turret.c3m", newTank.actors.head)
    
    local collisionCallback = function(entity) newTank:OnCollision(entity) end
    newTank:SetCollisionCallback(collisionCallback)
    
    return newTank
end

function RandomLocation()
    return math.random() * terrainSizeX
end

function RandomDirection()
    return (math.random() - 0.5) * 0.05
end

function Update()
    for i = 1, NumberOfTanks do
        local t = allTheTanks[i]
        if t and t.index then t:Update() end
        
        local chance = math.random()
        if false then
            t:SetVelocity(RandomDirection(), RandomDirection(), 0)
        end
    end
end

function OnMoveForward(intensity)
    if intensity > 0 then
        playerTank.velocity.y = 0.05 * intensity
    elseif playerTank.velocity.y > 0 then
        playerTank.velocity.y = 0
    end
    
    playerTank:UpdateVelocity()
end

function OnMoveBackward(intensity)
    if intensity > 0 then
        playerTank.velocity.y = -0.05 * intensity
    elseif playerTank.velocity.y < 0 then
        playerTank.velocity.y = 0
    end
    
    playerTank:UpdateVelocity()
end

function OnMoveLeft(intensity)
    if intensity > 0 then
        playerTank.velocity.x = -0.05 * intensity
    elseif playerTank.velocity.x < 0 then
        playerTank.velocity.x = 0
    end
    
    playerTank:UpdateVelocity()
end

function OnMoveRight(intensity)
    if intensity > 0 then
        playerTank.velocity.x = 0.05 * intensity
    elseif playerTank.velocity.x > 0 then
        playerTank.velocity.x = 0
    end

    playerTank:UpdateVelocity()
end

function OnSpace(intensity)
    if intensity < 1 then return end
    
    if playerTank.isBeingFollowed then
        playerTank.isBeingFollowed = false
        cameraUnfollow(true)
    else
        playerTank.isBeingFollowed = true
        cameraFollow(playerTank.index)
    end
end

function AllTheThings()
	EngineSetTerrainSize(terrainSizeX, terrainSizeY)
	
	EngineCreateCommand("Move Forward", OnMoveForward, 119)
	EngineCreateCommand("Move Backward", OnMoveBackward, 115)
	EngineCreateCommand("Move Left", OnMoveLeft, 97)
	EngineCreateCommand("Move Right", OnMoveRight, 100)
	EngineCreateCommand("Space", OnSpace, 32)
	EngineCreateCommand("Debug Output", DebugOutput, 92)
	
    for i = 1, NumberOfTanks do
        local t = Tank:New()
        t:SetPosition(RandomLocation(), RandomLocation(), 0)
        t:SetVelocity(RandomDirection(), RandomDirection(), 0)
        allTheTanks[i] = t
    end
    
	playerTank = Tank:New()
	playerTank:SetPosition(5, 5, 0)
    playerTank:SetVelocity(0, 0, 0)
	playerTank.isPlayerTank = true
	playerTank.isBeingFollowed = true
	playerTank.velocity = { x = 0, y = 0, z = 0 }
	
	function playerTank:UpdateVelocity()
	    self:SetVelocity(self.velocity.x, self.velocity.y, self.velocity.z)
	end
	
	EngineCameraFollow(playerTank.index)
    
    EngineSetUpdateCallback(Update)
end

AllTheThings()

