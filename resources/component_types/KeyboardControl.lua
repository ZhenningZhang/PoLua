KeyboardControl = {
    OnStart = function(self)
        self.rb = self.actor:GetComponent("Rigidbody")
        self.frame = 0
        self.moving = false
        self.moveCoroutine = nil
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

    OnUpdate = function(self)
        -- Resume coroutine if it's running
        if self.moving and self.moveCoroutine then
            local status, error = coroutine.resume(self.moveCoroutine)
            if not status then
                print("Coroutine error:", error)
                self.moving = false
            end
        end

        -- Only start new movements if no current coroutine is running
        if Application.GetFrame() - self.frame >= 5 and not self.moving then
            self.frame = Application.GetFrame()

            if Input.GetKey("right") then
                local posX = self.rb:GetPosition().x + 0.5
                local posY = self.rb:GetPosition().y
                local rotation = self.rb:GetRotation() + 90

                self.moving = true
                self.moveCoroutine = coroutine.create(function()
                    self:UpdatePositionAndRotation(posX, posY, rotation, 1) -- 90 degrees in 1 second
                end)
            end

            if Input.GetKey("left") then
                local posX = self.rb:GetPosition().x - 0.5
                local posY = self.rb:GetPosition().y
                local rotation = self.rb:GetRotation() - 90

                self.moving = true
                self.moveCoroutine = coroutine.create(function()
                    self:UpdatePositionAndRotation(posX, posY, rotation, 1) -- 90 degrees in 1 second
                end)
            end
        end
    end
}