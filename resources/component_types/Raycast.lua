Raycast = {

    OnStart = function(self)
        self.SpriteRenderer = self.actor:GetComponent("SpriteRenderer")
        self.rb = self.actor:GetComponent("Rigidbody")
        self.moving = false
        self.floating = false
        self.moveCoroutine = nil
        self.leftMovable = true
        self.rightMovable = true
    end,

    UpdatePositionAndRotation = function(self, targetX, targetY, targetRotation, duration)
        local startX, startY = self.rb:GetPosition().x, self.rb:GetPosition().y
        local startRotation = self.rb:GetRotation()
        local startTime = os.clock()
        local endTime = startTime + duration

        while os.clock() < endTime do
            local t = (os.clock() - startTime) / duration
            local newPosX = startX + (targetX - startX) * t
            local newPosY = startY + (targetY - startY) * t
            local newRotation = startRotation + (targetRotation - startRotation) * t

            self.rb:SetPosition(Vector2(newPosX, newPosY))
            self.rb:SetRotation(newRotation)
            coroutine.yield() -- Yield to allow smooth transition over the duration
        end

        -- Ensure final position and rotation
        self.rb:SetPosition(Vector2(targetX, targetY))
        self.rb:SetRotation(targetRotation)
        self.moving = false
    end,

    RaycastByDirecAndColor = function(self, x, y, isRed)
        local direc = Vector2(x, y)
        local results = Physics.RaycastAll(self.rb:GetPosition(), direc, 100)
        for index, result in ipairs(results) do
            local resultActor = result.actor
            if resultActor:GetComponent("Magnet") ~= nil then
                if isRed == true then
                    if resultActor:GetComponent("SpriteRenderer").sprite == "box2" and resultActor:GetComponent("Magnet").active == true and self.moving == false and self.actor:GetComponent("KeyboardControl").moving == false then -- suppose to be true
                        local distance = Vector2.Distance(self.rb:GetPosition(), results[1].actor:GetComponent("Rigidbody"):GetPosition()) - 0.5
                        if results[1].actor:GetComponent("Oneway") ~= nil then
                            distance = Vector2.Distance(self.rb:GetPosition(), results[2].actor:GetComponent("Rigidbody"):GetPosition()) - 0.5
                        end
                        local posX = self.rb:GetPosition().x + direc:__mul(distance).x
                        local posY = self.rb:GetPosition().y + direc:__mul(distance).y
                        local rotation = self.rb:GetRotation()
                        local duration = distance / 2

                        self.moving = true
                        self.moveCoroutine = coroutine.create(function()
                            self:UpdatePositionAndRotation(posX, posY, rotation, duration) -- 90 degrees in 1 second
                        end)
                    end
                end
                if isRed == false then
                    if resultActor:GetComponent("SpriteRenderer").sprite == "box1" and resultActor:GetComponent("Magnet").active == true and self.moving == false and self.actor:GetComponent("KeyboardControl").moving == false then -- suppose to be true
                        local distance = Vector2.Distance(self.rb:GetPosition(), results[1].actor:GetComponent("Rigidbody"):GetPosition()) - 0.5
                        if results[1].actor:GetComponent("Oneway") ~= nil then
                            distance = Vector2.Distance(self.rb:GetPosition(), results[2].actor:GetComponent("Rigidbody"):GetPosition()) - 0.5
                        end
                        local posX = self.rb:GetPosition().x + direc:__mul(distance).x
                        local posY = self.rb:GetPosition().y + direc:__mul(distance).y
                        local rotation = self.rb:GetRotation()
                        local duration = distance / 2

                        self.moving = true
                        self.moveCoroutine = coroutine.create(function()
                            self:UpdatePositionAndRotation(posX, posY, rotation, duration) -- 90 degrees in 1 second
                        end)
                    end
                end
            end
        end
    end,

    RaycastDown = function(self)
        local result = Physics.Raycast(self.rb:GetPosition(), Vector2(0,1), 100)
        local resultActor = result.actor
        local distance = Vector2.Distance(self.rb:GetPosition(), resultActor:GetComponent("Rigidbody"):GetPosition())
        if distance > 0.6 then
            self.floating = true
            if self.moving == false and self.actor:GetComponent("KeyboardControl").moving == false then
                local posX = self.rb:GetPosition().x
                local posY = resultActor:GetComponent("Rigidbody"):GetPosition().y - 0.5
                local rotation = self.rb:GetRotation()
                local duration = (distance - 0.5) / 2

                self.moving = true
                self.moveCoroutine = coroutine.create(function()
                    self:UpdatePositionAndRotation(posX, posY, rotation, duration) -- 90 degrees in 1 second
                end)
            end
        end
        if distance <= 0.6 then
            self.floating = false
        end
    end,

    RaycastLeft = function(self)
        local result = Physics.Raycast(self.rb:GetPosition(), Vector2(-1,0), 100)
        if result ~= nil then
            local resultActor = result.actor
            local distance = Vector2.Distance(self.rb:GetPosition(), resultActor:GetComponent("Rigidbody"):GetPosition())
            if distance < 0.6 then
                self.leftMovable = false
            else
                self.leftMovable = true
            end
        end
        if result == nil then
            self.leftMovable = true
        end
    end,

    RaycastRight = function(self)
        local result = Physics.Raycast(self.rb:GetPosition(), Vector2(1,0), 100)
        if result ~= nil then
            local resultActor = result.actor
            local distance = Vector2.Distance(self.rb:GetPosition(), resultActor:GetComponent("Rigidbody"):GetPosition())
            if distance < 0.6 then
                self.rightMovable = false
            end
            if distance >= 0.6 then
                self.rightMovable = true
            end
        end
        if result == nil then
            self.rightMovable = true
        end
    end,

    OnUpdate = function(self)
        if self.moving and self.moveCoroutine then
            local status, error = coroutine.resume(self.moveCoroutine)
            if not status then
                print("Coroutine error:", error)
                self.moving = false
            end
        end

        local status = self.actor:GetComponent("KeyboardControl").status
        self:RaycastDown()
        self:RaycastLeft()
        self:RaycastRight()
        if status == 0 then
            self:RaycastByDirecAndColor(1, 0, true)
            self:RaycastByDirecAndColor(-1, 0, false)
        end
        if status == 1 then
            self:RaycastByDirecAndColor(0, 1, true)
            self:RaycastByDirecAndColor(0, -1, false)
        end
        if status == 2 then
            self:RaycastByDirecAndColor(-1, 0, true)
            self:RaycastByDirecAndColor(1, 0, false)
        end
        if status == 3 then
            self:RaycastByDirecAndColor(0, -1, true)
            self:RaycastByDirecAndColor(0, 1, false)
        end
    end
}